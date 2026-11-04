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
  YYSYMBOL_spec_kw = 117,                  /* spec_kw  */
  YYSYMBOL_spec_ref = 118,                 /* spec_ref  */
  YYSYMBOL_type_arg_list = 119,            /* type_arg_list  */
  YYSYMBOL_type = 120,                     /* type  */
  YYSYMBOL_121_1 = 121,                    /* $@1  */
  YYSYMBOL_122_2 = 122,                    /* $@2  */
  YYSYMBOL_123_3 = 123,                    /* $@3  */
  YYSYMBOL_result_type = 124,              /* result_type  */
  YYSYMBOL_class_members = 125,            /* class_members  */
  YYSYMBOL_class_member = 126,             /* class_member  */
  YYSYMBOL_prop_index_opt = 127,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 128,               /* prop_specs  */
  YYSYMBOL_packed_opt = 129,               /* packed_opt  */
  YYSYMBOL_simple_type = 130,              /* simple_type  */
  YYSYMBOL_lo_bound = 131,                 /* lo_bound  */
  YYSYMBOL_lo_term = 132,                  /* lo_term  */
  YYSYMBOL_b_expr = 133,                   /* b_expr  */
  YYSYMBOL_b_term = 134,                   /* b_term  */
  YYSYMBOL_b_factor = 135,                 /* b_factor  */
  YYSYMBOL_record_body = 136,              /* record_body  */
  YYSYMBOL_record_field_list = 137,        /* record_field_list  */
  YYSYMBOL_record_field = 138,             /* record_field  */
  YYSYMBOL_139_4 = 139,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 140,            /* case_arm_mark  */
  YYSYMBOL_case_open = 141,                /* case_open  */
  YYSYMBOL_record_case_opt = 142,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 143,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 144,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 145,            /* var_decl_list  */
  YYSYMBOL_var_decl = 146,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 147,           /* procedure_decl  */
  YYSYMBOL_148_5 = 148,                    /* $@5  */
  YYSYMBOL_149_6 = 149,                    /* $@6  */
  YYSYMBOL_150_7 = 150,                    /* $@7  */
  YYSYMBOL_151_8 = 151,                    /* $@8  */
  YYSYMBOL_152_9 = 152,                    /* $@9  */
  YYSYMBOL_153_10 = 153,                   /* $@10  */
  YYSYMBOL_154_11 = 154,                   /* $@11  */
  YYSYMBOL_155_12 = 155,                   /* $@12  */
  YYSYMBOL_156_13 = 156,                   /* $@13  */
  YYSYMBOL_157_14 = 157,                   /* $@14  */
  YYSYMBOL_158_15 = 158,                   /* $@15  */
  YYSYMBOL_159_16 = 159,                   /* $@16  */
  YYSYMBOL_160_17 = 160,                   /* $@17  */
  YYSYMBOL_pv_mark = 161,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 162,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 163,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 164,           /* parameter_decl  */
  YYSYMBOL_param_type = 165,               /* param_type  */
  YYSYMBOL_routine_directives = 166,       /* routine_directives  */
  YYSYMBOL_pf_params = 167,                /* pf_params  */
  YYSYMBOL_pf_sections = 168,              /* pf_sections  */
  YYSYMBOL_pf_section = 169,               /* pf_section  */
  YYSYMBOL_enum_list = 170,                /* enum_list  */
  YYSYMBOL_enum_member = 171,              /* enum_member  */
  YYSYMBOL_id_list = 172,                  /* id_list  */
  YYSYMBOL_body = 173,                     /* body  */
  YYSYMBOL_statement_list = 174,           /* statement_list  */
  YYSYMBOL_175_18 = 175,                   /* $@18  */
  YYSYMBOL_176_19 = 176,                   /* $@19  */
  YYSYMBOL_body_stmt = 177,                /* body_stmt  */
  YYSYMBOL_178_20 = 178,                   /* $@20  */
  YYSYMBOL_statement = 179,                /* statement  */
  YYSYMBOL_statement_no_label = 180,       /* statement_no_label  */
  YYSYMBOL_try_statement = 181,            /* try_statement  */
  YYSYMBOL_handler_list = 182,             /* handler_list  */
  YYSYMBOL_handler = 183,                  /* handler  */
  YYSYMBOL_raise_statement = 184,          /* raise_statement  */
  YYSYMBOL_inherited_call = 185,           /* inherited_call  */
  YYSYMBOL_call = 186,                     /* call  */
  YYSYMBOL_recv_primary = 187,             /* recv_primary  */
  YYSYMBOL_call_with_args = 188,           /* call_with_args  */
  YYSYMBOL_argument_list = 189,            /* argument_list  */
  YYSYMBOL_argument = 190,                 /* argument  */
  YYSYMBOL_assignment = 191,               /* assignment  */
  YYSYMBOL_selector = 192,                 /* selector  */
  YYSYMBOL_expression_list = 193,          /* expression_list  */
  YYSYMBOL_compound_statement = 194,       /* compound_statement  */
  YYSYMBOL_goto_statement = 195,           /* goto_statement  */
  YYSYMBOL_if_statement = 196,             /* if_statement  */
  YYSYMBOL_case_statement = 197,           /* case_statement  */
  YYSYMBOL_198_21 = 198,                   /* $@21  */
  YYSYMBOL_case_tail = 199,                /* case_tail  */
  YYSYMBOL_case_list = 200,                /* case_list  */
  YYSYMBOL_case_elem = 201,                /* case_elem  */
  YYSYMBOL_constant_list = 202,            /* constant_list  */
  YYSYMBOL_case_label = 203,               /* case_label  */
  YYSYMBOL_while_statement = 204,          /* while_statement  */
  YYSYMBOL_repeat_statement = 205,         /* repeat_statement  */
  YYSYMBOL_for_statement = 206,            /* for_statement  */
  YYSYMBOL_with_statement = 207,           /* with_statement  */
  YYSYMBOL_with_open = 208,                /* with_open  */
  YYSYMBOL_expression = 209,               /* expression  */
  YYSYMBOL_simple_expression = 210,        /* simple_expression  */
  YYSYMBOL_term = 211,                     /* term  */
  YYSYMBOL_power = 212,                    /* power  */
  YYSYMBOL_factor = 213,                   /* factor  */
  YYSYMBOL_set_member_list = 214,          /* set_member_list  */
  YYSYMBOL_set_member = 215                /* set_member  */
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
#define YYLAST   1488

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  121
/* YYNRULES -- Number of rules.  */
#define YYNRULES  370
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  867

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
     314,   315,   318,   321,   322,   325,   326,   329,   330,   331,
     332,   332,   332,   333,   334,   335,   336,   337,   338,   339,
     339,   340,   341,   342,   343,   344,   347,   348,   349,   352,
     353,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   368,   369,   372,   373,   374,   377,   378,   381,   382,
     383,   384,   385,   386,   389,   390,   391,   392,   393,   396,
     397,   398,   399,   400,   401,   404,   405,   406,   407,   408,
     411,   412,   413,   414,   415,   418,   419,   420,   423,   426,
     427,   430,   430,   431,   434,   437,   440,   441,   442,   445,
     446,   449,   450,   453,   454,   457,   458,   459,   462,   463,
     464,   464,   465,   465,   466,   466,   467,   467,   468,   468,
     469,   469,   470,   470,   471,   471,   472,   472,   475,   475,
     478,   478,   481,   481,   484,   484,   489,   492,   493,   494,
     497,   498,   501,   502,   503,   504,   505,   506,   507,   508,
     511,   512,   513,   514,   517,   518,   528,   529,   530,   533,
     534,   537,   538,   539,   540,   541,   544,   545,   548,   551,
     552,   555,   558,   558,   559,   559,   562,   562,   565,   566,
     567,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   587,   588,   589,   590,
     593,   594,   595,   598,   599,   602,   603,   606,   607,   608,
     611,   612,   613,   614,   617,   618,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   633,   634,   637,   638,
     639,   642,   643,   644,   645,   646,   647,   650,   651,   652,
     653,   654,   655,   656,   657,   658,   661,   662,   665,   668,
     669,   672,   673,   676,   676,   679,   680,   681,   684,   685,
     688,   689,   692,   693,   696,   697,   700,   703,   706,   707,
     708,   711,   714,   715,   722,   723,   724,   725,   726,   727,
     728,   729,   730,   733,   734,   735,   736,   737,   738,   739,
     742,   743,   744,   745,   746,   747,   748,   749,   750,   751,
     752,   753,   754,   757,   758,   761,   762,   763,   764,   765,
     766,   767,   768,   769,   770,   771,   772,   775,   776,   779,
     780
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
  "type_decl_list", "type_decl", "spec_kw", "spec_ref", "type_arg_list",
  "type", "$@1", "$@2", "$@3", "result_type", "class_members",
  "class_member", "prop_index_opt", "prop_specs", "packed_opt",
  "simple_type", "lo_bound", "lo_term", "b_expr", "b_term", "b_factor",
  "record_body", "record_field_list", "record_field", "$@4",
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

#define YYPACT_NINF (-515)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-296)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     295,   -23,    72,   162,  -515,   112,   597,  -515,  -515,   374,
     229,  -515,  -515,    73,   124,   174,   180,   174,   174,   207,
    -515,   182,   261,   271,   287,   309,  -515,  -515,  -515,   300,
     174,   355,   395,  -515,  -515,    65,   145,   124,  -515,  -515,
      68,     0,   174,  -515,   141,    70,   330,    41,   718,   443,
     353,   182,  -515,   367,   389,   373,   471,   401,   467,  -515,
     286,  -515,  -515,  -515,   228,  1058,   462,  -515,  -515,   470,
     174,   482,   407,  -515,   462,  -515,   174,   493,    39,  -515,
    -515,   538,   524,  -515,  1058,   149,  1058,  -515,  1058,  1058,
     473,  -515,  1409,  -515,  1058,   532,   537,   560,  -515,  -515,
    -515,  -515,  -515,  -515,   316,   190,  -515,   148,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,   174,   640,   593,  -515,
     513,   587,   574,   577,   608,   613,  -515,   614,   354,  -515,
    -515,  1226,  1226,  1226,   904,  1058,  1226,  -515,  -515,  -515,
     349,   594,   532,   560,  -515,   523,   531,    40,   278,   598,
     590,  -515,   615,  -515,   615,   689,   689,    84,   607,  -515,
    -515,  -515,  -515,  1390,   620,  -515,  -515,  -515,   654,   284,
    -515,   272,   119,  -515,   386,  -515,   194,   688,    28,   409,
    -515,  -515,   638,   718,  -515,  -515,   580,   234,   371,   523,
     531,    25,   903,    42,  1273,  1290,   810,   810,   932,    22,
    1380,   646,   663,   958,   650,  -515,  1058,   660,  -515,  1058,
    1058,   665,  -515,  1058,  1058,  1058,  1058,  1058,   412,    47,
     713,   174,   668,   674,  -515,  -515,   174,   174,  -515,   124,
     174,  -515,   675,   174,   678,   182,  -515,  -515,   598,   598,
    -515,  1308,   486,  -515,  1327,  -515,   683,  -515,  1058,  1058,
    1058,  1058,  1058,  1058,  1058,  1058,  1226,  1226,  1226,  1226,
    1126,  1142,  1168,  1184,  1210,  1226,  1226,  1226,   195,   694,
     685,   671,   119,   119,   720,  -515,  -515,   432,  -515,  1255,
     693,  1309,  1309,   174,   826,  1084,    85,   744,   746,  -515,
     430,   430,  1309,   430,   430,   430,   430,  -515,   407,   174,
     699,   174,   700,  -515,   174,    29,  -515,   453,   768,  -515,
    1084,   708,  -515,   407,  -515,   179,  -515,  1058,  1058,  1058,
    -515,  -515,   149,  -515,  -515,  -515,  -515,   387,  -515,  -515,
    -515,   435,  -515,  1345,   695,  -515,   974,  1255,  -515,   449,
     723,  1380,   724,   504,  1380,  1380,  1389,   508,  1380,  1380,
    1380,  1380,   736,  -515,   174,  -515,   418,  -515,  -515,   407,
     407,   422,   425,   124,    75,   771,  -515,   174,  -515,   182,
    1058,  1058,  -515,   578,   727,   278,   278,   278,   278,   278,
     278,   278,   278,   598,   598,   598,   598,  1226,   590,  1226,
     590,  1226,   590,  1226,   590,  1226,   590,   590,   590,  -515,
     174,   716,   174,   719,  -515,    30,  -515,   589,   726,   179,
     607,  -515,   428,  -515,  -515,   430,   430,  1309,   745,  -515,
     363,   221,  -515,   103,   457,   735,  -515,   739,    64,   333,
     741,   742,   743,  -515,   599,  1100,   795,  1380,   462,  1255,
     462,  1255,   174,   221,   221,   275,  -515,  -515,  -515,  -515,
     407,   815,   610,   615,   632,   615,   639,   206,  -515,    24,
     816,   747,   817,   821,   407,   767,   823,   352,  -515,   827,
     869,   528,  1380,  -515,   718,  -515,   531,  1309,   792,  1058,
    -515,  1058,   761,   137,   105,  -515,   199,  -515,   463,   434,
    -515,  1000,  1016,  1058,  -515,  1042,  -515,   462,   474,   787,
     407,   407,   836,   837,  -515,  -515,  -515,  -515,  -515,   830,
     407,   407,  1380,  -515,   590,   590,   590,   590,   590,   642,
     615,   645,   615,   329,  -515,    24,  -515,   818,  -515,  -515,
    1255,   221,   221,   237,  1309,   430,   430,  -515,   430,   430,
     430,   430,  -515,  -515,  -515,  -515,  -515,   773,   777,  -515,
    -515,   793,   462,   561,   796,    44,  1327,  -515,  -515,   510,
    -515,  -515,   828,   111,  -515,   659,   852,  -515,    24,  -515,
      24,   804,    24,  -515,   838,   356,  -515,   812,  -515,   870,
    -515,  -515,  -515,   820,   179,  -515,   800,   872,  1058,  1058,
    -515,  -515,   288,   136,  -515,   669,  -515,  -515,  -515,  1361,
      67,  -515,   695,  -515,  -515,  -515,  -515,  -515,  -515,   490,
    -515,   494,  1380,  -515,   496,   873,   874,   462,   877,   824,
    -515,  -515,   407,   407,   883,   883,  -515,   859,   885,   832,
      24,  -515,    24,   834,  -515,  -515,   822,  -515,  -515,   303,
     221,   221,  -515,  -515,  -515,  -515,   847,   407,   407,  -515,
    -515,   407,   407,   206,   835,   894,  1084,  1084,   220,  -515,
    1255,   875,  -515,   174,   825,  -515,  -515,  -515,   747,   854,
    -515,   179,  -515,   829,   833,  1058,  -515,   905,   179,   907,
    -515,   908,   747,   546,   819,  -515,  1309,  1309,  -515,  -515,
     878,  1309,  -515,  -515,  1058,  -515,   848,  -515,   203,  -515,
    -515,  -515,  -515,  -515,   920,  -515,   179,   747,   747,   921,
     868,   883,   871,  -515,   179,  -515,  -515,   179,  -515,  -515,
    -515,   924,   879,   407,   407,   927,   928,    49,   858,  -515,
    -515,  -515,   499,  -515,   880,   886,  -515,  -515,    69,   462,
     747,  -515,  1058,  -515,  -515,  -515,  1380,  -515,   933,  -515,
    -515,  -515,  -515,  -515,   275,  -515,   883,   883,  -515,  -515,
    -515,  1380,  -515,   925,  -515,  -515,   747,   936,  -515,  -515,
    -515,   179,  -515,   941,  -515,  -515,   179,   943,   897,  -515,
    -515,  -515,  -515,  1084,  -515,  1084,   934,  -515,  -515,   884,
    -515,  -515,   956,  1380,  -515,   747,   959,  -515,  -515,  -515,
    -515,  -515,   960,   964,   747,   965,  -515,   967,  -515,   179,
      32,  -515,  -515,   462,   462,  1309,   935,   968,  -515,   747,
    -515,  -515,  -515,   969,   747,  -515,  -515,  -515,  -515,  -515,
     976,  -515,   554,  -515,  -515,   977,  -515,   672,  -515,  -515,
    -515,   978,  -515,  -515,   980,   747,  -515,  -515,  -515,  1309,
     926,  1309,   983,  -515,   984,  -515,  -515,  -515,  -515,   977,
    -515,  -515,   987,   174,  -515,   930,  -515
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,     0,
     224,     0,     0,     0,     0,     0,    11,    17,    10,     0,
       0,     0,     0,    36,    37,     0,     0,    14,    39,   220,
       0,   186,    16,   154,     0,     0,   186,     0,   245,     0,
       0,    15,    56,     0,     0,     0,     0,     0,     0,     7,
       0,    12,    22,    13,     0,     0,   107,    38,    18,     0,
       0,     0,   189,   153,   107,    19,     0,     0,   189,   222,
     221,     0,     0,   224,     0,     0,     0,   224,     0,     0,
       0,   264,   260,   224,   255,   244,     0,   261,   225,   228,
     241,   242,   243,   232,     0,   263,   231,     0,   233,   234,
     235,   236,   237,   238,   239,   240,     0,   107,     0,    55,
       0,     0,     0,     0,     0,     0,     8,     0,     0,    34,
      35,     0,     0,     0,     0,     0,     0,   357,   360,   358,
     359,   295,     0,     0,   362,   356,   355,     0,   324,   333,
     340,   353,   208,   106,   208,     0,     0,     0,     0,    51,
      54,    52,    53,   109,    89,   119,    63,   112,     0,     0,
      67,     0,   114,   219,     0,   186,     0,     0,     0,     0,
     186,   164,     0,   245,   299,   300,     0,     0,     0,     0,
     323,     0,     0,     0,     0,     0,   245,   245,     0,     0,
     256,   257,     0,     0,     0,   294,     0,     0,   292,     0,
       0,     0,   289,     0,     0,     0,     0,     0,     0,    89,
       0,     0,     0,     0,   186,   186,     0,     0,     4,     0,
       0,    12,     0,     0,     0,     0,    21,   363,   334,   335,
     365,   369,     0,   367,     0,   364,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,    50,   115,   116,    68,    69,   218,     0,   217,     0,
       0,     0,     0,     0,     0,     0,     0,    82,     0,    79,
       0,     0,     0,     0,     0,     0,     0,   186,   189,     0,
       0,     0,     0,   188,   220,     0,   191,     0,   204,   155,
       0,     0,   186,   189,    12,     0,   223,     0,     0,     0,
     226,   226,     0,   303,   298,   226,   265,   260,   229,   230,
     269,     0,   277,   278,   224,   224,     0,     0,   268,     0,
     293,   286,   290,     0,   297,   281,   262,     0,   282,   283,
     284,   285,     0,    60,     0,    57,     0,   186,   186,   189,
     189,     0,     0,    24,     0,    31,   186,    26,   186,    25,
       0,     0,   366,   361,   288,   325,   326,   327,   328,   329,
     330,   331,   332,   338,   336,   337,   339,     0,   343,     0,
     344,     0,   345,     0,   341,     0,   342,   351,   352,   354,
       0,     0,     0,     0,   207,     0,   210,     0,     0,     0,
       0,   108,     0,    65,   111,     0,     0,     0,    50,   135,
       0,   125,   130,     0,     0,     0,    84,     0,    91,     0,
       0,     0,     0,    90,     0,     0,     0,    42,   107,     0,
     107,     0,   143,   117,   118,   113,   122,   123,   121,   124,
     189,     0,     0,   208,     0,   208,     0,     0,   187,     0,
       0,   160,     0,     0,   189,     0,     0,    86,    88,     0,
       0,     0,   317,   316,   245,   321,   322,   311,   301,     0,
     266,     0,     0,     0,     0,   250,     0,   258,     0,     0,
     267,     0,     0,     0,   291,     0,   287,   107,     0,     0,
     189,   189,     0,     0,   186,   186,    23,   224,   224,    33,
     189,   189,   370,   368,   348,   349,   350,   346,   347,     0,
     208,     0,   208,     0,   206,     0,    75,    76,   216,    64,
       0,   126,   127,     0,     0,     0,     0,   110,     0,     0,
       0,     0,   120,    89,   186,   186,    92,     0,     0,   186,
     186,   101,   107,   295,     0,     0,    42,    41,    73,     0,
      83,    81,     0,   148,   140,     0,     0,   204,     0,   192,
       0,     0,     0,   190,     0,   200,   203,   195,   158,     0,
      12,   156,   157,     0,     0,   165,     0,   204,     0,     0,
     226,   227,   314,   305,   309,     0,   313,   226,   276,   279,
       0,   246,   252,   247,   224,   249,   259,    62,   275,     0,
     273,     0,   296,   271,     0,     0,    89,   107,     0,     0,
     204,   204,   189,   189,    29,    30,   224,     0,     0,     0,
       0,   214,     0,     0,   209,   211,     0,    66,   137,     0,
     128,   129,   132,   133,   131,   134,     0,   189,   189,   186,
     186,   189,   189,     0,     0,     0,     0,     0,     0,    44,
       0,     0,    80,   143,     0,   138,   141,   204,   174,   196,
     194,     0,   199,     0,     0,     0,   205,     0,     0,     0,
      87,     0,   162,     0,     0,   320,     0,   311,   224,   224,
       0,     0,   226,   302,     0,   226,     0,   251,     0,   274,
     272,   270,    59,    61,     0,   204,     0,   178,   180,     0,
       0,    32,     0,    27,     0,   213,   212,     0,    77,   136,
      85,     0,     0,   189,   189,     0,     0,     0,     0,    93,
      48,    46,     0,    45,     0,     0,    70,   139,     0,   107,
     170,    12,     0,   193,   202,   201,   197,   161,     0,   204,
     159,    12,   226,   226,   315,   308,   307,   306,   304,   312,
     310,   280,   254,     0,   248,    58,   182,     0,    12,    12,
     204,     0,    20,     0,   215,    94,     0,     0,     0,    96,
      97,   102,   103,     0,    43,     0,     0,    71,   145,     0,
     142,    12,     0,   198,   204,   176,     0,   319,   318,   226,
      12,   204,     0,     0,   166,     0,    28,     0,    98,     0,
       0,    47,    49,   107,   107,   152,     0,     0,   175,   172,
      12,   163,   253,     0,   184,   179,   181,    12,   204,    95,
       0,   100,     0,    78,    72,   147,   150,     0,   145,   171,
      12,     0,   183,    12,     0,   168,    99,   105,   104,   152,
       0,   152,     0,   177,     0,   167,    12,   149,   144,   146,
     173,   185,     0,   143,   169,     0,   151
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -515,  -515,  -515,  -515,  -515,   -60,   762,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,   774,   -29,  -304,  -515,  -515,   -57,
     765,   -38,   -43,  -131,   673,   -67,  -515,  -515,  -515,  -397,
    -514,  -515,  -515,  -515,  -515,  -251,  -515,   236,  -241,  -272,
    -217,   139,  -515,   346,  -515,  -515,   173,  -515,   163,   166,
     783,   -31,  -515,  -515,  -515,  -515,  -515,  -515,  -515,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,   294,   101,   365,   564,
    -438,  -482,  -150,  -515,   501,  -515,   617,   -15,  -515,   -73,
    -515,  -515,  -289,  -515,  -167,   468,  -515,  -515,   433,  -515,
     -24,  -515,  -515,   -46,  -181,   558,  -515,   -27,   831,  -515,
    -515,  -515,  -515,  -515,  -515,  -515,   364,  -452,   351,  -515,
    -515,  -515,  -515,  -515,    31,  1233,   -76,  -198,   -93,  -515,
     682
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    31,     5,     6,    26,     7,   128,
     236,   509,   627,    35,    37,    38,   436,   732,   555,   419,
      51,    52,   143,   167,   412,   168,   787,   814,   442,   469,
     284,   433,   654,   810,   169,   170,   171,   172,   592,   421,
     422,   562,   563,   564,   739,   863,   815,   665,   835,   836,
      42,    43,    27,   580,   751,   314,   827,   856,   791,   840,
     741,   820,   768,   769,   800,   843,    72,   177,   305,   306,
     577,   461,   269,   405,   406,   277,   278,    44,    28,    47,
     183,    48,   473,   474,    98,    99,   100,   484,   485,   101,
     144,   103,   104,   145,   331,   332,   106,   146,   343,   108,
     109,   110,   111,   477,   690,   593,   594,   837,   596,   112,
     113,   114,   115,   191,   333,   148,   149,   150,   151,   242,
     243
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,   127,   105,    45,   270,    97,   462,   178,    67,   165,
     187,    73,   527,   119,   193,    60,   316,   165,   443,   444,
     199,   107,   339,   166,   102,   595,   275,    79,   413,   646,
     574,   166,   475,   309,   457,   523,   478,   831,   237,   189,
     420,   423,   321,   245,   181,   247,    79,    79,    70,   658,
     220,   445,   353,     8,   457,   174,   238,   239,   190,    71,
     165,   179,   388,   390,   392,   394,   396,   397,   398,   546,
      63,    80,   324,    68,   166,    75,   446,   447,   448,   449,
     506,   310,   248,   322,   695,   668,   413,   635,   249,   250,
     251,   252,   253,   254,   458,   524,   147,   788,   165,   165,
     575,   218,   646,   176,   311,   682,   334,   335,   832,   659,
     602,   354,   781,   438,   166,   188,   663,   192,    96,   194,
     195,   255,  -220,    64,  -220,   200,    69,   696,    69,   789,
     669,   554,   670,    69,   672,   603,   664,   105,   707,   708,
      97,   687,    79,   531,   532,    33,   604,   439,    10,    34,
     105,   105,   688,    97,    97,   488,   107,   535,   536,   102,
     274,   307,    11,   293,   294,   241,   244,   601,   542,   107,
     107,    12,   102,   102,   399,   295,   533,   689,    96,   182,
     383,   384,   385,   386,   468,   740,   210,   679,   559,   514,
     561,   515,   715,   516,   716,   517,   296,   518,    65,    69,
      36,    74,   299,   400,    79,    66,   356,   211,    79,   212,
     213,   361,   362,    89,   299,   364,   214,   215,   216,   217,
     300,   401,   165,   766,    91,   141,   301,   402,   206,   605,
     302,   403,   300,   764,    32,   595,   166,   341,   301,    79,
     344,   345,   302,    96,   344,   348,   349,   350,   351,   207,
      39,   208,   209,   407,   466,   467,    41,   319,    49,   303,
     404,   483,   486,   640,   641,   538,   539,   795,   424,   434,
     304,    39,   166,    96,   743,    50,   189,   540,   468,   637,
     165,   748,   304,    46,   452,   733,   454,    53,   804,   456,
     286,   535,   536,   639,   166,   476,   734,    54,   541,     1,
     129,   685,   638,   569,   130,   571,     2,   591,   693,   767,
     609,   611,   819,   287,   614,   288,   437,   773,   289,   824,
     774,   642,   643,   644,   645,   256,   290,   291,   576,   535,
     536,   119,   257,   258,    67,    57,    73,   400,   292,   498,
      78,   437,   535,   536,    69,    58,   845,    55,   470,   471,
     472,   126,   730,   731,   686,   401,   259,   535,   536,   547,
      61,   402,   229,    56,   230,   403,   166,   231,   719,   548,
     631,   558,   633,   560,   805,   204,    59,   205,    76,   807,
     232,   165,   165,   165,   165,   519,   233,   521,   320,    77,
     234,   272,   273,   235,   576,   166,   166,   166,   166,   451,
     279,   512,   241,   760,   279,    39,   762,    62,  -264,   735,
    -264,   586,   830,   248,   465,   674,   166,   535,   536,   249,
     250,   251,   252,   253,   254,  -295,   537,   565,   105,   118,
     615,    97,   122,    29,   624,   625,   297,   576,    30,   576,
     165,   576,   307,   120,    69,   754,  -295,   107,  -295,  -295,
     102,   198,   255,   468,   166,  -295,  -295,  -295,  -295,   312,
     502,   503,   352,   797,   798,   121,   556,    69,   499,   298,
      69,   176,   504,   165,   313,   505,    69,   124,   529,   811,
      69,   812,   166,    69,   607,   655,   530,   166,   152,   153,
     410,   116,   530,   479,   417,   165,   117,   411,   154,   576,
     480,   576,   159,   160,   161,   162,   418,   479,   407,   166,
     822,    69,   599,   459,   490,    69,   155,   156,   359,   360,
     677,   479,   543,   157,   612,   166,   158,   166,   606,   166,
     123,   698,    69,   196,   159,   160,   161,   162,   163,   616,
     468,   166,   164,   125,   371,   590,   173,   468,   479,   372,
     704,   566,   479,   711,   479,   699,    96,   783,   175,   700,
     165,   701,   493,   752,   784,   583,   493,   494,   660,   180,
     248,   496,   222,   661,   166,   468,   249,   250,   251,   252,
     253,   254,   207,   468,   208,   209,   468,   166,   248,   166,
     246,   450,   212,   213,   249,   250,   251,   252,   253,   254,
     186,   618,   619,   165,    13,    14,   464,    15,   201,   255,
     184,   628,   629,   202,   185,   756,   757,   166,   317,   683,
     684,   656,   318,    16,   203,   198,   847,   255,   166,    17,
     848,   434,    18,    19,    20,   166,    21,  -265,   307,  -265,
     468,   221,   260,   261,   262,   468,   223,    69,   565,   525,
     224,   500,   501,   225,   263,   264,   226,    69,   198,   552,
     510,   227,   511,   166,   328,   329,   152,   153,    69,   267,
     568,   166,   790,   228,   166,   265,   154,    22,   468,   268,
     266,   792,   165,   276,   283,    23,    24,   437,   437,    25,
      69,   796,   570,   308,   155,   156,   166,    69,   315,   572,
      69,   157,   630,    69,   158,   632,   746,   285,   802,   803,
     336,   337,   159,   160,   161,   162,   163,    69,   355,   666,
     219,    81,   408,   709,   710,   761,   340,   691,   166,   692,
     691,   817,   850,   166,    96,   282,   342,    82,    83,    84,
     823,   346,    85,    86,   357,   409,   833,   834,   721,   722,
     358,   366,   725,   726,   368,    87,   165,   165,    88,   374,
     841,   159,   160,   161,   162,   271,   166,   844,   279,   414,
     166,   166,   440,   793,   441,   453,   455,   460,    13,    14,
     852,    15,    89,   854,   463,   507,   482,   491,   492,   497,
      90,   495,   520,    91,    92,   522,   862,    16,   622,   623,
     557,    93,   526,    17,    94,    95,    18,    19,   508,   534,
      21,   544,    96,    81,   437,   545,   437,   549,   550,   551,
     567,   578,   581,   579,   777,   778,   582,   584,   585,    82,
      83,    84,   587,   597,    85,    86,   753,   600,   647,   648,
     617,   620,   621,   651,   652,   626,   636,    87,   565,   649,
      88,    22,   425,   650,   657,   653,   426,   667,   662,    23,
      24,   248,   427,    25,   671,   675,   673,   249,   250,   251,
     252,   253,   254,   425,    89,   676,   680,   720,   702,   703,
     678,   681,   705,   427,   706,    91,   327,   588,    79,   712,
     713,   589,   714,    93,   717,   728,    94,    95,   718,   729,
     255,   738,   428,   736,    96,   744,   429,   742,   758,   745,
     747,   248,   749,   750,   430,   431,   432,   249,   250,   251,
     252,   253,   254,   428,   763,   765,   770,   429,   771,   775,
     772,   323,   779,   780,   782,   430,   431,   432,   794,   776,
     785,   801,   799,   723,   724,   248,   806,   131,   808,   786,
     255,   249,   250,   251,   252,   253,   254,   809,   132,   133,
     816,   818,   813,   838,   821,   825,   134,   240,   135,   826,
     828,   136,   829,   839,   842,   131,   137,   138,   139,   140,
     141,   846,   849,   853,   255,   855,   132,   133,   860,   861,
     858,   142,   864,   365,   134,   866,   135,   330,    96,   136,
     369,   131,   865,   363,   137,   138,   139,   140,   141,   737,
     489,   851,   132,   133,   859,   857,   367,   131,   727,   142,
     134,   573,   135,   338,   634,   136,    96,   528,   132,   133,
     137,   138,   139,   140,   141,   697,   134,   598,   135,   487,
       0,   136,   759,   131,   347,   142,   137,   138,   139,   140,
     141,   755,    96,   513,   132,   133,     0,     0,     0,   131,
       0,   142,   134,     0,   135,   608,     0,   136,    96,     0,
     132,   133,   137,   138,   139,   140,   141,     0,   134,     0,
     135,   610,     0,   136,     0,   131,     0,   142,   137,   138,
     139,   140,   141,     0,    96,     0,   132,   133,     0,     0,
       0,   131,     0,   142,   134,     0,   135,   613,     0,   136,
      96,     0,   132,   133,   137,   138,   139,   140,   141,     0,
     134,     0,   135,     0,     0,   136,     0,   131,     0,   142,
     137,   138,   139,   140,   141,     0,    96,     0,   132,   133,
       0,     0,     0,   131,     0,   142,   134,     0,   435,     0,
       0,   136,    96,     0,   132,   133,   137,   138,   139,   140,
     141,     0,   134,     0,   435,     0,     0,   136,     0,   131,
       0,   142,   137,   138,   139,   140,   553,     0,    96,     0,
       0,   387,     0,     0,     0,   131,     0,   142,   134,     0,
     135,     0,     0,   136,    96,     0,     0,   389,   137,   138,
     139,   140,   141,     0,   134,     0,   135,     0,     0,   136,
       0,   131,     0,   142,   137,   138,   139,   140,   141,     0,
      96,     0,     0,   391,     0,     0,     0,   131,     0,   142,
     134,     0,   135,     0,     0,   136,    96,     0,     0,   393,
     137,   138,   139,   140,   141,     0,   134,     0,   135,     0,
       0,   136,     0,   131,     0,   142,   137,   138,   139,   140,
     141,     0,    96,     0,     0,   395,     0,     0,     0,   131,
       0,   142,   134,     0,   135,     0,     0,   136,    96,     0,
       0,     0,   137,   138,   139,   140,   141,     0,   134,     0,
     135,     0,     0,   136,     0,     0,     0,   142,   137,   138,
     139,   140,   141,     0,    96,     0,   325,     0,     0,   155,
     156,     0,     0,   142,     0,   248,     0,     0,     0,   158,
      96,   249,   250,   251,   252,   253,   254,   159,   160,   161,
     162,   163,   248,     0,     0,     0,     0,     0,   249,   250,
     251,   252,   253,   254,     0,     0,     0,     0,     0,    96,
     248,     0,     0,     0,   255,   326,   249,   250,   251,   252,
     253,   254,     0,   415,   416,     0,     0,     0,     0,   248,
       0,   255,     0,   417,   370,   249,   250,   251,   252,   253,
     254,   159,   160,   161,   162,   418,     0,   248,     0,   255,
       0,     0,   373,   249,   250,   251,   252,   253,   254,     0,
       0,     0,     0,   248,     0,   481,     0,     0,   255,   249,
     250,   251,   252,   253,   254,     0,     0,     0,     0,     0,
       0,   694,   248,     0,     0,     0,   255,  -288,   249,   250,
     251,   252,   253,   254,   -50,   -50,     0,     0,   279,     0,
       0,     0,   255,     0,   -50,   -50,   -50,  -295,  -288,   280,
    -288,  -288,   281,   495,   282,     0,   -50,  -288,  -288,  -288,
    -288,   255,     0,     0,     0,     0,     0,   -50,  -295,   197,
    -295,  -295,     0,   198,     0,     0,     0,  -295,  -295,  -295,
    -295,   375,   376,   377,   378,   379,   380,   381,   382
};

static const yytype_int16 yycheck[] =
{
      15,    61,    48,    18,   154,    48,   310,    74,    37,    66,
      83,    42,   409,    51,    87,    30,   183,    74,   290,   291,
      93,    48,   203,    66,    48,   477,   157,     5,   279,   543,
       6,    74,   321,     5,     5,     5,   325,     5,   131,    85,
     281,   282,    17,   136,     5,     5,     5,     5,    48,     5,
     117,   292,     5,    76,     5,    70,   132,   133,    85,    59,
     117,    76,   260,   261,   262,   263,   264,   265,   266,     5,
       5,    30,    30,     5,   117,     5,   293,   294,   295,   296,
       5,    53,    42,    58,    17,   567,   337,   525,    48,    49,
      50,    51,    52,    53,    65,    65,    65,    28,   155,   156,
      76,   116,   616,    64,    76,   587,    84,    85,    76,    65,
       5,    64,    63,    28,   157,    84,     5,    86,    94,    88,
      89,    81,    58,    58,    60,    94,    58,    60,    58,    60,
     568,   435,   570,    58,   572,    30,    25,   183,   620,   621,
     183,     5,     5,   415,   416,    72,    41,    62,    76,    76,
     196,   197,    16,   196,   197,   336,   183,    54,    55,   183,
      76,   176,     0,    44,    45,   134,   135,    30,    65,   196,
     197,    59,   196,   197,   267,    56,   417,    41,    94,    78,
     256,   257,   258,   259,   315,   667,    38,   584,   439,   387,
     441,   389,   630,   391,   632,   393,    77,   395,    53,    58,
      76,    60,     8,     8,     5,    60,   221,    59,     5,    61,
      62,   226,   227,    64,     8,   230,    68,    69,    70,    71,
      26,    26,   279,   705,    75,    76,    32,    32,    38,    30,
      36,    36,    26,    30,     5,   687,   279,   206,    32,     5,
     209,   210,    36,    94,   213,   214,   215,   216,   217,    59,
      76,    61,    62,   268,   314,    76,    76,    23,    76,    65,
      65,   334,   335,   535,   536,    44,    45,   749,   283,   284,
      76,    76,   315,    94,   671,    93,   322,    56,   409,   530,
     337,   678,    76,    76,   299,    65,   301,    26,   770,   304,
       6,    54,    55,   534,   337,   322,    76,    36,    77,     4,
      72,   590,    65,   453,    76,   455,    11,   474,   597,   706,
     491,   492,   794,    29,   495,    31,   285,   714,    34,   801,
     717,   538,   539,   540,   541,    47,    54,    55,   459,    54,
      55,   369,    54,    55,   363,    26,   367,     8,    66,   354,
      46,   310,    54,    55,    58,    36,   828,    76,   317,   318,
     319,    65,   656,   657,    66,    26,    78,    54,    55,    26,
       5,    32,     8,    76,    10,    36,   409,    13,    65,    36,
     520,   438,   522,   440,   771,    59,    76,    61,    48,   776,
      26,   438,   439,   440,   441,   400,    32,   402,    17,    59,
      36,   155,   156,    39,   525,   438,   439,   440,   441,   298,
      48,   370,   371,   692,    48,    76,   695,    12,    59,   660,
      61,    59,   809,    42,   313,    59,   459,    54,    55,    48,
      49,    50,    51,    52,    53,    38,    63,   442,   474,    76,
     497,   474,    59,    59,   507,   508,    50,   568,    64,   570,
     497,   572,   457,    76,    58,   686,    59,   474,    61,    62,
     474,    64,    81,   584,   497,    68,    69,    70,    71,    50,
     359,   360,    50,   752,   753,    76,   435,    58,    50,   175,
      58,    64,    50,   530,   180,    50,    58,    76,    50,   783,
      58,   785,   525,    58,    50,   552,    58,   530,    26,    27,
      58,    48,    58,    58,    64,   552,    53,    65,    36,   630,
      65,   632,    72,    73,    74,    75,    76,    58,   523,   552,
     799,    58,   481,    60,    65,    58,    54,    55,   224,   225,
     580,    58,    65,    61,   493,   568,    64,   570,    65,   572,
      59,   604,    58,    60,    72,    73,    74,    75,    76,    65,
     671,   584,    80,    76,    58,    17,    76,   678,    58,    63,
     617,   450,    58,   626,    58,    65,    94,    58,    76,    65,
     617,    65,    58,    17,    65,   464,    58,    63,    58,    76,
      42,    63,    59,    63,   617,   706,    48,    49,    50,    51,
      52,    53,    59,   714,    61,    62,   717,   630,    42,   632,
      59,   297,    61,    62,    48,    49,    50,    51,    52,    53,
      76,   500,   501,   660,     7,     8,   312,    10,    76,    81,
      72,   510,   511,    76,    76,   688,   689,   660,    38,   588,
     589,    60,    42,    26,    64,    64,    72,    81,   671,    32,
      76,   646,    35,    36,    37,   678,    39,    59,   653,    61,
     771,    48,    44,    45,    46,   776,    59,    58,   663,    60,
      76,   357,   358,    76,    56,    57,    48,    58,    64,    60,
     366,    48,   368,   706,   196,   197,    26,    27,    58,    79,
      60,   714,   739,    59,   717,    77,    36,    80,   809,    64,
      82,   741,   739,    76,    64,    88,    89,   656,   657,    92,
      58,   751,    60,     5,    54,    55,   739,    58,    60,    60,
      58,    61,    60,    58,    64,    60,   675,    53,   768,   769,
      64,    48,    72,    73,    74,    75,    76,    58,     5,    60,
      80,     3,    28,   622,   623,   694,    76,    58,   771,    60,
      58,   791,    60,   776,    94,    64,    76,    19,    20,    21,
     800,    76,    24,    25,    76,    60,   813,   814,   647,   648,
      76,    76,   651,   652,    76,    37,   813,   814,    40,    76,
     820,    72,    73,    74,    75,    76,   809,   827,    48,    76,
     813,   814,    28,   742,    28,    76,    76,     9,     7,     8,
     840,    10,    64,   843,    76,    14,    91,    64,    64,    53,
      72,    64,    76,    75,    76,    76,   856,    26,   504,   505,
       5,    83,    76,    32,    86,    87,    35,    36,    37,    64,
      39,    76,    94,     3,   783,    76,   785,    76,    76,    76,
       5,     5,     5,    76,   723,   724,     5,    60,     5,    19,
      20,    21,     5,    41,    24,    25,    17,    76,   544,   545,
      53,     5,     5,   549,   550,    15,    28,    37,   863,    76,
      40,    80,    26,    76,    58,    62,    30,     5,    30,    88,
      89,    42,    36,    92,    60,    53,    28,    48,    49,    50,
      51,    52,    53,    26,    64,     5,    76,    30,     5,     5,
      60,     9,     5,    36,    60,    75,    76,    18,     5,    30,
       5,    22,    60,    83,    60,    60,    86,    87,    76,     5,
      81,    76,    76,    28,    94,    76,    80,    53,    30,    76,
       5,    42,     5,     5,    88,    89,    90,    48,    49,    50,
      51,    52,    53,    76,    76,     5,     5,    80,    60,     5,
      59,    28,     5,     5,    76,    88,    89,    90,     5,    60,
      60,     5,    17,   649,   650,    42,     5,    43,     5,    63,
      81,    48,    49,    50,    51,    52,    53,    60,    54,    55,
      76,     5,    28,    28,     5,     5,    62,    63,    64,     5,
       5,    67,     5,     5,     5,    43,    72,    73,    74,    75,
      76,     5,     5,     5,    81,     5,    54,    55,     5,     5,
      64,    87,     5,   231,    62,    65,    64,    65,    94,    67,
     235,    43,   863,   229,    72,    73,    74,    75,    76,   663,
     337,   838,    54,    55,   851,   849,   233,    43,   653,    87,
      62,   457,    64,    65,   523,    67,    94,   410,    54,    55,
      72,    73,    74,    75,    76,   602,    62,   479,    64,    65,
      -1,    67,   691,    43,   213,    87,    72,    73,    74,    75,
      76,   687,    94,   371,    54,    55,    -1,    -1,    -1,    43,
      -1,    87,    62,    -1,    64,    65,    -1,    67,    94,    -1,
      54,    55,    72,    73,    74,    75,    76,    -1,    62,    -1,
      64,    65,    -1,    67,    -1,    43,    -1,    87,    72,    73,
      74,    75,    76,    -1,    94,    -1,    54,    55,    -1,    -1,
      -1,    43,    -1,    87,    62,    -1,    64,    65,    -1,    67,
      94,    -1,    54,    55,    72,    73,    74,    75,    76,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    43,    -1,    87,
      72,    73,    74,    75,    76,    -1,    94,    -1,    54,    55,
      -1,    -1,    -1,    43,    -1,    87,    62,    -1,    64,    -1,
      -1,    67,    94,    -1,    54,    55,    72,    73,    74,    75,
      76,    -1,    62,    -1,    64,    -1,    -1,    67,    -1,    43,
      -1,    87,    72,    73,    74,    75,    76,    -1,    94,    -1,
      -1,    55,    -1,    -1,    -1,    43,    -1,    87,    62,    -1,
      64,    -1,    -1,    67,    94,    -1,    -1,    55,    72,    73,
      74,    75,    76,    -1,    62,    -1,    64,    -1,    -1,    67,
      -1,    43,    -1,    87,    72,    73,    74,    75,    76,    -1,
      94,    -1,    -1,    55,    -1,    -1,    -1,    43,    -1,    87,
      62,    -1,    64,    -1,    -1,    67,    94,    -1,    -1,    55,
      72,    73,    74,    75,    76,    -1,    62,    -1,    64,    -1,
      -1,    67,    -1,    43,    -1,    87,    72,    73,    74,    75,
      76,    -1,    94,    -1,    -1,    55,    -1,    -1,    -1,    43,
      -1,    87,    62,    -1,    64,    -1,    -1,    67,    94,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    62,    -1,
      64,    -1,    -1,    67,    -1,    -1,    -1,    87,    72,    73,
      74,    75,    76,    -1,    94,    -1,    33,    -1,    -1,    54,
      55,    -1,    -1,    87,    -1,    42,    -1,    -1,    -1,    64,
      94,    48,    49,    50,    51,    52,    53,    72,    73,    74,
      75,    76,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    94,
      42,    -1,    -1,    -1,    81,    65,    48,    49,    50,    51,
      52,    53,    -1,    54,    55,    -1,    -1,    -1,    -1,    42,
      -1,    81,    -1,    64,    66,    48,    49,    50,    51,    52,
      53,    72,    73,    74,    75,    76,    -1,    42,    -1,    81,
      -1,    -1,    65,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    42,    -1,    60,    -1,    -1,    81,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    42,    -1,    -1,    -1,    81,    38,    48,    49,
      50,    51,    52,    53,    44,    45,    -1,    -1,    48,    -1,
      -1,    -1,    81,    -1,    54,    55,    56,    38,    59,    59,
      61,    62,    62,    64,    64,    -1,    66,    68,    69,    70,
      71,    81,    -1,    -1,    -1,    -1,    -1,    77,    59,    60,
      61,    62,    -1,    64,    -1,    -1,    -1,    68,    69,    70,
      71,   248,   249,   250,   251,   252,   253,   254,   255
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    96,    97,   100,   101,   103,    76,    98,
      76,     0,    59,     7,     8,    10,    26,    32,    35,    36,
      37,    39,    80,    88,    89,    92,   102,   147,   173,    59,
      64,    99,     5,    72,    76,   108,    76,   109,   110,    76,
     172,    76,   145,   146,   172,   172,    76,   174,   176,    76,
      93,   115,   116,    26,    36,    76,    76,    26,    36,    76,
     172,     5,    12,     5,    58,    53,    60,   110,     5,    58,
      48,    59,   161,   146,    60,     5,    48,    59,   161,     5,
      30,     3,    19,    20,    21,    24,    25,    37,    40,    64,
      72,    75,    76,    83,    86,    87,    94,   117,   179,   180,
     181,   184,   185,   186,   187,   188,   191,   192,   194,   195,
     196,   197,   204,   205,   206,   207,    48,    53,    76,   116,
      76,    76,    59,    59,    76,    76,    65,   100,   104,    72,
      76,    43,    54,    55,    62,    64,    67,    72,    73,    74,
      75,    76,    87,   117,   185,   188,   192,   209,   210,   211,
     212,   213,    26,    27,    36,    54,    55,    61,    64,    72,
      73,    74,    75,    76,    80,   114,   117,   118,   120,   129,
     130,   131,   132,    76,   172,    76,    64,   162,   120,   172,
      76,     5,   162,   175,    72,    76,    76,   174,   209,   188,
     192,   208,   209,   174,   209,   209,    60,    60,    64,   174,
     209,    76,    76,    64,    59,    61,    38,    59,    61,    62,
      38,    59,    61,    62,    68,    69,    70,    71,   172,    80,
     120,    48,    59,    59,    76,    76,    48,    48,    59,     8,
      10,    13,    26,    32,    36,    39,   105,   213,   211,   211,
      63,   209,   214,   215,   209,   213,    59,     5,    42,    48,
      49,    50,    51,    52,    53,    81,    47,    54,    55,    78,
      44,    45,    46,    56,    57,    77,    82,    79,    64,   167,
     167,    76,   132,   132,    76,   118,    76,   170,   171,    48,
      59,    62,    64,    64,   125,    53,     6,    29,    31,    34,
      54,    55,    66,    44,    45,    56,    77,    50,   161,     8,
      26,    32,    36,    65,    76,   163,   164,   172,     5,     5,
      53,    76,    50,   161,   150,    60,   179,    38,    42,    23,
      17,    17,    58,    28,    30,    33,    65,    76,   180,   180,
      65,   189,   190,   209,    84,    85,    64,    48,    65,   189,
      76,   209,    76,   193,   209,   209,    76,   193,   209,   209,
     209,   209,    50,     5,    64,     5,   172,    76,    76,   161,
     161,   172,   172,   109,   172,   101,    76,   145,    76,   115,
      66,    58,    63,    65,    76,   210,   210,   210,   210,   210,
     210,   210,   210,   211,   211,   211,   211,    55,   212,    55,
     212,    55,   212,    55,   212,    55,   212,   212,   212,   213,
       8,    26,    32,    36,    65,   168,   169,   172,    28,    60,
      58,    65,   119,   130,    76,    54,    55,    64,    76,   114,
     133,   134,   135,   133,   172,    26,    30,    36,    76,    80,
      88,    89,    90,   126,   172,    64,   111,   209,    28,    62,
      28,    28,   123,   134,   134,   133,   135,   135,   135,   135,
     161,   162,   172,    76,   172,    76,   172,     5,    65,    60,
       9,   166,   111,    76,   161,   162,   100,    76,   118,   124,
     209,   209,   209,   177,   178,   177,   192,   198,   177,    58,
      65,    60,    91,   174,   182,   183,   174,    65,   189,   119,
      65,    64,    64,    58,    63,    64,    63,    53,   172,    50,
     161,   161,   162,   162,    50,    50,     5,    14,    37,   106,
     161,   161,   209,   215,   212,   212,   212,   212,   212,   172,
      76,   172,    76,     5,    65,    60,    76,   124,   171,    50,
      58,   134,   134,   133,    64,    54,    55,    63,    44,    45,
      56,    77,    65,    65,    76,    76,     5,    26,    36,    76,
      76,    76,    60,    76,   111,   113,   209,     5,   120,   130,
     120,   130,   136,   137,   138,   172,   162,     5,    60,   167,
      60,   167,    60,   164,     6,    76,   118,   165,     5,    76,
     148,     5,     5,   162,    60,     5,    59,     5,    18,    22,
      17,   179,   133,   200,   201,   202,   203,    41,   190,   209,
      76,    30,     5,    30,    41,    30,    65,    50,    65,   189,
      65,   189,   209,    65,   189,   120,    65,    53,   162,   162,
       5,     5,   161,   161,   174,   174,    15,   107,   162,   162,
      60,   167,    60,   167,   169,   165,    28,   130,    65,   133,
     134,   134,   135,   135,   135,   135,   125,   161,   161,    76,
      76,   161,   161,    62,   127,   120,    60,    58,     5,    65,
      58,    63,    30,     5,    25,   142,    60,     5,   166,   165,
     165,    60,   165,    28,    59,    53,     5,   100,    60,   124,
      76,     9,   166,   209,   209,   177,    66,     5,    16,    41,
     199,    58,    60,   177,    60,    17,    60,   183,   174,    65,
      65,    65,     5,     5,   120,     5,    60,   166,   166,   162,
     162,   174,    30,     5,    60,   165,   165,    60,    76,    65,
      30,   162,   162,   161,   161,   162,   162,   163,    60,     5,
     111,   111,   112,    65,    76,   130,    28,   138,    76,   139,
     166,   155,    53,   124,    76,    76,   209,     5,   124,     5,
       5,   149,    17,    17,   133,   201,   174,   174,    30,   203,
     177,   209,   177,    76,    30,     5,   166,   124,   157,   158,
       5,    60,    59,   124,   124,     5,    60,   162,   162,     5,
       5,    63,    76,    58,    65,    60,    63,   121,    28,    60,
     120,   153,   100,   209,     5,   166,   100,   177,   177,    17,
     159,     5,   100,   100,   166,   124,     5,   124,     5,    60,
     128,   111,   111,    28,   122,   141,    76,   100,     5,   166,
     156,     5,   177,   100,   166,     5,     5,   151,     5,     5,
     124,     5,    76,   120,   120,   143,   144,   202,    28,     5,
     154,   100,     5,   160,   100,   166,     5,    72,    76,     5,
      60,   141,   100,     5,   100,     5,   152,   144,    64,   143,
       5,     5,   100,   140,     5,   136,    65
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
     116,   116,   117,   118,   118,   119,   119,   120,   120,   120,
     121,   122,   120,   120,   120,   120,   120,   120,   120,   123,
     120,   120,   120,   120,   120,   120,   124,   124,   124,   125,
     125,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   127,   127,   128,   128,   128,   129,   129,   130,   130,
     130,   130,   130,   130,   131,   131,   131,   131,   131,   132,
     132,   132,   132,   132,   132,   133,   133,   133,   133,   133,
     134,   134,   134,   134,   134,   135,   135,   135,   136,   137,
     137,   139,   138,   138,   140,   141,   142,   142,   142,   143,
     143,   144,   144,   145,   145,   146,   146,   146,   147,   147,
     148,   147,   149,   147,   150,   147,   151,   147,   152,   147,
     153,   147,   154,   147,   155,   147,   156,   147,   157,   147,
     158,   147,   159,   147,   160,   147,   161,   162,   162,   162,
     163,   163,   164,   164,   164,   164,   164,   164,   164,   164,
     165,   165,   165,   165,   166,   166,   167,   167,   167,   168,
     168,   169,   169,   169,   169,   169,   170,   170,   171,   172,
     172,   173,   175,   174,   176,   174,   178,   177,   179,   179,
     179,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   181,   181,   181,   181,
     182,   182,   182,   183,   183,   184,   184,   185,   185,   185,
     186,   186,   186,   186,   187,   187,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   189,   190,   190,
     190,   191,   191,   191,   191,   191,   191,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   193,   193,   194,   195,
     195,   196,   196,   198,   197,   199,   199,   199,   200,   200,
     201,   201,   202,   202,   203,   203,   204,   205,   206,   206,
     206,   207,   208,   208,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   210,   210,   210,   210,   210,   210,   210,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   212,   212,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   214,   214,   215,
     215
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
       4,     7,     5,     1,     4,     1,     3,     1,     2,     2,
       0,     0,     9,     4,     2,     4,     4,     6,     9,     0,
       5,     4,     2,     4,     3,     6,     1,     3,     1,     0,
       2,     1,     2,     4,     5,     7,     5,     5,     6,     8,
       7,     0,     3,     0,     3,     3,     1,     0,     3,     1,
       4,     3,     1,     3,     1,     2,     2,     3,     3,     1,
       4,     3,     3,     3,     3,     1,     2,     2,     3,     3,
       1,     3,     3,     3,     3,     1,     4,     3,     2,     3,
       1,     0,     4,     0,     0,     0,     7,     5,     0,     3,
       1,     6,     0,     2,     1,     4,     6,     6,     7,     9,
       0,     9,     0,    11,     0,     7,     0,    13,     0,    15,
       0,    12,     0,    14,     0,    11,     0,    13,     0,    11,
       0,    11,     0,    12,     0,    14,     0,     3,     2,     0,
       3,     1,     3,     5,     4,     3,     4,     5,     6,     4,
       1,     3,     3,     1,     0,     3,     3,     2,     0,     3,
       1,     3,     4,     4,     3,     5,     3,     1,     1,     3,
       1,     3,     0,     4,     0,     2,     0,     2,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     5,     5,     7,     5,
       1,     3,     2,     6,     4,     1,     2,     2,     4,     5,
       1,     1,     3,     1,     1,     3,     4,     4,     3,     3,
       6,     5,     6,     5,     6,     5,     3,     1,     1,     3,
       5,     3,     3,     3,     3,     3,     3,     4,     3,     2,
       3,     4,     2,     3,     2,     1,     3,     1,     3,     2,
       2,     4,     6,     0,     7,     0,     2,     2,     3,     1,
       3,     0,     3,     1,     1,     3,     4,     4,     8,     8,
       6,     4,     3,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     2,     2,     3,     3,     3,     3,
       1,     3,     3,     3,     3,     3,     4,     4,     4,     4,
       4,     3,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     3,     1,     2,     2,     2,     3,     1,     3,     1,
       3
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
#line 2161 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 217 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2167 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 220 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2173 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 221 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2179 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 224 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2185 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 225 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2191 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 228 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2197 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 229 "pascal.y"
     { (yyval.node) = NULL; }
#line 2203 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 232 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2209 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 235 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2215 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 236 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2221 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 239 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2227 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 240 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2233 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 241 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2239 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 242 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2245 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 243 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2251 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 244 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2257 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 245 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2263 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 248 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2269 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 251 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2275 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 252 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2281 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY id_list SEMICOLON  */
#line 255 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2287 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 256 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2293 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 257 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2299 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 258 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2305 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 259 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2311 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 260 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2317 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 263 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2323 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 264 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2329 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 265 "pascal.y"
     { (yyval.node) = NULL; }
#line 2335 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 268 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2341 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 269 "pascal.y"
     { (yyval.node) = NULL; }
#line 2347 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 272 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2353 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 273 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2359 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 274 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2365 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 275 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2371 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 278 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2377 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 279 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2383 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 282 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2389 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 283 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2395 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 286 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2401 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 287 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2407 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 288 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2413 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 289 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2419 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 292 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 293 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2431 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 296 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2437 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 297 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2443 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 300 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2449 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 301 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2455 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 302 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2461 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 303 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2467 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 304 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2473 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 307 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2479 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 308 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2485 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 311 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 312 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2497 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 313 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2503 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 314 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2509 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 315 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2515 "pascal.tab.c"
    break;

  case 62: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 318 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2521 "pascal.tab.c"
    break;

  case 63: /* spec_ref: spec_kw  */
#line 321 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2527 "pascal.tab.c"
    break;

  case 64: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 322 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2533 "pascal.tab.c"
    break;

  case 65: /* type_arg_list: simple_type  */
#line 325 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2539 "pascal.tab.c"
    break;

  case 66: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 326 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 67: /* type: simple_type  */
#line 329 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2551 "pascal.tab.c"
    break;

  case 68: /* type: ARROW IDENT  */
#line 330 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2557 "pascal.tab.c"
    break;

  case 69: /* type: ARROW spec_ref  */
#line 331 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2563 "pascal.tab.c"
    break;

  case 70: /* $@1: %empty  */
#line 332 "pascal.y"
                                                       { }
#line 2569 "pascal.tab.c"
    break;

  case 71: /* $@2: %empty  */
#line 332 "pascal.y"
                                                           { }
#line 2575 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 332 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2581 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt ARRAYSY OFSY type  */
#line 333 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2587 "pascal.tab.c"
    break;

  case 74: /* type: PROCEDURESY pf_params  */
#line 334 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2593 "pascal.tab.c"
    break;

  case 75: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 335 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2599 "pascal.tab.c"
    break;

  case 76: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 336 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2605 "pascal.tab.c"
    break;

  case 77: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 337 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2611 "pascal.tab.c"
    break;

  case 78: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 338 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2617 "pascal.tab.c"
    break;

  case 79: /* $@3: %empty  */
#line 339 "pascal.y"
                         { }
#line 2623 "pascal.tab.c"
    break;

  case 80: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 339 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2629 "pascal.tab.c"
    break;

  case 81: /* type: packed_opt SETSY OFSY simple_type  */
#line 340 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2635 "pascal.tab.c"
    break;

  case 82: /* type: packed_opt FILESY  */
#line 341 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2641 "pascal.tab.c"
    break;

  case 83: /* type: packed_opt FILESY OFSY type  */
#line 342 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2647 "pascal.tab.c"
    break;

  case 84: /* type: CLASSSY class_members ENDSY  */
#line 343 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2653 "pascal.tab.c"
    break;

  case 85: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 344 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2659 "pascal.tab.c"
    break;

  case 86: /* result_type: IDENT  */
#line 347 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2665 "pascal.tab.c"
    break;

  case 87: /* result_type: IDENT PERIOD IDENT  */
#line 348 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2671 "pascal.tab.c"
    break;

  case 88: /* result_type: spec_ref  */
#line 349 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2677 "pascal.tab.c"
    break;

  case 89: /* class_members: %empty  */
#line 352 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2683 "pascal.tab.c"
    break;

  case 90: /* class_members: class_members class_member  */
#line 353 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2689 "pascal.tab.c"
    break;

  case 91: /* class_member: IDENT  */
#line 356 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2695 "pascal.tab.c"
    break;

  case 92: /* class_member: IDENT SEMICOLON  */
#line 357 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2701 "pascal.tab.c"
    break;

  case 93: /* class_member: id_list COLON type SEMICOLON  */
#line 358 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2707 "pascal.tab.c"
    break;

  case 94: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 359 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2713 "pascal.tab.c"
    break;

  case 95: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 360 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2719 "pascal.tab.c"
    break;

  case 96: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 361 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2725 "pascal.tab.c"
    break;

  case 97: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 362 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2731 "pascal.tab.c"
    break;

  case 98: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 363 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2737 "pascal.tab.c"
    break;

  case 99: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 364 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2743 "pascal.tab.c"
    break;

  case 100: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 365 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2749 "pascal.tab.c"
    break;

  case 101: /* prop_index_opt: %empty  */
#line 368 "pascal.y"
    { (yyval.node) = NULL; }
#line 2755 "pascal.tab.c"
    break;

  case 102: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 369 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2761 "pascal.tab.c"
    break;

  case 103: /* prop_specs: %empty  */
#line 372 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2767 "pascal.tab.c"
    break;

  case 104: /* prop_specs: prop_specs IDENT IDENT  */
#line 373 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2773 "pascal.tab.c"
    break;

  case 105: /* prop_specs: prop_specs IDENT INTCONST  */
#line 374 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2779 "pascal.tab.c"
    break;

  case 106: /* packed_opt: PACKEDSY  */
#line 377 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2785 "pascal.tab.c"
    break;

  case 107: /* packed_opt: %empty  */
#line 378 "pascal.y"
     { (yyval.node) = NULL; }
#line 2791 "pascal.tab.c"
    break;

  case 108: /* simple_type: LPARENT enum_list RPARENT  */
#line 381 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2797 "pascal.tab.c"
    break;

  case 109: /* simple_type: IDENT  */
#line 382 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2803 "pascal.tab.c"
    break;

  case 110: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 383 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2809 "pascal.tab.c"
    break;

  case 111: /* simple_type: IDENT PERIOD IDENT  */
#line 384 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2815 "pascal.tab.c"
    break;

  case 112: /* simple_type: spec_ref  */
#line 385 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2821 "pascal.tab.c"
    break;

  case 113: /* simple_type: lo_bound DOTDOT b_expr  */
#line 386 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2827 "pascal.tab.c"
    break;

  case 114: /* lo_bound: lo_term  */
#line 389 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2833 "pascal.tab.c"
    break;

  case 115: /* lo_bound: PLUS lo_term  */
#line 390 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2839 "pascal.tab.c"
    break;

  case 116: /* lo_bound: MINUS lo_term  */
#line 391 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2845 "pascal.tab.c"
    break;

  case 117: /* lo_bound: lo_bound PLUS b_term  */
#line 392 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2851 "pascal.tab.c"
    break;

  case 118: /* lo_bound: lo_bound MINUS b_term  */
#line 393 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2857 "pascal.tab.c"
    break;

  case 119: /* lo_term: scalar_constant  */
#line 396 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2863 "pascal.tab.c"
    break;

  case 120: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 397 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2869 "pascal.tab.c"
    break;

  case 121: /* lo_term: lo_term MUL b_factor  */
#line 398 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2875 "pascal.tab.c"
    break;

  case 122: /* lo_term: lo_term IDIV b_factor  */
#line 399 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2881 "pascal.tab.c"
    break;

  case 123: /* lo_term: lo_term IMOD b_factor  */
#line 400 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2887 "pascal.tab.c"
    break;

  case 124: /* lo_term: lo_term SHIFTOP b_factor  */
#line 401 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2893 "pascal.tab.c"
    break;

  case 125: /* b_expr: b_term  */
#line 404 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2899 "pascal.tab.c"
    break;

  case 126: /* b_expr: PLUS b_term  */
#line 405 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2905 "pascal.tab.c"
    break;

  case 127: /* b_expr: MINUS b_term  */
#line 406 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2911 "pascal.tab.c"
    break;

  case 128: /* b_expr: b_expr PLUS b_term  */
#line 407 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2917 "pascal.tab.c"
    break;

  case 129: /* b_expr: b_expr MINUS b_term  */
#line 408 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2923 "pascal.tab.c"
    break;

  case 130: /* b_term: b_factor  */
#line 411 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2929 "pascal.tab.c"
    break;

  case 131: /* b_term: b_term MUL b_factor  */
#line 412 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2935 "pascal.tab.c"
    break;

  case 132: /* b_term: b_term IDIV b_factor  */
#line 413 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2941 "pascal.tab.c"
    break;

  case 133: /* b_term: b_term IMOD b_factor  */
#line 414 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2947 "pascal.tab.c"
    break;

  case 134: /* b_term: b_term SHIFTOP b_factor  */
#line 415 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2953 "pascal.tab.c"
    break;

  case 135: /* b_factor: scalar_constant  */
#line 418 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2959 "pascal.tab.c"
    break;

  case 136: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 419 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2965 "pascal.tab.c"
    break;

  case 137: /* b_factor: LPARENT b_expr RPARENT  */
#line 420 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2971 "pascal.tab.c"
    break;

  case 138: /* record_body: record_field_list record_case_opt  */
#line 423 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2977 "pascal.tab.c"
    break;

  case 139: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 426 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2983 "pascal.tab.c"
    break;

  case 140: /* record_field_list: record_field  */
#line 427 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2989 "pascal.tab.c"
    break;

  case 141: /* $@4: %empty  */
#line 430 "pascal.y"
                  { }
#line 2995 "pascal.tab.c"
    break;

  case 142: /* record_field: id_list COLON $@4 type  */
#line 430 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3001 "pascal.tab.c"
    break;

  case 143: /* record_field: %empty  */
#line 431 "pascal.y"
     { (yyval.node) = NULL; }
#line 3007 "pascal.tab.c"
    break;

  case 144: /* case_arm_mark: %empty  */
#line 434 "pascal.y"
    { }
#line 3013 "pascal.tab.c"
    break;

  case 145: /* case_open: %empty  */
#line 437 "pascal.y"
    { }
#line 3019 "pascal.tab.c"
    break;

  case 146: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 440 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3025 "pascal.tab.c"
    break;

  case 147: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 441 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3031 "pascal.tab.c"
    break;

  case 148: /* record_case_opt: %empty  */
#line 442 "pascal.y"
     { (yyval.node) = NULL; }
#line 3037 "pascal.tab.c"
    break;

  case 149: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 445 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3043 "pascal.tab.c"
    break;

  case 150: /* record_case_list: record_case_arm  */
#line 446 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3049 "pascal.tab.c"
    break;

  case 151: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 449 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3055 "pascal.tab.c"
    break;

  case 152: /* record_case_arm: %empty  */
#line 450 "pascal.y"
     { (yyval.node) = NULL; }
#line 3061 "pascal.tab.c"
    break;

  case 153: /* var_decl_list: var_decl_list var_decl  */
#line 453 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3067 "pascal.tab.c"
    break;

  case 154: /* var_decl_list: var_decl  */
#line 454 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3073 "pascal.tab.c"
    break;

  case 155: /* var_decl: id_list COLON type SEMICOLON  */
#line 457 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3079 "pascal.tab.c"
    break;

  case 156: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 458 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3085 "pascal.tab.c"
    break;

  case 157: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 459 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3091 "pascal.tab.c"
    break;

  case 158: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 462 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3097 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 463 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3103 "pascal.tab.c"
    break;

  case 160: /* $@5: %empty  */
#line 464 "pascal.y"
                                                                               { }
#line 3109 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 464 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3115 "pascal.tab.c"
    break;

  case 162: /* $@6: %empty  */
#line 465 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3121 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 465 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 164: /* $@7: %empty  */
#line 466 "pascal.y"
                                        { }
#line 3133 "pascal.tab.c"
    break;

  case 165: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 466 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3139 "pascal.tab.c"
    break;

  case 166: /* $@8: %empty  */
#line 467 "pascal.y"
                                                                                                           { }
#line 3145 "pascal.tab.c"
    break;

  case 167: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 467 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3151 "pascal.tab.c"
    break;

  case 168: /* $@9: %empty  */
#line 468 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3157 "pascal.tab.c"
    break;

  case 169: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 468 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3163 "pascal.tab.c"
    break;

  case 170: /* $@10: %empty  */
#line 469 "pascal.y"
                                                                                                 { }
#line 3169 "pascal.tab.c"
    break;

  case 171: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 469 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3175 "pascal.tab.c"
    break;

  case 172: /* $@11: %empty  */
#line 470 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3181 "pascal.tab.c"
    break;

  case 173: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 470 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3187 "pascal.tab.c"
    break;

  case 174: /* $@12: %empty  */
#line 471 "pascal.y"
                                                                                            { }
#line 3193 "pascal.tab.c"
    break;

  case 175: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 471 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3199 "pascal.tab.c"
    break;

  case 176: /* $@13: %empty  */
#line 472 "pascal.y"
                                                                                                             { }
#line 3205 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 472 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3213 "pascal.tab.c"
    break;

  case 178: /* $@14: %empty  */
#line 475 "pascal.y"
                                                                                              { }
#line 3219 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 475 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3227 "pascal.tab.c"
    break;

  case 180: /* $@15: %empty  */
#line 478 "pascal.y"
                                                                                             { }
#line 3233 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 478 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3241 "pascal.tab.c"
    break;

  case 182: /* $@16: %empty  */
#line 481 "pascal.y"
                                                                                                    { }
#line 3247 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 481 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3255 "pascal.tab.c"
    break;

  case 184: /* $@17: %empty  */
#line 484 "pascal.y"
                                                                                                                     { }
#line 3261 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 484 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3269 "pascal.tab.c"
    break;

  case 186: /* pv_mark: %empty  */
#line 489 "pascal.y"
    { }
#line 3275 "pascal.tab.c"
    break;

  case 187: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 492 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3281 "pascal.tab.c"
    break;

  case 188: /* parameter_list_opt: LPARENT RPARENT  */
#line 493 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3287 "pascal.tab.c"
    break;

  case 189: /* parameter_list_opt: %empty  */
#line 494 "pascal.y"
     { (yyval.node) = NULL; }
#line 3293 "pascal.tab.c"
    break;

  case 190: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 497 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3299 "pascal.tab.c"
    break;

  case 191: /* parameter_decl_list: parameter_decl  */
#line 498 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3305 "pascal.tab.c"
    break;

  case 192: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 501 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3311 "pascal.tab.c"
    break;

  case 193: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 502 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3317 "pascal.tab.c"
    break;

  case 194: /* parameter_decl: VARSY id_list COLON param_type  */
#line 503 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3323 "pascal.tab.c"
    break;

  case 195: /* parameter_decl: id_list COLON param_type  */
#line 504 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3329 "pascal.tab.c"
    break;

  case 196: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 505 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3335 "pascal.tab.c"
    break;

  case 197: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 506 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3341 "pascal.tab.c"
    break;

  case 198: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 507 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3347 "pascal.tab.c"
    break;

  case 199: /* parameter_decl: IDENT id_list COLON param_type  */
#line 508 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3353 "pascal.tab.c"
    break;

  case 200: /* param_type: IDENT  */
#line 511 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3359 "pascal.tab.c"
    break;

  case 201: /* param_type: IDENT PERIOD IDENT  */
#line 512 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 202: /* param_type: ARRAYSY OFSY IDENT  */
#line 513 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3371 "pascal.tab.c"
    break;

  case 203: /* param_type: spec_ref  */
#line 514 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3377 "pascal.tab.c"
    break;

  case 204: /* routine_directives: %empty  */
#line 517 "pascal.y"
    { (yyval.node) = NULL; }
#line 3383 "pascal.tab.c"
    break;

  case 205: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 518 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3396 "pascal.tab.c"
    break;

  case 206: /* pf_params: LPARENT pf_sections RPARENT  */
#line 528 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3402 "pascal.tab.c"
    break;

  case 207: /* pf_params: LPARENT RPARENT  */
#line 529 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3408 "pascal.tab.c"
    break;

  case 208: /* pf_params: %empty  */
#line 530 "pascal.y"
     { (yyval.node) = NULL; }
#line 3414 "pascal.tab.c"
    break;

  case 209: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 533 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3420 "pascal.tab.c"
    break;

  case 210: /* pf_sections: pf_section  */
#line 534 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3426 "pascal.tab.c"
    break;

  case 211: /* pf_section: id_list COLON param_type  */
#line 537 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3432 "pascal.tab.c"
    break;

  case 212: /* pf_section: VARSY id_list COLON param_type  */
#line 538 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3438 "pascal.tab.c"
    break;

  case 213: /* pf_section: CONSTSY id_list COLON param_type  */
#line 539 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3444 "pascal.tab.c"
    break;

  case 214: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 540 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3450 "pascal.tab.c"
    break;

  case 215: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 541 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3456 "pascal.tab.c"
    break;

  case 216: /* enum_list: enum_list COMMA enum_member  */
#line 544 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3462 "pascal.tab.c"
    break;

  case 217: /* enum_list: enum_member  */
#line 545 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3468 "pascal.tab.c"
    break;

  case 218: /* enum_member: IDENT  */
#line 548 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3474 "pascal.tab.c"
    break;

  case 219: /* id_list: id_list COMMA IDENT  */
#line 551 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3480 "pascal.tab.c"
    break;

  case 220: /* id_list: IDENT  */
#line 552 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3486 "pascal.tab.c"
    break;

  case 221: /* body: BEGINSY statement_list ENDSY  */
#line 555 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3492 "pascal.tab.c"
    break;

  case 222: /* $@18: %empty  */
#line 558 "pascal.y"
                             { pas_stmt_mark(); }
#line 3498 "pascal.tab.c"
    break;

  case 223: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 558 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3504 "pascal.tab.c"
    break;

  case 224: /* $@19: %empty  */
#line 559 "pascal.y"
     { pas_stmt_mark(); }
#line 3510 "pascal.tab.c"
    break;

  case 225: /* statement_list: $@19 statement  */
#line 559 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3516 "pascal.tab.c"
    break;

  case 226: /* $@20: %empty  */
#line 562 "pascal.y"
    { pas_stmt_mark(); }
#line 3522 "pascal.tab.c"
    break;

  case 227: /* body_stmt: $@20 statement  */
#line 562 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3528 "pascal.tab.c"
    break;

  case 228: /* statement: statement_no_label  */
#line 565 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3534 "pascal.tab.c"
    break;

  case 229: /* statement: INTCONST COLON statement_no_label  */
#line 566 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3540 "pascal.tab.c"
    break;

  case 230: /* statement: IDENT COLON statement_no_label  */
#line 567 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3546 "pascal.tab.c"
    break;

  case 231: /* statement_no_label: assignment  */
#line 570 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3552 "pascal.tab.c"
    break;

  case 232: /* statement_no_label: call  */
#line 571 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3558 "pascal.tab.c"
    break;

  case 233: /* statement_no_label: compound_statement  */
#line 572 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3564 "pascal.tab.c"
    break;

  case 234: /* statement_no_label: goto_statement  */
#line 573 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3570 "pascal.tab.c"
    break;

  case 235: /* statement_no_label: if_statement  */
#line 574 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3576 "pascal.tab.c"
    break;

  case 236: /* statement_no_label: case_statement  */
#line 575 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3582 "pascal.tab.c"
    break;

  case 237: /* statement_no_label: while_statement  */
#line 576 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3588 "pascal.tab.c"
    break;

  case 238: /* statement_no_label: repeat_statement  */
#line 577 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3594 "pascal.tab.c"
    break;

  case 239: /* statement_no_label: for_statement  */
#line 578 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3600 "pascal.tab.c"
    break;

  case 240: /* statement_no_label: with_statement  */
#line 579 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3606 "pascal.tab.c"
    break;

  case 241: /* statement_no_label: try_statement  */
#line 580 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3612 "pascal.tab.c"
    break;

  case 242: /* statement_no_label: raise_statement  */
#line 581 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3618 "pascal.tab.c"
    break;

  case 243: /* statement_no_label: inherited_call  */
#line 582 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3624 "pascal.tab.c"
    break;

  case 244: /* statement_no_label: INHERITEDSY  */
#line 583 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3630 "pascal.tab.c"
    break;

  case 245: /* statement_no_label: %empty  */
#line 584 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3636 "pascal.tab.c"
    break;

  case 246: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 587 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3642 "pascal.tab.c"
    break;

  case 247: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 588 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3648 "pascal.tab.c"
    break;

  case 248: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 589 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3654 "pascal.tab.c"
    break;

  case 249: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 590 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3660 "pascal.tab.c"
    break;

  case 250: /* handler_list: handler  */
#line 593 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3666 "pascal.tab.c"
    break;

  case 251: /* handler_list: handler_list SEMICOLON handler  */
#line 594 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3672 "pascal.tab.c"
    break;

  case 252: /* handler_list: handler_list SEMICOLON  */
#line 595 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3678 "pascal.tab.c"
    break;

  case 253: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 598 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3684 "pascal.tab.c"
    break;

  case 254: /* handler: ONSY IDENT DOSY body_stmt  */
#line 599 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3690 "pascal.tab.c"
    break;

  case 255: /* raise_statement: RAISESY  */
#line 602 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3696 "pascal.tab.c"
    break;

  case 256: /* raise_statement: RAISESY expression  */
#line 603 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3702 "pascal.tab.c"
    break;

  case 257: /* inherited_call: INHERITEDSY IDENT  */
#line 606 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3708 "pascal.tab.c"
    break;

  case 258: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 607 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3714 "pascal.tab.c"
    break;

  case 259: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 608 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3720 "pascal.tab.c"
    break;

  case 260: /* call: IDENT  */
#line 611 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3726 "pascal.tab.c"
    break;

  case 261: /* call: spec_kw  */
#line 612 "pascal.y"
             { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3732 "pascal.tab.c"
    break;

  case 262: /* call: selector PERIOD IDENT  */
#line 613 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3738 "pascal.tab.c"
    break;

  case 263: /* call: call_with_args  */
#line 614 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3744 "pascal.tab.c"
    break;

  case 264: /* recv_primary: STRINGCONST  */
#line 617 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3750 "pascal.tab.c"
    break;

  case 265: /* recv_primary: LPARENT expression RPARENT  */
#line 618 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3756 "pascal.tab.c"
    break;

  case 266: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 621 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3762 "pascal.tab.c"
    break;

  case 267: /* call_with_args: spec_kw LPARENT argument_list RPARENT  */
#line 622 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3768 "pascal.tab.c"
    break;

  case 268: /* call_with_args: spec_kw LPARENT RPARENT  */
#line 623 "pascal.y"
                             { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3774 "pascal.tab.c"
    break;

  case 269: /* call_with_args: IDENT LPARENT RPARENT  */
#line 624 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3780 "pascal.tab.c"
    break;

  case 270: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 625 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3786 "pascal.tab.c"
    break;

  case 271: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 626 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3792 "pascal.tab.c"
    break;

  case 272: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 627 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3798 "pascal.tab.c"
    break;

  case 273: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 628 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3804 "pascal.tab.c"
    break;

  case 274: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 629 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3810 "pascal.tab.c"
    break;

  case 275: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 630 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3816 "pascal.tab.c"
    break;

  case 276: /* argument_list: argument_list COMMA argument  */
#line 633 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3822 "pascal.tab.c"
    break;

  case 277: /* argument_list: argument  */
#line 634 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3828 "pascal.tab.c"
    break;

  case 278: /* argument: expression  */
#line 637 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3834 "pascal.tab.c"
    break;

  case 279: /* argument: expression COLON expression  */
#line 638 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3840 "pascal.tab.c"
    break;

  case 280: /* argument: expression COLON expression COLON expression  */
#line 639 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3846 "pascal.tab.c"
    break;

  case 281: /* assignment: selector BECOMES expression  */
#line 642 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3852 "pascal.tab.c"
    break;

  case 282: /* assignment: selector ADDBECOMES expression  */
#line 643 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3858 "pascal.tab.c"
    break;

  case 283: /* assignment: selector SUBBECOMES expression  */
#line 644 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3864 "pascal.tab.c"
    break;

  case 284: /* assignment: selector MULBECOMES expression  */
#line 645 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3870 "pascal.tab.c"
    break;

  case 285: /* assignment: selector DIVBECOMES expression  */
#line 646 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3876 "pascal.tab.c"
    break;

  case 286: /* assignment: call_with_args BECOMES expression  */
#line 647 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3882 "pascal.tab.c"
    break;

  case 287: /* selector: selector LBRACK expression_list RBRACK  */
#line 650 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3888 "pascal.tab.c"
    break;

  case 288: /* selector: selector PERIOD IDENT  */
#line 651 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3894 "pascal.tab.c"
    break;

  case 289: /* selector: selector ARROW  */
#line 652 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3900 "pascal.tab.c"
    break;

  case 290: /* selector: call_with_args PERIOD IDENT  */
#line 653 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3906 "pascal.tab.c"
    break;

  case 291: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 654 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3912 "pascal.tab.c"
    break;

  case 292: /* selector: call_with_args ARROW  */
#line 655 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3918 "pascal.tab.c"
    break;

  case 293: /* selector: recv_primary PERIOD IDENT  */
#line 656 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3924 "pascal.tab.c"
    break;

  case 294: /* selector: recv_primary ARROW  */
#line 657 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3930 "pascal.tab.c"
    break;

  case 295: /* selector: IDENT  */
#line 658 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3936 "pascal.tab.c"
    break;

  case 296: /* expression_list: expression_list COMMA expression  */
#line 661 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3942 "pascal.tab.c"
    break;

  case 297: /* expression_list: expression  */
#line 662 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3948 "pascal.tab.c"
    break;

  case 298: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 665 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3954 "pascal.tab.c"
    break;

  case 299: /* goto_statement: GOTOSY INTCONST  */
#line 668 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3960 "pascal.tab.c"
    break;

  case 300: /* goto_statement: GOTOSY IDENT  */
#line 669 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3966 "pascal.tab.c"
    break;

  case 301: /* if_statement: IFSY expression THENSY body_stmt  */
#line 672 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3972 "pascal.tab.c"
    break;

  case 302: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 673 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3978 "pascal.tab.c"
    break;

  case 303: /* $@21: %empty  */
#line 676 "pascal.y"
                           { }
#line 3984 "pascal.tab.c"
    break;

  case 304: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 676 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3990 "pascal.tab.c"
    break;

  case 305: /* case_tail: %empty  */
#line 679 "pascal.y"
    { (yyval.node) = NULL; }
#line 3996 "pascal.tab.c"
    break;

  case 306: /* case_tail: ELSESY statement_list  */
#line 680 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4002 "pascal.tab.c"
    break;

  case 307: /* case_tail: OTHERWISESY statement_list  */
#line 681 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4008 "pascal.tab.c"
    break;

  case 308: /* case_list: case_list SEMICOLON case_elem  */
#line 684 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4014 "pascal.tab.c"
    break;

  case 309: /* case_list: case_elem  */
#line 685 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4020 "pascal.tab.c"
    break;

  case 310: /* case_elem: constant_list COLON body_stmt  */
#line 688 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4026 "pascal.tab.c"
    break;

  case 311: /* case_elem: %empty  */
#line 689 "pascal.y"
     { (yyval.node) = NULL; }
#line 4032 "pascal.tab.c"
    break;

  case 312: /* constant_list: constant_list COMMA case_label  */
#line 692 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4038 "pascal.tab.c"
    break;

  case 313: /* constant_list: case_label  */
#line 693 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4044 "pascal.tab.c"
    break;

  case 314: /* case_label: b_expr  */
#line 696 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4050 "pascal.tab.c"
    break;

  case 315: /* case_label: b_expr DOTDOT b_expr  */
#line 697 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4056 "pascal.tab.c"
    break;

  case 316: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 700 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4062 "pascal.tab.c"
    break;

  case 317: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 703 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4068 "pascal.tab.c"
    break;

  case 318: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 706 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4074 "pascal.tab.c"
    break;

  case 319: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 707 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4080 "pascal.tab.c"
    break;

  case 320: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 708 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4086 "pascal.tab.c"
    break;

  case 321: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 711 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4092 "pascal.tab.c"
    break;

  case 322: /* with_open: with_open COMMA selector  */
#line 714 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4098 "pascal.tab.c"
    break;

  case 323: /* with_open: selector  */
#line 715 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4104 "pascal.tab.c"
    break;

  case 324: /* expression: simple_expression  */
#line 722 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4110 "pascal.tab.c"
    break;

  case 325: /* expression: expression INOP simple_expression  */
#line 723 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4116 "pascal.tab.c"
    break;

  case 326: /* expression: expression LTOP simple_expression  */
#line 724 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4122 "pascal.tab.c"
    break;

  case 327: /* expression: expression LEOP simple_expression  */
#line 725 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4128 "pascal.tab.c"
    break;

  case 328: /* expression: expression GTOP simple_expression  */
#line 726 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4134 "pascal.tab.c"
    break;

  case 329: /* expression: expression GEOP simple_expression  */
#line 727 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4140 "pascal.tab.c"
    break;

  case 330: /* expression: expression NEOP simple_expression  */
#line 728 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4146 "pascal.tab.c"
    break;

  case 331: /* expression: expression EQOP simple_expression  */
#line 729 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4152 "pascal.tab.c"
    break;

  case 332: /* expression: expression ISOP simple_expression  */
#line 730 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4158 "pascal.tab.c"
    break;

  case 333: /* simple_expression: term  */
#line 733 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4164 "pascal.tab.c"
    break;

  case 334: /* simple_expression: PLUS term  */
#line 734 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4170 "pascal.tab.c"
    break;

  case 335: /* simple_expression: MINUS term  */
#line 735 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4176 "pascal.tab.c"
    break;

  case 336: /* simple_expression: simple_expression PLUS term  */
#line 736 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4182 "pascal.tab.c"
    break;

  case 337: /* simple_expression: simple_expression MINUS term  */
#line 737 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4188 "pascal.tab.c"
    break;

  case 338: /* simple_expression: simple_expression OROP term  */
#line 738 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4194 "pascal.tab.c"
    break;

  case 339: /* simple_expression: simple_expression XOROP term  */
#line 739 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4200 "pascal.tab.c"
    break;

  case 340: /* term: power  */
#line 742 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4206 "pascal.tab.c"
    break;

  case 341: /* term: term MUL power  */
#line 743 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4212 "pascal.tab.c"
    break;

  case 342: /* term: term RDIV power  */
#line 744 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4218 "pascal.tab.c"
    break;

  case 343: /* term: term IDIV power  */
#line 745 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4224 "pascal.tab.c"
    break;

  case 344: /* term: term IMOD power  */
#line 746 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4230 "pascal.tab.c"
    break;

  case 345: /* term: term ANDOP power  */
#line 747 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4236 "pascal.tab.c"
    break;

  case 346: /* term: term MUL MINUS power  */
#line 748 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4242 "pascal.tab.c"
    break;

  case 347: /* term: term RDIV MINUS power  */
#line 749 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4248 "pascal.tab.c"
    break;

  case 348: /* term: term IDIV MINUS power  */
#line 750 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4254 "pascal.tab.c"
    break;

  case 349: /* term: term IMOD MINUS power  */
#line 751 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4260 "pascal.tab.c"
    break;

  case 350: /* term: term ANDOP MINUS power  */
#line 752 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4266 "pascal.tab.c"
    break;

  case 351: /* term: term SHIFTOP power  */
#line 753 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4272 "pascal.tab.c"
    break;

  case 352: /* term: term ASOP power  */
#line 754 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4278 "pascal.tab.c"
    break;

  case 353: /* power: factor  */
#line 757 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4284 "pascal.tab.c"
    break;

  case 354: /* power: power POWOP factor  */
#line 758 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4290 "pascal.tab.c"
    break;

  case 355: /* factor: selector  */
#line 761 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4296 "pascal.tab.c"
    break;

  case 356: /* factor: call_with_args  */
#line 762 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4302 "pascal.tab.c"
    break;

  case 357: /* factor: INTCONST  */
#line 763 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4308 "pascal.tab.c"
    break;

  case 358: /* factor: REALCONST  */
#line 764 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4314 "pascal.tab.c"
    break;

  case 359: /* factor: STRINGCONST  */
#line 765 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4320 "pascal.tab.c"
    break;

  case 360: /* factor: CHARCODE  */
#line 766 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4326 "pascal.tab.c"
    break;

  case 361: /* factor: LPARENT expression RPARENT  */
#line 767 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 4332 "pascal.tab.c"
    break;

  case 362: /* factor: inherited_call  */
#line 768 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4338 "pascal.tab.c"
    break;

  case 363: /* factor: NOTSY factor  */
#line 769 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4344 "pascal.tab.c"
    break;

  case 364: /* factor: ATSIGN factor  */
#line 770 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4350 "pascal.tab.c"
    break;

  case 365: /* factor: LBRACK RBRACK  */
#line 771 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4356 "pascal.tab.c"
    break;

  case 366: /* factor: LBRACK set_member_list RBRACK  */
#line 772 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4362 "pascal.tab.c"
    break;

  case 367: /* set_member_list: set_member  */
#line 775 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4368 "pascal.tab.c"
    break;

  case 368: /* set_member_list: set_member_list COMMA set_member  */
#line 776 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4374 "pascal.tab.c"
    break;

  case 369: /* set_member: expression  */
#line 779 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4380 "pascal.tab.c"
    break;

  case 370: /* set_member: expression DOTDOT expression  */
#line 780 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4386 "pascal.tab.c"
    break;


#line 4390 "pascal.tab.c"

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

#line 786 "pascal.y"

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
