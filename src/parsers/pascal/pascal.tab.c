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
static tree_t *pt_dot(tree_t *a, tree_t *b) {
    if (!a || !b || !a->v.sval || !b->v.sval) return a;
    size_t n = strlen(a->v.sval) + strlen(b->v.sval) + 2;
    char *j = ct_alloc(n);
    snprintf(j, n, "%s.%s", a->v.sval, b->v.sval);
    a->v.sval = j;
    return a;
}
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

#line 244 "pascal.tab.c"

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
  YYSYMBOL_GENLT = 95,                     /* GENLT  */
  YYSYMBOL_GENGT = 96,                     /* GENGT  */
  YYSYMBOL_YYACCEPT = 97,                  /* $accept  */
  YYSYMBOL_compilation_unit = 98,          /* compilation_unit  */
  YYSYMBOL_program = 99,                   /* program  */
  YYSYMBOL_program_name = 100,             /* program_name  */
  YYSYMBOL_file_id_list_opt = 101,         /* file_id_list_opt  */
  YYSYMBOL_block = 102,                    /* block  */
  YYSYMBOL_decl_part_list = 103,           /* decl_part_list  */
  YYSYMBOL_decl_part = 104,                /* decl_part  */
  YYSYMBOL_unit = 105,                     /* unit  */
  YYSYMBOL_intf_part_list = 106,           /* intf_part_list  */
  YYSYMBOL_intf_part = 107,                /* intf_part  */
  YYSYMBOL_init_opt = 108,                 /* init_opt  */
  YYSYMBOL_fini_opt = 109,                 /* fini_opt  */
  YYSYMBOL_label_list = 110,               /* label_list  */
  YYSYMBOL_const_decl_list = 111,          /* const_decl_list  */
  YYSYMBOL_const_decl = 112,               /* const_decl  */
  YYSYMBOL_typed_value = 113,              /* typed_value  */
  YYSYMBOL_typed_value_list = 114,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 115,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 116,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 117,           /* type_decl_list  */
  YYSYMBOL_type_decl = 118,                /* type_decl  */
  YYSYMBOL_uses_list = 119,                /* uses_list  */
  YYSYMBOL_uses_name = 120,                /* uses_name  */
  YYSYMBOL_spec_gen = 121,                 /* spec_gen  */
  YYSYMBOL_spec_expr = 122,                /* spec_expr  */
  YYSYMBOL_class_parents = 123,            /* class_parents  */
  YYSYMBOL_parent_item = 124,              /* parent_item  */
  YYSYMBOL_spec_kw = 125,                  /* spec_kw  */
  YYSYMBOL_spec_ref = 126,                 /* spec_ref  */
  YYSYMBOL_type_arg_list = 127,            /* type_arg_list  */
  YYSYMBOL_type = 128,                     /* type  */
  YYSYMBOL_129_1 = 129,                    /* $@1  */
  YYSYMBOL_130_2 = 130,                    /* $@2  */
  YYSYMBOL_131_3 = 131,                    /* $@3  */
  YYSYMBOL_result_type = 132,              /* result_type  */
  YYSYMBOL_class_members = 133,            /* class_members  */
  YYSYMBOL_class_member = 134,             /* class_member  */
  YYSYMBOL_prop_index_opt = 135,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 136,               /* prop_specs  */
  YYSYMBOL_packed_opt = 137,               /* packed_opt  */
  YYSYMBOL_simple_type = 138,              /* simple_type  */
  YYSYMBOL_lo_bound = 139,                 /* lo_bound  */
  YYSYMBOL_lo_term = 140,                  /* lo_term  */
  YYSYMBOL_b_expr = 141,                   /* b_expr  */
  YYSYMBOL_b_term = 142,                   /* b_term  */
  YYSYMBOL_b_factor = 143,                 /* b_factor  */
  YYSYMBOL_record_body = 144,              /* record_body  */
  YYSYMBOL_record_field_list = 145,        /* record_field_list  */
  YYSYMBOL_record_field = 146,             /* record_field  */
  YYSYMBOL_147_4 = 147,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 148,            /* case_arm_mark  */
  YYSYMBOL_case_open = 149,                /* case_open  */
  YYSYMBOL_record_case_opt = 150,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 151,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 152,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 153,            /* var_decl_list  */
  YYSYMBOL_var_decl = 154,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 155,           /* procedure_decl  */
  YYSYMBOL_156_5 = 156,                    /* $@5  */
  YYSYMBOL_157_6 = 157,                    /* $@6  */
  YYSYMBOL_158_7 = 158,                    /* $@7  */
  YYSYMBOL_159_8 = 159,                    /* $@8  */
  YYSYMBOL_160_9 = 160,                    /* $@9  */
  YYSYMBOL_161_10 = 161,                   /* $@10  */
  YYSYMBOL_162_11 = 162,                   /* $@11  */
  YYSYMBOL_163_12 = 163,                   /* $@12  */
  YYSYMBOL_164_13 = 164,                   /* $@13  */
  YYSYMBOL_165_14 = 165,                   /* $@14  */
  YYSYMBOL_166_15 = 166,                   /* $@15  */
  YYSYMBOL_167_16 = 167,                   /* $@16  */
  YYSYMBOL_168_17 = 168,                   /* $@17  */
  YYSYMBOL_pv_mark = 169,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 170,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 171,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 172,           /* parameter_decl  */
  YYSYMBOL_param_type = 173,               /* param_type  */
  YYSYMBOL_routine_directives = 174,       /* routine_directives  */
  YYSYMBOL_pf_params = 175,                /* pf_params  */
  YYSYMBOL_pf_sections = 176,              /* pf_sections  */
  YYSYMBOL_pf_section = 177,               /* pf_section  */
  YYSYMBOL_enum_list = 178,                /* enum_list  */
  YYSYMBOL_enum_member = 179,              /* enum_member  */
  YYSYMBOL_id_list = 180,                  /* id_list  */
  YYSYMBOL_body = 181,                     /* body  */
  YYSYMBOL_statement_list = 182,           /* statement_list  */
  YYSYMBOL_183_18 = 183,                   /* $@18  */
  YYSYMBOL_184_19 = 184,                   /* $@19  */
  YYSYMBOL_body_stmt = 185,                /* body_stmt  */
  YYSYMBOL_186_20 = 186,                   /* $@20  */
  YYSYMBOL_statement = 187,                /* statement  */
  YYSYMBOL_statement_no_label = 188,       /* statement_no_label  */
  YYSYMBOL_try_statement = 189,            /* try_statement  */
  YYSYMBOL_handler_list = 190,             /* handler_list  */
  YYSYMBOL_handler = 191,                  /* handler  */
  YYSYMBOL_raise_statement = 192,          /* raise_statement  */
  YYSYMBOL_inherited_call = 193,           /* inherited_call  */
  YYSYMBOL_call = 194,                     /* call  */
  YYSYMBOL_recv_primary = 195,             /* recv_primary  */
  YYSYMBOL_call_with_args = 196,           /* call_with_args  */
  YYSYMBOL_argument_list = 197,            /* argument_list  */
  YYSYMBOL_argument = 198,                 /* argument  */
  YYSYMBOL_assignment = 199,               /* assignment  */
  YYSYMBOL_selector = 200,                 /* selector  */
  YYSYMBOL_expression_list = 201,          /* expression_list  */
  YYSYMBOL_compound_statement = 202,       /* compound_statement  */
  YYSYMBOL_goto_statement = 203,           /* goto_statement  */
  YYSYMBOL_if_statement = 204,             /* if_statement  */
  YYSYMBOL_case_statement = 205,           /* case_statement  */
  YYSYMBOL_206_21 = 206,                   /* $@21  */
  YYSYMBOL_case_tail = 207,                /* case_tail  */
  YYSYMBOL_case_list = 208,                /* case_list  */
  YYSYMBOL_case_elem = 209,                /* case_elem  */
  YYSYMBOL_constant_list = 210,            /* constant_list  */
  YYSYMBOL_case_label = 211,               /* case_label  */
  YYSYMBOL_while_statement = 212,          /* while_statement  */
  YYSYMBOL_repeat_statement = 213,         /* repeat_statement  */
  YYSYMBOL_for_statement = 214,            /* for_statement  */
  YYSYMBOL_with_statement = 215,           /* with_statement  */
  YYSYMBOL_with_open = 216,                /* with_open  */
  YYSYMBOL_expression = 217,               /* expression  */
  YYSYMBOL_simple_expression = 218,        /* simple_expression  */
  YYSYMBOL_term = 219,                     /* term  */
  YYSYMBOL_power = 220,                    /* power  */
  YYSYMBOL_factor = 221,                   /* factor  */
  YYSYMBOL_set_member_list = 222,          /* set_member_list  */
  YYSYMBOL_set_member = 223                /* set_member  */
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
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1722

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  392
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  919

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   351


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
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   224,   224,   225,   228,   229,   232,   233,   236,   237,
     240,   243,   244,   247,   248,   249,   250,   251,   252,   253,
     256,   259,   260,   263,   264,   265,   266,   267,   268,   271,
     272,   273,   276,   277,   280,   281,   282,   283,   286,   287,
     290,   291,   294,   295,   296,   297,   300,   301,   304,   305,
     308,   309,   310,   311,   312,   315,   316,   319,   320,   321,
     322,   323,   326,   327,   330,   331,   334,   337,   338,   341,
     342,   345,   346,   349,   352,   353,   354,   357,   358,   361,
     362,   363,   364,   364,   364,   365,   366,   367,   368,   369,
     370,   371,   371,   372,   373,   374,   375,   376,   379,   380,
     381,   384,   385,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   400,   401,   404,   405,   406,   409,   410,
     413,   414,   415,   416,   417,   418,   421,   422,   423,   424,
     425,   428,   429,   430,   431,   432,   433,   436,   437,   438,
     439,   440,   443,   444,   445,   446,   447,   450,   451,   452,
     455,   458,   459,   462,   462,   463,   464,   465,   466,   467,
     470,   473,   476,   477,   478,   481,   482,   485,   486,   489,
     490,   493,   494,   495,   498,   499,   500,   500,   501,   501,
     502,   502,   503,   503,   504,   504,   505,   505,   506,   506,
     507,   507,   508,   508,   511,   511,   514,   514,   517,   517,
     520,   520,   525,   528,   529,   530,   533,   534,   537,   538,
     539,   540,   541,   542,   543,   544,   547,   548,   549,   550,
     551,   554,   555,   565,   566,   567,   570,   571,   574,   575,
     576,   577,   578,   579,   582,   583,   586,   589,   590,   593,
     596,   596,   597,   597,   600,   600,   603,   604,   605,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   625,   626,   627,   628,   631,   632,
     633,   636,   637,   640,   641,   644,   645,   646,   649,   650,
     651,   652,   655,   656,   657,   660,   661,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   674,   675,   678,
     679,   680,   681,   684,   685,   686,   687,   688,   689,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   703,   704,
     707,   710,   711,   714,   715,   718,   718,   721,   722,   723,
     726,   727,   730,   731,   734,   735,   738,   739,   742,   745,
     748,   749,   750,   753,   756,   757,   764,   765,   766,   767,
     768,   769,   770,   771,   772,   775,   776,   777,   778,   779,
     780,   781,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   799,   800,   803,   804,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   817,
     818,   821,   822
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
  "GENERICTSY", "SPECIALIZESY", "GENLT", "GENGT", "$accept",
  "compilation_unit", "program", "program_name", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "unit", "intf_part_list",
  "intf_part", "init_opt", "fini_opt", "label_list", "const_decl_list",
  "const_decl", "typed_value", "typed_value_list", "field_value_list",
  "scalar_constant", "type_decl_list", "type_decl", "uses_list",
  "uses_name", "spec_gen", "spec_expr", "class_parents", "parent_item",
  "spec_kw", "spec_ref", "type_arg_list", "type", "$@1", "$@2", "$@3",
  "result_type", "class_members", "class_member", "prop_index_opt",
  "prop_specs", "packed_opt", "simple_type", "lo_bound", "lo_term",
  "b_expr", "b_term", "b_factor", "record_body", "record_field_list",
  "record_field", "$@4", "case_arm_mark", "case_open", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "pv_mark",
  "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "param_type", "routine_directives", "pf_params", "pf_sections",
  "pf_section", "enum_list", "enum_member", "id_list", "body",
  "statement_list", "$@18", "$@19", "body_stmt", "$@20", "statement",
  "statement_no_label", "try_statement", "handler_list", "handler",
  "raise_statement", "inherited_call", "call", "recv_primary",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@21", "case_tail", "case_list",
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

#define YYPACT_NINF (-555)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-318)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     308,    40,    66,   181,  -555,   125,   764,  -555,  -555,   358,
    -555,   120,  -555,  -555,   135,   239,    66,   303,   335,   335,
     347,  -555,    97,   352,   379,   416,   357,  -555,  -555,  -555,
     431,   335,   481,   344,   441,  -555,  -555,   140,   393,   239,
    -555,   151,   464,   197,  -555,   335,  -555,    57,   152,   283,
     219,   840,   403,   472,    97,  -555,   487,   505,   473,   530,
     521,   550,  -555,   410,  -555,  -555,  -555,  -555,   304,  1231,
     926,  -555,  -555,    66,   335,   557,   589,  -555,   593,   926,
    -555,   335,   594,    27,  -555,  -555,   523,   614,  -555,  1231,
      91,  1231,  -555,  1231,  1231,   631,  -555,   299,  -555,  1231,
     618,   621,  -555,   585,  -555,  -555,  -555,  -555,  -555,  -555,
    -555,   207,   886,  -555,   779,  -555,  -555,  -555,  -555,  -555,
    -555,  -555,  -555,   335,   967,   653,  -555,   650,   659,   646,
     652,   699,   704,  -555,   670,   946,  -555,  -555,  1466,  1466,
    1466,  1063,  1231,  1466,  -555,  -555,  -555,   280,   -14,   618,
     678,  -555,   556,   645,    81,   360,   458,   688,  -555,   713,
    -555,   713,   601,   601,   -25,   702,  -555,  -555,  -555,  -555,
    1331,   718,  -555,  -555,  -555,  -555,   732,   323,  -555,   371,
     289,   464,   370,  -555,   167,   787,  -555,    31,   386,  -555,
    -555,   733,   840,  -555,  -555,   567,   285,   579,   556,   645,
     136,  1051,   225,  1513,  1525,   882,   882,  1079,   817,    21,
    1641,   734,   754,  1105,   721,  -555,  1231,   729,  -555,  1231,
    1121,  1231,   730,  -555,  1231,  1231,  1231,  1231,  1231,   400,
      63,   807,   335,   740,   750,  -555,  -555,   335,   335,  -555,
     239,    66,  -555,   751,   335,   763,    97,  -555,  -555,   458,
     458,  -555,  1547,   453,  -555,  1559,  -555,   766,  -555,  1231,
    1231,  1231,  1231,  1231,  1231,  1231,  1231,  1466,  1466,  1466,
    1466,  1348,  1382,  1398,  1424,  1440,  1466,  1466,  1466,   189,
     818,   791,   790,   289,   289,    29,  -555,  -555,   440,  -555,
     817,   782,   962,   962,   230,   351,  1268,    25,   835,   841,
    -555,   747,   747,   962,   747,   747,   747,   747,  -555,   589,
     335,   800,   335,   806,  -555,   335,    43,  -555,   622,   874,
    -555,  1268,   808,  -555,   589,  -555,   277,  -555,  1231,  1231,
    1231,  -555,  -555,    91,  -555,  -555,  -555,  -555,   586,  -555,
    -555,  -555,   478,  -555,   654,  1581,   -16,  -555,   795,  -555,
    1147,   817,  -555,   503,   823,  1641,   824,   494,  1641,  -555,
     509,  1641,   694,   577,  1641,  1641,  1641,  1641,   844,  -555,
     230,  -555,   422,  -555,  -555,   589,   589,   438,   463,   239,
     200,   112,  -555,   335,  -555,    97,  1231,  1231,  -555,   664,
     834,   360,   360,   360,   360,   360,   360,   360,   360,   458,
     458,   458,   458,  1466,   688,  1466,   688,  1466,   688,  1466,
     688,  1466,   688,   688,   688,  -555,   335,   832,   335,   833,
    -555,   335,    49,  -555,   680,   837,   277,   702,  -555,   476,
    -555,   747,   747,   962,   836,  -555,   484,   356,  -555,   424,
      29,   511,  -555,  -555,   838,  -555,   842,    83,   372,   845,
     849,   852,  -555,   681,  1294,   905,  1641,   926,   817,   926,
     817,   237,   356,   356,    24,  -555,  -555,  -555,  -555,   589,
     912,   685,   713,   701,   713,   710,   170,  -555,    17,   924,
     855,   927,   928,   589,   875,   931,   169,  -555,   935,   229,
    1002,  1641,  -555,   840,  -555,   645,   962,   900,  1231,  -555,
    1231,  1231,   817,  -555,   867,   245,   211,  -555,   256,  -555,
     513,   504,  -555,  1173,  1189,  1231,  -555,  -555,  1215,  -555,
     926,   519,   891,   589,   589,   944,   950,  -555,  -555,  -555,
    -555,  -555,   936,   589,   589,  1641,  -555,   688,   688,   688,
     688,   688,   715,   713,   723,   713,   731,   212,  -555,    17,
    -555,   932,  -555,  -555,   356,   356,   455,   962,   747,   747,
    -555,   747,   747,   747,   747,  -555,   230,  -555,  -555,  -555,
    -555,   885,   887,  -555,  -555,   902,   926,   210,   908,    71,
    1559,  -555,  -555,   578,  -555,  -555,   894,   897,   898,   899,
     937,    47,  -555,   755,   972,  -555,    17,  -555,    17,   923,
      17,  -555,   958,   194,  -555,   938,  -555,   983,  -555,  -555,
    -555,   929,   277,  -555,   916,   988,  1231,  1231,  -555,  -555,
     419,    80,  -555,   797,  -555,  -555,  -555,  1594,  1606,  -555,
      22,  -555,   795,  -555,  -555,  -555,  -555,  -555,  -555,   525,
    -555,   527,  1641,  -555,   533,   999,  1000,   926,  1013,   963,
    -555,  -555,   589,   589,  1019,  1019,  -555,   995,  1022,   969,
      17,  -555,    17,   970,    17,  -555,  -555,   956,  -555,   501,
     356,   356,  -555,  -555,  -555,  -555,  -555,   461,   589,   589,
    -555,  -555,   589,   589,   170,   985,  1041,  1268,  1268,   286,
    -555,   817,  1020,  -555,  -555,  -555,  -555,  -555,   237,   973,
    -555,  -555,  -555,   855,  1004,  -555,   277,  -555,    37,   984,
    1231,  -555,  1054,   277,  1057,  -555,  1065,   855,  1016,  1039,
    -555,   962,   962,  -555,  -555,  1042,   962,  -555,  -555,  1231,
    1231,  -555,   997,  -555,   262,  -555,  -555,  -555,  -555,  -555,
    1066,  -555,   277,   855,   855,  1069,  1015,  1019,  1017,  -555,
     277,  -555,  -555,   277,  -555,  -555,  -555,  -555,  1072,  1018,
     589,   589,  1075,  1077,    55,   230,  -555,  -555,  -555,   549,
    -555,  1024,  1023,  -555,   589,   589,   589,   589,  -555,   163,
     926,   855,  -555,  1231,  -555,  -555,  -555,  -555,  1641,  -555,
    1080,  -555,  -555,  -555,  -555,  -555,    24,  -555,  1019,  1019,
    -555,  -555,  -555,  1619,  1641,  -555,  1078,  -555,  -555,   855,
    1089,  -555,  -555,  -555,   277,  -555,   855,  1091,  -555,  -555,
     277,  1093,  1045,  -555,  -555,  -555,  -555,  1268,  -555,  1268,
    1083,  -555,  -555,  1047,  -555,  -555,  -555,  1032,  -555,  -555,
    1107,  1641,  -555,   855,  1108,  -555,  -555,  -555,  -555,  -555,
    -555,  1109,  1110,   855,  1111,  -555,  1114,  -555,   277,    33,
    -555,  -555,   926,   926,   277,   962,  1095,  1116,  -555,   855,
    -555,  -555,  -555,  1123,   855,  -555,  -555,  -555,  -555,   855,
    -555,  1124,  -555,   591,  -555,  -555,  -555,  1126,  -555,   815,
    -555,  -555,  -555,  1135,  -555,  -555,  1137,   855,  -555,  -555,
    -555,   962,  1081,   962,  1142,  -555,  1144,  -555,  -555,  -555,
    -555,  1126,  -555,  -555,  1151,   237,  -555,  1096,  -555
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   202,   238,    16,   170,     0,     0,   202,
       0,   263,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     7,     0,    12,    22,    65,    13,     0,     0,
     119,    38,    18,     0,     0,     0,   205,   169,     0,   119,
      19,     0,     0,   205,   240,   239,     0,     0,   242,     0,
       0,     0,   242,     0,     0,     0,   282,   278,   242,   273,
     262,     0,    68,   279,    67,   243,   246,   259,   260,   261,
     250,     0,   281,   249,     0,   251,   252,   253,   254,   255,
     256,   257,   258,     0,   119,     0,    55,     0,     0,     0,
       0,     0,     0,     8,     0,     0,    34,    35,     0,     0,
       0,     0,     0,     0,   379,   382,   380,   381,   317,     0,
     284,   384,   378,   377,     0,   346,   355,   362,   375,   225,
     118,   225,     0,     0,     0,     0,    51,    54,    52,    53,
     121,   101,   131,    75,    74,   124,     0,     0,    79,     0,
     126,    63,     0,   202,     0,     0,   237,     0,     0,   202,
     180,     0,   263,   321,   322,     0,     0,     0,     0,   345,
       0,     0,     0,     0,     0,   263,   263,     0,     0,     0,
     274,   275,     0,     0,     0,   316,     0,     0,   314,     0,
       0,     0,     0,   311,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,   202,   202,     0,     0,     4,
       0,     0,    12,     0,     0,     0,     0,    21,   385,   356,
     357,   387,   391,     0,   389,     0,   386,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,    50,   127,   128,    80,    81,   236,     0,   235,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
      91,     0,     0,     0,     0,     0,     0,     0,   202,   205,
       0,     0,     0,     0,   204,   238,     0,   207,     0,   221,
     171,     0,     0,   202,   205,    12,     0,   241,     0,     0,
       0,   244,   244,     0,   325,   320,   244,   283,   278,   247,
     248,   288,     0,   298,   377,   299,     0,    77,   242,   242,
       0,     0,   287,     0,   315,   308,   312,     0,   319,   290,
       0,   303,   280,     0,   304,   305,   306,   307,     0,    60,
       0,    57,     0,   202,   202,   205,   205,     0,     0,    24,
       0,    31,   202,    26,   202,    25,     0,     0,   388,   383,
     310,   347,   348,   349,   350,   351,   352,   353,   354,   360,
     358,   359,   361,     0,   365,     0,   366,     0,   367,     0,
     363,     0,   364,   373,   374,   376,     0,     0,     0,     0,
     224,   238,     0,   227,     0,     0,     0,     0,   120,     0,
     123,     0,     0,     0,    50,   147,     0,   137,   142,     0,
      71,     0,    69,    72,     0,    96,     0,   103,     0,     0,
       0,     0,   102,     0,     0,     0,    42,   119,     0,   119,
       0,   159,   129,   130,   125,   134,   135,   133,   136,   205,
       0,     0,   225,     0,   225,     0,     0,   203,     0,     0,
     176,     0,     0,   205,     0,     0,    98,   100,     0,     0,
       0,   339,   338,   263,   343,   344,   333,   323,     0,   285,
       0,     0,     0,    66,     0,     0,     0,   268,     0,   276,
       0,     0,   286,     0,     0,     0,   313,   289,     0,   309,
     119,     0,     0,   205,   205,     0,     0,   202,   202,    23,
     242,   242,    33,   205,   205,   392,   390,   370,   371,   372,
     368,   369,     0,   225,     0,   225,     0,     0,   223,     0,
      87,    88,   234,    76,   138,   139,     0,     0,     0,     0,
     122,     0,     0,     0,     0,   132,     0,   101,   202,   202,
     104,     0,     0,   202,   202,   113,   119,   317,     0,     0,
      42,    41,    85,     0,    95,    93,     0,     0,     0,     0,
       0,   164,   152,     0,     0,   221,     0,   208,     0,     0,
       0,   206,     0,   216,   220,   211,   174,     0,    12,   172,
     173,     0,     0,   181,     0,   221,     0,     0,   244,   245,
     336,   327,   331,     0,   335,   244,   297,   319,   300,    78,
       0,   264,   270,   265,   242,   267,   277,    73,   296,     0,
     294,     0,   318,   292,     0,     0,   101,   119,     0,     0,
     221,   221,   205,   205,    29,    30,   242,     0,     0,     0,
       0,   232,     0,     0,     0,   226,   228,     0,   149,     0,
     140,   141,   144,   145,   143,   146,    70,     0,   205,   205,
     202,   202,   205,   205,     0,     0,     0,     0,     0,     0,
      44,     0,     0,   202,   202,   202,   202,    92,   159,     0,
     150,   153,   221,   190,   212,   210,     0,   215,     0,     0,
       0,   222,     0,     0,     0,    99,     0,   178,     0,     0,
     342,     0,   333,   242,   242,     0,     0,   244,   324,     0,
       0,   244,     0,   269,     0,   295,   293,   291,    59,    61,
       0,   221,     0,   194,   196,     0,     0,    32,     0,   221,
       0,   230,   229,     0,   231,    89,   148,    97,     0,     0,
     205,   205,     0,     0,     0,     0,   105,    48,    46,     0,
      45,     0,     0,    82,   205,   205,   205,   205,   151,     0,
     119,   186,    12,     0,   209,   219,   218,   217,   213,   177,
       0,   221,   175,    12,   244,   244,   337,   330,   329,   328,
     326,   334,   332,     0,   301,   272,     0,   266,    58,   198,
       0,    12,    12,   221,     0,    20,    27,     0,   233,   106,
       0,     0,     0,   108,   109,   114,   115,     0,    43,     0,
       0,    83,   155,     0,   157,   158,   161,     0,   154,    12,
       0,   214,   221,   192,     0,   341,   340,   302,   244,    12,
     221,     0,     0,   182,     0,   221,     0,   110,     0,     0,
      47,    49,   119,   119,     0,   168,     0,     0,   191,   188,
      12,   179,   271,     0,   200,   195,   197,    12,   221,    28,
     107,     0,   112,     0,    90,    84,   156,   163,   166,     0,
     161,   187,    12,     0,   199,    12,     0,   184,   111,   117,
     116,   168,     0,   168,     0,   193,     0,   183,    12,   165,
     160,   162,   189,   201,     0,   159,   185,     0,   167
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -555,  -555,  -555,  -555,  -555,   -63,   920,  -555,  -555,  -555,
    -555,  -555,  -555,  -555,   918,   -36,  -302,  -555,  -555,   -68,
     917,   -45,   930,    39,   -54,   -34,   798,  -551,     4,  -131,
    -261,   -75,  -555,  -555,  -555,  -404,  -554,  -555,  -555,  -555,
    -555,  -201,  -555,   233,  -272,  -255,  -241,   250,  -555,   486,
    -555,  -555,   284,  -555,   279,   288,   943,   -39,  -555,  -555,
    -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,  -555,
    -555,  -555,    -9,   -40,   507,   722,  -525,   129,  -124,  -555,
     656,  -555,   773,   -19,  -555,   -31,  -555,  -555,  -318,  -555,
    -182,   456,  -555,  -555,   572,  -555,   -23,  -555,  -555,   -46,
    -186,   707,  -555,   381,   987,  -555,  -555,  -555,  -555,  -555,
    -555,  -555,   485,  -488,   491,  -555,  -555,  -555,  -555,  -555,
     609,   749,   -81,  -174,  -108,  -555,   826
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   135,
     247,   532,   657,    37,    39,    40,   455,   769,   579,   435,
      54,    55,    41,    42,   102,   150,   441,   442,   104,   175,
     346,   176,   831,   863,   461,   488,   295,   452,   685,   859,
     177,   178,   179,   180,   620,   437,   438,   590,   591,   592,
     780,   915,   865,   700,   887,   888,    45,    46,    28,   608,
     793,   325,   877,   908,   839,   892,   782,   870,   811,   812,
     849,   895,    76,   185,   316,   317,   605,   480,   280,   422,
     423,   288,   289,    47,    29,    50,   192,    51,   492,   493,
     105,   106,   107,   506,   507,   108,   151,   110,   111,   152,
     342,   343,   113,   153,   363,   115,   116,   117,   118,   496,
     725,   621,   622,   889,   624,   119,   120,   121,   122,   200,
     345,   155,   156,   157,   158,   253,   254
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   134,   172,    71,   187,   112,    77,   347,   623,   126,
     327,   172,    63,   677,   494,   676,   173,   103,   497,   481,
     436,   439,   551,   602,   666,   173,    84,   353,   109,   429,
     248,   464,   190,   286,   360,   256,   320,   281,   882,   731,
      83,    11,   502,   191,   198,   785,   462,   463,   476,   231,
     207,   285,   698,   457,   547,   182,   172,   196,   249,   250,
     476,   202,   188,   465,   466,   467,   468,   209,   369,   101,
     173,   704,   699,   705,   174,   707,   689,   290,   558,   559,
     503,   208,   732,   174,   321,   722,   258,   458,   570,   347,
     511,   184,   677,   603,   172,   172,   723,   404,   406,   408,
     410,   412,   413,   414,   229,   348,   349,   322,   477,   883,
     173,   101,   181,   786,   548,    78,     8,    79,   825,    14,
      15,   724,    16,   259,   208,    33,   530,   370,   174,   260,
     261,   262,   263,   264,   265,   751,   690,   752,    17,   754,
     172,  -238,    10,  -238,    18,    67,   112,    19,    20,   531,
     347,    22,   578,   332,   173,    94,    72,    80,   103,   112,
     112,   556,   266,   443,   510,   318,    96,   148,   174,   109,
     415,   103,   103,    52,   309,   310,   554,   555,   310,    34,
     324,    12,   109,   109,    13,   101,   399,   400,   401,   402,
      53,   836,    23,   311,   333,   487,   311,   416,    68,   312,
      24,    25,   312,   313,    26,   529,   313,    35,   714,    73,
      78,    36,   174,   372,   826,   417,   632,   290,   377,   378,
     416,   418,   172,   837,    84,   419,   375,   376,   614,   537,
      84,   538,   314,   539,   623,   540,   173,   541,   417,   443,
     173,   633,   290,   315,   418,    74,   315,   616,   419,    85,
      84,   617,   634,   709,   420,   335,    75,   583,    73,   585,
     424,    84,   485,   586,   208,   421,   214,    84,   215,   470,
     687,   259,   173,   587,   207,   631,   453,   260,   261,   262,
     263,   264,   265,   172,   484,   669,   635,   198,   421,   208,
      84,   471,   807,   473,   174,   487,   475,   173,   174,   469,
     720,   629,   784,   670,   671,   208,   440,   728,   330,   790,
     266,   619,     1,    44,   483,    38,   173,   505,   508,     2,
     672,   673,   674,   675,   101,   588,   589,   639,   641,   297,
     174,    81,   644,   304,   305,   525,   526,  -317,   810,  -282,
     126,  -282,    82,    71,    77,   306,   817,   604,   597,   818,
     599,   770,   298,   486,   299,   174,    65,   300,  -317,   206,
    -317,  -317,   771,   207,   523,   524,   307,  -317,  -317,  -317,
    -317,   101,   173,   533,   174,   534,   136,   444,    56,    43,
     137,   445,   582,    60,   584,   767,   768,   446,    57,   172,
     172,   172,   172,    61,   208,   283,   284,   542,   571,   544,
     561,   562,   546,   173,   173,   173,   173,   267,   572,   802,
     854,    44,   563,   805,   268,   269,   856,    30,   604,   661,
     308,   663,    31,    49,   173,   301,   302,   447,    78,   594,
     174,   448,   114,   564,   172,   443,   323,   303,   270,   449,
     450,   451,   593,   611,    78,   645,    69,   112,   173,   796,
     368,   123,   172,    70,   881,    58,   124,   318,    78,   103,
     886,   174,   174,   174,   174,   604,   173,   604,    78,   604,
     109,   199,   522,   558,   559,   133,   845,   846,   558,   559,
      78,   487,   174,   648,   649,   721,    64,   444,   527,   565,
     772,   757,    59,   658,   659,   173,    78,   446,   427,   654,
     655,   686,   271,   272,   273,   428,   174,    62,   172,   558,
     559,   387,   173,   528,   274,   275,   388,    66,   652,   653,
     668,    78,   173,    34,   174,   860,   553,   861,   424,   604,
     872,   604,   129,   604,   502,   276,   498,   447,   558,   559,
     277,   448,   173,   499,   173,   712,   173,   560,   125,   449,
     450,   451,   515,   174,   637,   558,   559,   516,   173,   678,
     679,   498,   502,   127,   682,   683,   756,   498,   512,   566,
     174,   498,   740,   114,   517,   487,   567,   566,   636,   172,
     174,   128,   487,   498,   646,   498,   114,   114,   344,   130,
     735,   498,   736,   173,   344,   193,   331,   131,   737,   194,
     174,   344,   174,   734,   174,   328,   173,   827,   173,   329,
     173,   487,   745,   746,   828,   217,   174,   218,   219,   487,
     220,   259,   487,   172,  -317,   747,   132,   260,   261,   262,
     263,   264,   265,   183,   443,   515,   691,   173,   758,   759,
     519,   692,   762,   763,  -284,  -317,  -284,  -317,  -317,   213,
     207,   174,   173,   184,  -317,  -317,  -317,  -317,   453,   173,
     266,   339,   340,   899,   174,   318,   174,   900,   174,   186,
     189,   760,   761,   166,   167,   168,   169,   282,   154,   593,
      78,   208,   478,   487,   774,   775,   776,   777,   173,   487,
     195,   205,   798,   799,   211,   174,   173,   212,   197,   173,
     201,   232,   203,   204,   257,   838,   223,   224,   210,   233,
     174,   173,   172,   257,   495,   223,   500,   174,   234,   840,
     821,   822,   235,  -283,   703,  -283,   173,   487,   236,   239,
     844,   344,  -310,   487,   832,   833,   834,   835,    78,    78,
     549,   576,   213,    78,   717,   596,   174,   237,   851,   852,
     252,   255,   238,  -310,   174,  -310,  -310,   174,   518,    78,
     173,   598,  -310,  -310,  -310,  -310,   173,   278,    78,   174,
     600,    14,    15,    78,    16,   660,   867,   279,   287,   743,
     744,    78,   294,   662,   174,   296,   873,   884,   885,    78,
      17,   664,   319,   326,   172,   172,    18,   354,   350,    19,
      20,    21,   351,    22,   173,   356,   362,   893,   173,   173,
     173,   433,   371,    78,   896,   701,   373,   221,   174,   166,
     167,   168,   169,   434,   174,   355,   374,   382,   358,   904,
     361,   781,   906,   358,   364,   365,   366,   367,   222,   384,
     223,   224,   390,    86,    23,   914,   425,   225,   226,   227,
     228,   426,    24,    25,   293,   726,    26,   727,   430,    87,
      88,    89,   174,   459,    90,    91,   174,   174,   174,   460,
     809,   162,   163,   726,   114,   902,   472,    92,   816,   344,
      93,   165,   474,   479,   482,    86,   504,   513,   514,   166,
     167,   168,   169,   170,   344,   344,   593,   520,   518,   344,
     557,    87,    88,    89,    94,   456,    90,    91,   543,   545,
     581,   101,    95,   550,   568,    96,    97,   595,   569,    92,
     843,   573,    93,    98,   216,   574,    99,   100,   575,   606,
     456,   607,   609,   610,   101,   612,   613,   489,   490,   491,
     615,   625,   853,   630,   647,   217,    94,   218,   219,   650,
     220,   656,   159,   160,   240,   651,   241,    96,   338,   242,
     667,   680,   161,   681,   684,    98,   688,   697,    99,   100,
     693,   869,   243,   694,   695,   696,   101,   702,   244,   874,
     162,   163,   245,   706,   879,   246,   708,   164,   711,   713,
     165,   710,   715,   159,   160,   535,   252,   716,   166,   167,
     168,   169,   170,   161,   738,   739,   171,   897,   391,   392,
     393,   394,   395,   396,   397,   398,   431,   432,   741,   618,
     101,   162,   163,   742,    84,   748,   433,   749,   164,   750,
     753,   165,   755,   794,   166,   167,   168,   169,   434,   166,
     167,   168,   169,   170,   259,   765,   766,   230,   773,   779,
     260,   261,   262,   263,   264,   265,   795,   783,   259,   789,
     787,   101,   791,   580,   260,   261,   262,   263,   264,   265,
     792,   808,   800,   806,   813,   814,   815,   819,   820,   334,
     823,   259,   824,   266,   829,   842,   830,   260,   261,   262,
     263,   264,   265,   259,   850,   848,   855,   266,   857,   260,
     261,   262,   263,   264,   265,   858,   138,   864,   866,   627,
     628,   862,   868,   871,   875,   876,   878,   139,   140,   880,
     266,   891,   138,   890,   642,   141,   251,   142,   894,   898,
     143,   901,   266,   139,   140,   144,   145,   146,   147,   148,
     905,   141,   907,   142,   341,   910,   143,   912,   138,   913,
     149,   144,   145,   146,   147,   148,   916,   101,   379,   139,
     140,   918,   381,   385,   138,   917,   149,   141,   521,   142,
     352,   380,   143,   101,   903,   139,   140,   144,   145,   146,
     147,   148,   911,   141,   778,   142,   359,   383,   143,   909,
     138,   764,   149,   144,   145,   146,   147,   148,   601,   101,
     552,   139,   140,   665,   733,   626,   357,   797,   149,   141,
       0,   142,   509,   536,   143,   101,   138,   801,     0,   144,
     145,   146,   147,   148,     0,   718,   719,   139,   140,     0,
       0,     0,   138,     0,   149,   141,     0,   142,   638,     0,
     143,   101,     0,   139,   140,   144,   145,   146,   147,   148,
       0,   141,     0,   142,   640,     0,   143,     0,   138,     0,
     149,   144,   145,   146,   147,   148,     0,   101,     0,   139,
     140,     0,     0,     0,   138,     0,   149,   141,     0,   142,
     643,     0,   143,   101,     0,   139,   140,   144,   145,   146,
     147,   148,     0,   141,     0,   142,   456,   456,   143,     0,
       0,     0,   149,   144,   145,   146,   147,   148,     0,   101,
       0,   138,     0,     0,     0,     0,     0,     0,   149,   788,
       0,     0,   139,   140,     0,   101,     0,     0,     0,     0,
     141,     0,   454,     0,     0,   143,     0,   138,   803,   804,
     144,   145,   146,   147,   148,     0,     0,     0,   139,   140,
       0,     0,     0,     0,     0,   149,   141,     0,   454,     0,
       0,   143,   101,     0,     0,     0,   144,   145,   146,   147,
     577,     0,     0,     0,     0,   -50,   -50,     0,     0,   290,
       0,   149,     0,     0,     0,   -50,   -50,   -50,   101,     0,
     291,   138,   841,   292,     0,   293,     0,   -50,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,   -50,     0,
     141,     0,   142,     0,     0,   143,     0,     0,     0,     0,
     144,   145,   146,   147,   148,   138,   208,     0,     0,     0,
       0,     0,     0,     0,     0,   149,   456,   405,   456,     0,
       0,   138,   101,     0,   141,     0,   142,     0,     0,   143,
       0,     0,     0,   407,   144,   145,   146,   147,   148,     0,
     141,     0,   142,     0,     0,   143,     0,   138,     0,   149,
     144,   145,   146,   147,   148,     0,   101,     0,     0,   409,
       0,     0,     0,   138,     0,   149,   141,     0,   142,     0,
       0,   143,   101,     0,     0,   411,   144,   145,   146,   147,
     148,     0,   141,     0,   142,     0,     0,   143,     0,   138,
       0,   149,   144,   145,   146,   147,   148,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   149,   141,     0,
     142,     0,     0,   143,   101,     0,     0,     0,   144,   145,
     146,   147,   148,     0,     0,     0,   336,     0,     0,     0,
       0,     0,     0,   149,     0,   259,     0,     0,     0,     0,
     101,   260,   261,   262,   263,   264,   265,   259,     0,     0,
       0,     0,     0,   260,   261,   262,   263,   264,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     337,     0,     0,     0,   266,   260,   261,   262,   263,   264,
     265,   259,     0,     0,     0,     0,   266,   260,   261,   262,
     263,   264,   265,   386,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   259,   389,     0,     0,     0,   266,   260,
     261,   262,   263,   264,   265,     0,   259,     0,     0,     0,
     266,   501,   260,   261,   262,   263,   264,   265,   259,     0,
       0,     0,     0,     0,   260,   261,   262,   263,   264,   265,
     729,   259,   266,     0,     0,     0,   730,   260,   261,   262,
     263,   264,   265,     0,     0,   266,     0,     0,     0,     0,
       0,     0,   847,   259,     0,     0,     0,   266,     0,   260,
     261,   262,   263,   264,   265,     0,     0,     0,     0,     0,
     266,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   266
};

static const yytype_int16 yycheck[] =
{
      19,    64,    70,    39,    79,    51,    45,   208,   496,    54,
     192,    79,    31,   567,   332,   566,    70,    51,   336,   321,
     292,   293,   426,     6,   549,    79,     5,   213,    51,   290,
     138,   303,     5,   164,   220,   143,     5,   161,     5,    17,
      49,     2,    58,    83,    90,     8,   301,   302,     5,   124,
      64,    76,     5,    28,     5,    74,   124,    88,   139,   140,
       5,    92,    81,   304,   305,   306,   307,    98,     5,    94,
     124,   596,    25,   598,    70,   600,     5,    48,    54,    55,
      96,    95,    60,    79,    53,     5,     5,    62,     5,   290,
     351,    64,   646,    76,   162,   163,    16,   271,   272,   273,
     274,   275,   276,   277,   123,    84,    85,    76,    65,    76,
     164,    94,    73,    76,    65,    58,    76,    60,    63,     7,
       8,    41,    10,    42,    95,     5,    14,    64,   124,    48,
      49,    50,    51,    52,    53,   660,    65,   662,    26,   664,
     208,    58,    76,    60,    32,     5,   192,    35,    36,    37,
     351,    39,   454,    17,   208,    64,     5,     5,   192,   205,
     206,   433,    81,   294,   350,   184,    75,    76,   164,   192,
     278,   205,   206,    76,   183,     8,   431,   432,     8,    59,
     189,     0,   205,   206,    59,    94,   267,   268,   269,   270,
      93,    28,    80,    26,    58,   326,    26,     8,    58,    32,
      88,    89,    32,    36,    92,     5,    36,    72,   612,    58,
      58,    76,   208,   232,   765,    26,     5,    48,   237,   238,
       8,    32,   290,    60,     5,    36,   235,   236,    59,   403,
       5,   405,    65,   407,   722,   409,   290,   411,    26,   370,
     294,    30,    48,    76,    32,    48,    76,    18,    36,    30,
       5,    22,    41,    59,    65,    30,    59,   458,    58,   460,
     279,     5,   325,    26,    95,    76,    59,     5,    61,   309,
      60,    42,   326,    36,    64,    30,   295,    48,    49,    50,
      51,    52,    53,   351,   324,   557,    30,   333,    76,    95,
       5,   310,    30,   312,   290,   426,   315,   351,   294,   308,
     618,   502,   706,   558,   559,    95,    76,   625,    23,   713,
      81,   493,     4,    76,   323,    76,   370,   348,   349,    11,
     561,   562,   563,   564,    94,    88,    89,   513,   514,     6,
     326,    48,   518,    44,    45,   375,   376,    38,   742,    59,
     385,    61,    59,   379,   383,    56,   750,   478,   472,   753,
     474,    65,    29,    76,    31,   351,    12,    34,    59,    60,
      61,    62,    76,    64,   373,   374,    77,    68,    69,    70,
      71,    94,   426,   382,   370,   384,    72,    26,    26,    76,
      76,    30,   457,    26,   459,   687,   688,    36,    36,   457,
     458,   459,   460,    36,    95,   162,   163,   416,    26,   418,
      44,    45,   421,   457,   458,   459,   460,    47,    36,   727,
     814,    76,    56,   731,    54,    55,   820,    59,   549,   543,
      50,   545,    64,    76,   478,    54,    55,    76,    58,   469,
     426,    80,    51,    77,   502,   566,    50,    66,    78,    88,
      89,    90,   461,   483,    58,   520,    53,   493,   502,   721,
      50,    48,   520,    60,   858,    76,    53,   476,    58,   493,
     864,   457,   458,   459,   460,   596,   520,   598,    58,   600,
     493,    90,    50,    54,    55,    65,   794,   795,    54,    55,
      58,   612,   478,   523,   524,    66,     5,    26,    50,    65,
     691,    30,    76,   533,   534,   549,    58,    36,    58,   530,
     531,   576,    44,    45,    46,    65,   502,    76,   576,    54,
      55,    58,   566,    50,    56,    57,    63,    76,   527,   528,
      65,    58,   576,    59,   520,   827,    50,   829,   547,   660,
     848,   662,    59,   664,    58,    77,    58,    76,    54,    55,
      82,    80,   596,    65,   598,   608,   600,    63,    76,    88,
      89,    90,    58,   549,    50,    54,    55,    63,   612,   568,
     569,    58,    58,    76,   573,   574,    65,    58,    65,    58,
     566,    58,   647,   192,    65,   706,    65,    58,    65,   647,
     576,    76,   713,    58,    65,    58,   205,   206,   207,    59,
      65,    58,    65,   647,   213,    72,    17,    76,    65,    76,
     596,   220,   598,   634,   600,    38,   660,    58,   662,    42,
     664,   742,   652,   653,    65,    59,   612,    61,    62,   750,
      64,    42,   753,   691,    38,   656,    76,    48,    49,    50,
      51,    52,    53,    76,   765,    58,    58,   691,   678,   679,
      63,    63,   682,   683,    59,    59,    61,    61,    62,    64,
      64,   647,   706,    64,    68,    69,    70,    71,   677,   713,
      81,   205,   206,    72,   660,   684,   662,    76,   664,    76,
      76,   680,   681,    72,    73,    74,    75,    76,    69,   698,
      58,    95,    60,   814,   693,   694,   695,   696,   742,   820,
      76,    60,   723,   724,    76,   691,   750,    76,    89,   753,
      91,    48,    93,    94,    59,   780,    61,    62,    99,    59,
     706,   765,   780,    59,   333,    61,    62,   713,    59,   782,
     760,   761,    76,    59,   595,    61,   780,   858,    76,    59,
     793,   350,    38,   864,   774,   775,   776,   777,    58,    58,
      60,    60,    64,    58,   615,    60,   742,    48,   811,   812,
     141,   142,    48,    59,   750,    61,    62,   753,    64,    58,
     814,    60,    68,    69,    70,    71,   820,    79,    58,   765,
      60,     7,     8,    58,    10,    60,   839,    64,    76,   650,
     651,    58,    64,    60,   780,    53,   849,   862,   863,    58,
      26,    60,     5,    60,   862,   863,    32,    76,    64,    35,
      36,    37,    48,    39,   858,    76,    76,   870,   862,   863,
     864,    64,     5,    58,   877,    60,    76,    38,   814,    72,
      73,    74,    75,    76,   820,   216,    76,    76,   219,   892,
     221,   702,   895,   224,   225,   226,   227,   228,    59,    76,
      61,    62,    76,     3,    80,   908,    28,    68,    69,    70,
      71,    60,    88,    89,    64,    58,    92,    60,    76,    19,
      20,    21,   858,    28,    24,    25,   862,   863,   864,    28,
     741,    54,    55,    58,   493,    60,    76,    37,   749,   498,
      40,    64,    76,     9,    76,     3,    91,    64,    64,    72,
      73,    74,    75,    76,   513,   514,   915,    53,    64,   518,
      64,    19,    20,    21,    64,   296,    24,    25,    76,    76,
       5,    94,    72,    76,    76,    75,    76,     5,    76,    37,
     791,    76,    40,    83,    38,    76,    86,    87,    76,     5,
     321,    76,     5,     5,    94,    60,     5,   328,   329,   330,
       5,    41,   813,    76,    53,    59,    64,    61,    62,     5,
      64,    15,    26,    27,     8,     5,    10,    75,    76,    13,
      28,    76,    36,    76,    62,    83,    58,    30,    86,    87,
      76,   842,    26,    76,    76,    76,    94,     5,    32,   850,
      54,    55,    36,    60,   855,    39,    28,    61,     5,    60,
      64,    53,    76,    26,    27,   386,   387,     9,    72,    73,
      74,    75,    76,    36,     5,     5,    80,   878,   259,   260,
     261,   262,   263,   264,   265,   266,    54,    55,     5,    17,
      94,    54,    55,    60,     5,    30,    64,     5,    61,    60,
      60,    64,    76,    17,    72,    73,    74,    75,    76,    72,
      73,    74,    75,    76,    42,    60,     5,    80,    28,    76,
      48,    49,    50,    51,    52,    53,    17,    53,    42,     5,
      76,    94,     5,   454,    48,    49,    50,    51,    52,    53,
       5,     5,    30,    76,     5,    60,    59,     5,    60,    28,
       5,    42,     5,    81,    60,     5,    63,    48,    49,    50,
      51,    52,    53,    42,     5,    17,     5,    81,     5,    48,
      49,    50,    51,    52,    53,    60,    43,    60,    76,   500,
     501,    28,     5,     5,     5,     5,     5,    54,    55,     5,
      81,     5,    43,    28,   515,    62,    63,    64,     5,     5,
      67,     5,    81,    54,    55,    72,    73,    74,    75,    76,
       5,    62,     5,    64,    65,    64,    67,     5,    43,     5,
      87,    72,    73,    74,    75,    76,     5,    94,   240,    54,
      55,    65,   242,   246,    43,   915,    87,    62,   370,    64,
      65,   241,    67,    94,   890,    54,    55,    72,    73,    74,
      75,    76,   903,    62,   698,    64,    65,   244,    67,   901,
      43,   684,    87,    72,    73,    74,    75,    76,   476,    94,
     427,    54,    55,   547,   632,   498,   219,   722,    87,    62,
      -1,    64,    65,   387,    67,    94,    43,   726,    -1,    72,
      73,    74,    75,    76,    -1,   616,   617,    54,    55,    -1,
      -1,    -1,    43,    -1,    87,    62,    -1,    64,    65,    -1,
      67,    94,    -1,    54,    55,    72,    73,    74,    75,    76,
      -1,    62,    -1,    64,    65,    -1,    67,    -1,    43,    -1,
      87,    72,    73,    74,    75,    76,    -1,    94,    -1,    54,
      55,    -1,    -1,    -1,    43,    -1,    87,    62,    -1,    64,
      65,    -1,    67,    94,    -1,    54,    55,    72,    73,    74,
      75,    76,    -1,    62,    -1,    64,   687,   688,    67,    -1,
      -1,    -1,    87,    72,    73,    74,    75,    76,    -1,    94,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    87,   710,
      -1,    -1,    54,    55,    -1,    94,    -1,    -1,    -1,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    43,   729,   730,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    87,    62,    -1,    64,    -1,
      -1,    67,    94,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,    48,
      -1,    87,    -1,    -1,    -1,    54,    55,    56,    94,    -1,
      59,    43,   783,    62,    -1,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    77,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    43,    95,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,   827,    55,   829,    -1,
      -1,    43,    94,    -1,    62,    -1,    64,    -1,    -1,    67,
      -1,    -1,    -1,    55,    72,    73,    74,    75,    76,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    43,    -1,    87,
      72,    73,    74,    75,    76,    -1,    94,    -1,    -1,    55,
      -1,    -1,    -1,    43,    -1,    87,    62,    -1,    64,    -1,
      -1,    67,    94,    -1,    -1,    55,    72,    73,    74,    75,
      76,    -1,    62,    -1,    64,    -1,    -1,    67,    -1,    43,
      -1,    87,    72,    73,    74,    75,    76,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    62,    -1,
      64,    -1,    -1,    67,    94,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    42,    -1,    -1,    -1,    -1,
      94,    48,    49,    50,    51,    52,    53,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      65,    -1,    -1,    -1,    81,    48,    49,    50,    51,    52,
      53,    42,    -1,    -1,    -1,    -1,    81,    48,    49,    50,
      51,    52,    53,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    65,    -1,    -1,    -1,    81,    48,
      49,    50,    51,    52,    53,    -1,    42,    -1,    -1,    -1,
      81,    60,    48,    49,    50,    51,    52,    53,    42,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      66,    42,    81,    -1,    -1,    -1,    60,    48,    49,    50,
      51,    52,    53,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    42,    -1,    -1,    -1,    81,    -1,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    98,    99,   102,   103,   105,    76,   100,
      76,   120,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    88,    89,    92,   104,   155,   181,
      59,    64,   101,     5,    59,    72,    76,   110,    76,   111,
     112,   119,   120,    76,    76,   153,   154,   180,   180,    76,
     182,   184,    76,    93,   117,   118,    26,    36,    76,    76,
      26,    36,    76,   180,     5,    12,    76,     5,    58,    53,
      60,   112,     5,    58,    48,    59,   169,   154,    58,    60,
       5,    48,    59,   169,     5,    30,     3,    19,    20,    21,
      24,    25,    37,    40,    64,    72,    75,    76,    83,    86,
      87,    94,   121,   122,   125,   187,   188,   189,   192,   193,
     194,   195,   196,   199,   200,   202,   203,   204,   205,   212,
     213,   214,   215,    48,    53,    76,   118,    76,    76,    59,
      59,    76,    76,    65,   102,   106,    72,    76,    43,    54,
      55,    62,    64,    67,    72,    73,    74,    75,    76,    87,
     122,   193,   196,   200,   217,   218,   219,   220,   221,    26,
      27,    36,    54,    55,    61,    64,    72,    73,    74,    75,
      76,    80,   116,   121,   125,   126,   128,   137,   138,   139,
     140,   120,   180,    76,    64,   170,    76,   128,   180,    76,
       5,   170,   183,    72,    76,    76,   182,   217,   196,   200,
     216,   217,   182,   217,   217,    60,    60,    64,    95,   182,
     217,    76,    76,    64,    59,    61,    38,    59,    61,    62,
      64,    38,    59,    61,    62,    68,    69,    70,    71,   180,
      80,   128,    48,    59,    59,    76,    76,    48,    48,    59,
       8,    10,    13,    26,    32,    36,    39,   107,   221,   219,
     219,    63,   217,   222,   223,   217,   221,    59,     5,    42,
      48,    49,    50,    51,    52,    53,    81,    47,    54,    55,
      78,    44,    45,    46,    56,    57,    77,    82,    79,    64,
     175,   175,    76,   140,   140,    76,   126,    76,   178,   179,
      48,    59,    62,    64,    64,   133,    53,     6,    29,    31,
      34,    54,    55,    66,    44,    45,    56,    77,    50,   169,
       8,    26,    32,    36,    65,    76,   171,   172,   180,     5,
       5,    53,    76,    50,   169,   158,    60,   187,    38,    42,
      23,    17,    17,    58,    28,    30,    33,    65,    76,   188,
     188,    65,   197,   198,   200,   217,   127,   138,    84,    85,
      64,    48,    65,   197,    76,   217,    76,   201,   217,    65,
     197,   217,    76,   201,   217,   217,   217,   217,    50,     5,
      64,     5,   180,    76,    76,   169,   169,   180,   180,   111,
     119,   103,    76,   153,    76,   117,    66,    58,    63,    65,
      76,   218,   218,   218,   218,   218,   218,   218,   218,   219,
     219,   219,   219,    55,   220,    55,   220,    55,   220,    55,
     220,    55,   220,   220,   220,   221,     8,    26,    32,    36,
      65,    76,   176,   177,   180,    28,    60,    58,    65,   127,
      76,    54,    55,    64,    76,   116,   141,   142,   143,   141,
      76,   123,   124,   126,    26,    30,    36,    76,    80,    88,
      89,    90,   134,   180,    64,   113,   217,    28,    62,    28,
      28,   131,   142,   142,   141,   143,   143,   143,   143,   169,
     170,   180,    76,   180,    76,   180,     5,    65,    60,     9,
     174,   113,    76,   169,   170,   102,    76,   126,   132,   217,
     217,   217,   185,   186,   185,   200,   206,   185,    58,    65,
      62,    60,    58,    96,    91,   182,   190,   191,   182,    65,
     197,   127,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   123,    50,   169,   169,   170,   170,    50,    50,     5,
      14,    37,   108,   169,   169,   217,   223,   220,   220,   220,
     220,   220,   180,    76,   180,    76,   180,     5,    65,    60,
      76,   132,   179,    50,   142,   142,   141,    64,    54,    55,
      63,    44,    45,    56,    77,    65,    58,    65,    76,    76,
       5,    26,    36,    76,    76,    76,    60,    76,   113,   115,
     217,     5,   128,   138,   128,   138,    26,    36,    88,    89,
     144,   145,   146,   180,   170,     5,    60,   175,    60,   175,
      60,   172,     6,    76,   126,   173,     5,    76,   156,     5,
       5,   170,    60,     5,    59,     5,    18,    22,    17,   187,
     141,   208,   209,   210,   211,    41,   198,   217,   217,   138,
      76,    30,     5,    30,    41,    30,    65,    50,    65,   197,
      65,   197,   217,    65,   197,   128,    65,    53,   170,   170,
       5,     5,   169,   169,   182,   182,    15,   109,   170,   170,
      60,   175,    60,   175,    60,   177,   173,    28,    65,   141,
     142,   142,   143,   143,   143,   143,   124,   133,   169,   169,
      76,    76,   169,   169,    62,   135,   128,    60,    58,     5,
      65,    58,    63,    76,    76,    76,    76,    30,     5,    25,
     150,    60,     5,   174,   173,   173,    60,   173,    28,    59,
      53,     5,   102,    60,   132,    76,     9,   174,   217,   217,
     185,    66,     5,    16,    41,   207,    58,    60,   185,    66,
      60,    17,    60,   191,   182,    65,    65,    65,     5,     5,
     128,     5,    60,   174,   174,   170,   170,   182,    30,     5,
      60,   173,   173,    60,   173,    76,    65,    30,   170,   170,
     169,   169,   170,   170,   171,    60,     5,   113,   113,   114,
      65,    76,   138,    28,   169,   169,   169,   169,   146,    76,
     147,   174,   163,    53,   132,     8,    76,    76,   217,     5,
     132,     5,     5,   157,    17,    17,   141,   209,   182,   182,
      30,   211,   185,   217,   217,   185,    76,    30,     5,   174,
     132,   165,   166,     5,    60,    59,   174,   132,   132,     5,
      60,   170,   170,     5,     5,    63,   124,    58,    65,    60,
      63,   129,   170,   170,   170,   170,    28,    60,   128,   161,
     102,   217,     5,   174,   102,   185,   185,    63,    17,   167,
       5,   102,   102,   174,   132,     5,   132,     5,    60,   136,
     113,   113,    28,   130,    60,   149,    76,   102,     5,   174,
     164,     5,   185,   102,   174,     5,     5,   159,     5,   174,
       5,   132,     5,    76,   128,   128,   132,   151,   152,   210,
      28,     5,   162,   102,     5,   168,   102,   174,     5,    72,
      76,     5,    60,   149,   102,     5,   102,     5,   160,   152,
      64,   151,     5,     5,   102,   148,     5,   144,    65
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     102,   103,   103,   104,   104,   104,   104,   104,   104,   104,
     105,   106,   106,   107,   107,   107,   107,   107,   107,   108,
     108,   108,   109,   109,   110,   110,   110,   110,   111,   111,
     112,   112,   113,   113,   113,   113,   114,   114,   115,   115,
     116,   116,   116,   116,   116,   117,   117,   118,   118,   118,
     118,   118,   119,   119,   120,   120,   121,   122,   122,   123,
     123,   124,   124,   125,   126,   126,   126,   127,   127,   128,
     128,   128,   129,   130,   128,   128,   128,   128,   128,   128,
     128,   131,   128,   128,   128,   128,   128,   128,   132,   132,
     132,   133,   133,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   135,   135,   136,   136,   136,   137,   137,
     138,   138,   138,   138,   138,   138,   139,   139,   139,   139,
     139,   140,   140,   140,   140,   140,   140,   141,   141,   141,
     141,   141,   142,   142,   142,   142,   142,   143,   143,   143,
     144,   145,   145,   147,   146,   146,   146,   146,   146,   146,
     148,   149,   150,   150,   150,   151,   151,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   156,   155,   157,   155,
     158,   155,   159,   155,   160,   155,   161,   155,   162,   155,
     163,   155,   164,   155,   165,   155,   166,   155,   167,   155,
     168,   155,   169,   170,   170,   170,   171,   171,   172,   172,
     172,   172,   172,   172,   172,   172,   173,   173,   173,   173,
     173,   174,   174,   175,   175,   175,   176,   176,   177,   177,
     177,   177,   177,   177,   178,   178,   179,   180,   180,   181,
     183,   182,   184,   182,   186,   185,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   189,   189,   189,   189,   190,   190,
     190,   191,   191,   192,   192,   193,   193,   193,   194,   194,
     194,   194,   195,   195,   195,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   197,   197,   198,
     198,   198,   198,   199,   199,   199,   199,   199,   199,   200,
     200,   200,   200,   200,   200,   200,   200,   200,   201,   201,
     202,   203,   203,   204,   204,   206,   205,   207,   207,   207,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   213,
     214,   214,   214,   215,   216,   216,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   218,   218,   218,   218,   218,
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   220,   220,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   222,
     222,   223,   223
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,     3,
      11,     2,     0,     3,     2,     2,     2,     6,     8,     2,
       2,     0,     2,     0,     3,     3,     1,     1,     2,     1,
       4,     6,     1,     5,     3,     4,     1,     3,     3,     5,
       1,     1,     1,     1,     1,     2,     1,     4,     8,     7,
       4,     7,     1,     3,     1,     3,     4,     1,     1,     1,
       3,     1,     1,     5,     1,     1,     4,     1,     3,     1,
       2,     2,     0,     0,     9,     4,     2,     4,     4,     6,
       9,     0,     5,     4,     2,     4,     3,     6,     1,     3,
       1,     0,     2,     1,     2,     4,     5,     7,     5,     5,
       6,     8,     7,     0,     3,     0,     3,     3,     1,     0,
       3,     1,     4,     3,     1,     3,     1,     2,     2,     3,
       3,     1,     4,     3,     3,     3,     3,     1,     2,     2,
       3,     3,     1,     3,     3,     3,     3,     1,     4,     3,
       2,     3,     1,     0,     4,     4,     6,     4,     4,     0,
       0,     0,     7,     5,     0,     3,     1,     6,     0,     2,
       1,     4,     6,     6,     7,     9,     0,     9,     0,    11,
       0,     7,     0,    13,     0,    15,     0,    12,     0,    14,
       0,    11,     0,    13,     0,    11,     0,    11,     0,    12,
       0,    14,     0,     3,     2,     0,     3,     1,     3,     5,
       4,     3,     4,     5,     6,     4,     1,     3,     3,     3,
       1,     0,     3,     3,     2,     0,     3,     1,     3,     4,
       4,     4,     3,     5,     3,     1,     1,     3,     1,     3,
       0,     4,     0,     2,     0,     2,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     5,     5,     7,     5,     1,     3,
       2,     6,     4,     1,     2,     2,     4,     5,     1,     1,
       3,     1,     1,     3,     1,     4,     4,     3,     3,     4,
       3,     6,     5,     6,     5,     6,     5,     3,     1,     1,
       3,     5,     6,     3,     3,     3,     3,     3,     3,     4,
       3,     2,     3,     4,     2,     3,     2,     1,     3,     1,
       3,     2,     2,     4,     6,     0,     7,     0,     2,     2,
       3,     1,     3,     0,     3,     1,     1,     3,     4,     4,
       8,     8,     6,     4,     3,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     2,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     4,     4,
       4,     4,     4,     3,     3,     1,     3,     1,     1,     1,
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
#line 224 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2249 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2255 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2261 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2267 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2273 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2279 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2285 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2291 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2297 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2303 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2309 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2315 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2321 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2327 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2333 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2339 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2345 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2351 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2357 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2363 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2369 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2375 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2381 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2387 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2393 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2399 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2423 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2429 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2435 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2441 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2447 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2477 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2489 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2495 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2501 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2507 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2525 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2531 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2537 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2543 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2549 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2555 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2561 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2579 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2585 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2591 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2597 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2603 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2609 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2615 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2621 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2639 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2645 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2651 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2657 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2663 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2669 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2681 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2687 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2705 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2711 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2729 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2735 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2741 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2753 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2783 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2795 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2801 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 96: /* type: CLASSSY class_members ENDSY  */
#line 375 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2813 "pascal.tab.c"
    break;

  case 97: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 376 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 98: /* result_type: IDENT  */
#line 379 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2825 "pascal.tab.c"
    break;

  case 99: /* result_type: IDENT PERIOD IDENT  */
#line 380 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2831 "pascal.tab.c"
    break;

  case 100: /* result_type: spec_ref  */
#line 381 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2837 "pascal.tab.c"
    break;

  case 101: /* class_members: %empty  */
#line 384 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2843 "pascal.tab.c"
    break;

  case 102: /* class_members: class_members class_member  */
#line 385 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2849 "pascal.tab.c"
    break;

  case 103: /* class_member: IDENT  */
#line 388 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2855 "pascal.tab.c"
    break;

  case 104: /* class_member: IDENT SEMICOLON  */
#line 389 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2861 "pascal.tab.c"
    break;

  case 105: /* class_member: id_list COLON type SEMICOLON  */
#line 390 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 106: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 391 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 107: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 392 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 108: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2885 "pascal.tab.c"
    break;

  case 109: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 394 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2891 "pascal.tab.c"
    break;

  case 110: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2897 "pascal.tab.c"
    break;

  case 111: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 396 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2903 "pascal.tab.c"
    break;

  case 112: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 397 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2909 "pascal.tab.c"
    break;

  case 113: /* prop_index_opt: %empty  */
#line 400 "pascal.y"
    { (yyval.node) = NULL; }
#line 2915 "pascal.tab.c"
    break;

  case 114: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 401 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2921 "pascal.tab.c"
    break;

  case 115: /* prop_specs: %empty  */
#line 404 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2927 "pascal.tab.c"
    break;

  case 116: /* prop_specs: prop_specs IDENT IDENT  */
#line 405 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2933 "pascal.tab.c"
    break;

  case 117: /* prop_specs: prop_specs IDENT INTCONST  */
#line 406 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2939 "pascal.tab.c"
    break;

  case 118: /* packed_opt: PACKEDSY  */
#line 409 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2945 "pascal.tab.c"
    break;

  case 119: /* packed_opt: %empty  */
#line 410 "pascal.y"
     { (yyval.node) = NULL; }
#line 2951 "pascal.tab.c"
    break;

  case 120: /* simple_type: LPARENT enum_list RPARENT  */
#line 413 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2957 "pascal.tab.c"
    break;

  case 121: /* simple_type: IDENT  */
#line 414 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2963 "pascal.tab.c"
    break;

  case 122: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 415 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2969 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT PERIOD IDENT  */
#line 416 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2975 "pascal.tab.c"
    break;

  case 124: /* simple_type: spec_ref  */
#line 417 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2981 "pascal.tab.c"
    break;

  case 125: /* simple_type: lo_bound DOTDOT b_expr  */
#line 418 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2987 "pascal.tab.c"
    break;

  case 126: /* lo_bound: lo_term  */
#line 421 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2993 "pascal.tab.c"
    break;

  case 127: /* lo_bound: PLUS lo_term  */
#line 422 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2999 "pascal.tab.c"
    break;

  case 128: /* lo_bound: MINUS lo_term  */
#line 423 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3005 "pascal.tab.c"
    break;

  case 129: /* lo_bound: lo_bound PLUS b_term  */
#line 424 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3011 "pascal.tab.c"
    break;

  case 130: /* lo_bound: lo_bound MINUS b_term  */
#line 425 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3017 "pascal.tab.c"
    break;

  case 131: /* lo_term: scalar_constant  */
#line 428 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3023 "pascal.tab.c"
    break;

  case 132: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 429 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3029 "pascal.tab.c"
    break;

  case 133: /* lo_term: lo_term MUL b_factor  */
#line 430 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3035 "pascal.tab.c"
    break;

  case 134: /* lo_term: lo_term IDIV b_factor  */
#line 431 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term IMOD b_factor  */
#line 432 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3047 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term SHIFTOP b_factor  */
#line 433 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3053 "pascal.tab.c"
    break;

  case 137: /* b_expr: b_term  */
#line 436 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3059 "pascal.tab.c"
    break;

  case 138: /* b_expr: PLUS b_term  */
#line 437 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3065 "pascal.tab.c"
    break;

  case 139: /* b_expr: MINUS b_term  */
#line 438 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3071 "pascal.tab.c"
    break;

  case 140: /* b_expr: b_expr PLUS b_term  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3077 "pascal.tab.c"
    break;

  case 141: /* b_expr: b_expr MINUS b_term  */
#line 440 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 142: /* b_term: b_factor  */
#line 443 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3089 "pascal.tab.c"
    break;

  case 143: /* b_term: b_term MUL b_factor  */
#line 444 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3095 "pascal.tab.c"
    break;

  case 144: /* b_term: b_term IDIV b_factor  */
#line 445 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term IMOD b_factor  */
#line 446 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term SHIFTOP b_factor  */
#line 447 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 147: /* b_factor: scalar_constant  */
#line 450 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3119 "pascal.tab.c"
    break;

  case 148: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 451 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3125 "pascal.tab.c"
    break;

  case 149: /* b_factor: LPARENT b_expr RPARENT  */
#line 452 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3131 "pascal.tab.c"
    break;

  case 150: /* record_body: record_field_list record_case_opt  */
#line 455 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3137 "pascal.tab.c"
    break;

  case 151: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 458 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 152: /* record_field_list: record_field  */
#line 459 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 153: /* $@4: %empty  */
#line 462 "pascal.y"
                  { }
#line 3155 "pascal.tab.c"
    break;

  case 154: /* record_field: id_list COLON $@4 type  */
#line 462 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 155: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 463 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 156: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 464 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3173 "pascal.tab.c"
    break;

  case 157: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 465 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3179 "pascal.tab.c"
    break;

  case 158: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 466 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3185 "pascal.tab.c"
    break;

  case 159: /* record_field: %empty  */
#line 467 "pascal.y"
     { (yyval.node) = NULL; }
#line 3191 "pascal.tab.c"
    break;

  case 160: /* case_arm_mark: %empty  */
#line 470 "pascal.y"
    { }
#line 3197 "pascal.tab.c"
    break;

  case 161: /* case_open: %empty  */
#line 473 "pascal.y"
    { }
#line 3203 "pascal.tab.c"
    break;

  case 162: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 476 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 163: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 477 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3215 "pascal.tab.c"
    break;

  case 164: /* record_case_opt: %empty  */
#line 478 "pascal.y"
     { (yyval.node) = NULL; }
#line 3221 "pascal.tab.c"
    break;

  case 165: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 481 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3227 "pascal.tab.c"
    break;

  case 166: /* record_case_list: record_case_arm  */
#line 482 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3233 "pascal.tab.c"
    break;

  case 167: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 485 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 168: /* record_case_arm: %empty  */
#line 486 "pascal.y"
     { (yyval.node) = NULL; }
#line 3245 "pascal.tab.c"
    break;

  case 169: /* var_decl_list: var_decl_list var_decl  */
#line 489 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3251 "pascal.tab.c"
    break;

  case 170: /* var_decl_list: var_decl  */
#line 490 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 171: /* var_decl: id_list COLON type SEMICOLON  */
#line 493 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 172: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 494 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3269 "pascal.tab.c"
    break;

  case 173: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 495 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3275 "pascal.tab.c"
    break;

  case 174: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 498 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3281 "pascal.tab.c"
    break;

  case 175: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 499 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3287 "pascal.tab.c"
    break;

  case 176: /* $@5: %empty  */
#line 500 "pascal.y"
                                                                               { }
#line 3293 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 500 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3299 "pascal.tab.c"
    break;

  case 178: /* $@6: %empty  */
#line 501 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3305 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 501 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3311 "pascal.tab.c"
    break;

  case 180: /* $@7: %empty  */
#line 502 "pascal.y"
                                        { }
#line 3317 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 502 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3323 "pascal.tab.c"
    break;

  case 182: /* $@8: %empty  */
#line 503 "pascal.y"
                                                                                                           { }
#line 3329 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 503 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3335 "pascal.tab.c"
    break;

  case 184: /* $@9: %empty  */
#line 504 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3341 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 504 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3347 "pascal.tab.c"
    break;

  case 186: /* $@10: %empty  */
#line 505 "pascal.y"
                                                                                                 { }
#line 3353 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 505 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3359 "pascal.tab.c"
    break;

  case 188: /* $@11: %empty  */
#line 506 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 506 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3371 "pascal.tab.c"
    break;

  case 190: /* $@12: %empty  */
#line 507 "pascal.y"
                                                                                            { }
#line 3377 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3383 "pascal.tab.c"
    break;

  case 192: /* $@13: %empty  */
#line 508 "pascal.y"
                                                                                                             { }
#line 3389 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 508 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3397 "pascal.tab.c"
    break;

  case 194: /* $@14: %empty  */
#line 511 "pascal.y"
                                                                                              { }
#line 3403 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 511 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3411 "pascal.tab.c"
    break;

  case 196: /* $@15: %empty  */
#line 514 "pascal.y"
                                                                                             { }
#line 3417 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3425 "pascal.tab.c"
    break;

  case 198: /* $@16: %empty  */
#line 517 "pascal.y"
                                                                                                    { }
#line 3431 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3439 "pascal.tab.c"
    break;

  case 200: /* $@17: %empty  */
#line 520 "pascal.y"
                                                                                                                     { }
#line 3445 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3453 "pascal.tab.c"
    break;

  case 202: /* pv_mark: %empty  */
#line 525 "pascal.y"
    { }
#line 3459 "pascal.tab.c"
    break;

  case 203: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 528 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3465 "pascal.tab.c"
    break;

  case 204: /* parameter_list_opt: LPARENT RPARENT  */
#line 529 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3471 "pascal.tab.c"
    break;

  case 205: /* parameter_list_opt: %empty  */
#line 530 "pascal.y"
     { (yyval.node) = NULL; }
#line 3477 "pascal.tab.c"
    break;

  case 206: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 533 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3483 "pascal.tab.c"
    break;

  case 207: /* parameter_decl_list: parameter_decl  */
#line 534 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3489 "pascal.tab.c"
    break;

  case 208: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 537 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3495 "pascal.tab.c"
    break;

  case 209: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 538 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3501 "pascal.tab.c"
    break;

  case 210: /* parameter_decl: VARSY id_list COLON param_type  */
#line 539 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3507 "pascal.tab.c"
    break;

  case 211: /* parameter_decl: id_list COLON param_type  */
#line 540 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3513 "pascal.tab.c"
    break;

  case 212: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 541 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3519 "pascal.tab.c"
    break;

  case 213: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 542 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3525 "pascal.tab.c"
    break;

  case 214: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 543 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3531 "pascal.tab.c"
    break;

  case 215: /* parameter_decl: IDENT id_list COLON param_type  */
#line 544 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3537 "pascal.tab.c"
    break;

  case 216: /* param_type: IDENT  */
#line 547 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3543 "pascal.tab.c"
    break;

  case 217: /* param_type: IDENT PERIOD IDENT  */
#line 548 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3549 "pascal.tab.c"
    break;

  case 218: /* param_type: ARRAYSY OFSY IDENT  */
#line 549 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3555 "pascal.tab.c"
    break;

  case 219: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 550 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3561 "pascal.tab.c"
    break;

  case 220: /* param_type: spec_ref  */
#line 551 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3567 "pascal.tab.c"
    break;

  case 221: /* routine_directives: %empty  */
#line 554 "pascal.y"
    { (yyval.node) = NULL; }
#line 3573 "pascal.tab.c"
    break;

  case 222: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 555 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3586 "pascal.tab.c"
    break;

  case 223: /* pf_params: LPARENT pf_sections RPARENT  */
#line 565 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3592 "pascal.tab.c"
    break;

  case 224: /* pf_params: LPARENT RPARENT  */
#line 566 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3598 "pascal.tab.c"
    break;

  case 225: /* pf_params: %empty  */
#line 567 "pascal.y"
     { (yyval.node) = NULL; }
#line 3604 "pascal.tab.c"
    break;

  case 226: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 570 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3610 "pascal.tab.c"
    break;

  case 227: /* pf_sections: pf_section  */
#line 571 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3616 "pascal.tab.c"
    break;

  case 228: /* pf_section: id_list COLON param_type  */
#line 574 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3622 "pascal.tab.c"
    break;

  case 229: /* pf_section: VARSY id_list COLON param_type  */
#line 575 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3628 "pascal.tab.c"
    break;

  case 230: /* pf_section: CONSTSY id_list COLON param_type  */
#line 576 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3634 "pascal.tab.c"
    break;

  case 231: /* pf_section: IDENT id_list COLON param_type  */
#line 577 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3640 "pascal.tab.c"
    break;

  case 232: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 578 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3646 "pascal.tab.c"
    break;

  case 233: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 579 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3652 "pascal.tab.c"
    break;

  case 234: /* enum_list: enum_list COMMA enum_member  */
#line 582 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3658 "pascal.tab.c"
    break;

  case 235: /* enum_list: enum_member  */
#line 583 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3664 "pascal.tab.c"
    break;

  case 236: /* enum_member: IDENT  */
#line 586 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3670 "pascal.tab.c"
    break;

  case 237: /* id_list: id_list COMMA IDENT  */
#line 589 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3676 "pascal.tab.c"
    break;

  case 238: /* id_list: IDENT  */
#line 590 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3682 "pascal.tab.c"
    break;

  case 239: /* body: BEGINSY statement_list ENDSY  */
#line 593 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3688 "pascal.tab.c"
    break;

  case 240: /* $@18: %empty  */
#line 596 "pascal.y"
                             { pas_stmt_mark(); }
#line 3694 "pascal.tab.c"
    break;

  case 241: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 596 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3700 "pascal.tab.c"
    break;

  case 242: /* $@19: %empty  */
#line 597 "pascal.y"
     { pas_stmt_mark(); }
#line 3706 "pascal.tab.c"
    break;

  case 243: /* statement_list: $@19 statement  */
#line 597 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3712 "pascal.tab.c"
    break;

  case 244: /* $@20: %empty  */
#line 600 "pascal.y"
    { pas_stmt_mark(); }
#line 3718 "pascal.tab.c"
    break;

  case 245: /* body_stmt: $@20 statement  */
#line 600 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3724 "pascal.tab.c"
    break;

  case 246: /* statement: statement_no_label  */
#line 603 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3730 "pascal.tab.c"
    break;

  case 247: /* statement: INTCONST COLON statement_no_label  */
#line 604 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3736 "pascal.tab.c"
    break;

  case 248: /* statement: IDENT COLON statement_no_label  */
#line 605 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3742 "pascal.tab.c"
    break;

  case 249: /* statement_no_label: assignment  */
#line 608 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3748 "pascal.tab.c"
    break;

  case 250: /* statement_no_label: call  */
#line 609 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3754 "pascal.tab.c"
    break;

  case 251: /* statement_no_label: compound_statement  */
#line 610 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3760 "pascal.tab.c"
    break;

  case 252: /* statement_no_label: goto_statement  */
#line 611 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3766 "pascal.tab.c"
    break;

  case 253: /* statement_no_label: if_statement  */
#line 612 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3772 "pascal.tab.c"
    break;

  case 254: /* statement_no_label: case_statement  */
#line 613 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3778 "pascal.tab.c"
    break;

  case 255: /* statement_no_label: while_statement  */
#line 614 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3784 "pascal.tab.c"
    break;

  case 256: /* statement_no_label: repeat_statement  */
#line 615 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3790 "pascal.tab.c"
    break;

  case 257: /* statement_no_label: for_statement  */
#line 616 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3796 "pascal.tab.c"
    break;

  case 258: /* statement_no_label: with_statement  */
#line 617 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3802 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: try_statement  */
#line 618 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3808 "pascal.tab.c"
    break;

  case 260: /* statement_no_label: raise_statement  */
#line 619 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3814 "pascal.tab.c"
    break;

  case 261: /* statement_no_label: inherited_call  */
#line 620 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3820 "pascal.tab.c"
    break;

  case 262: /* statement_no_label: INHERITEDSY  */
#line 621 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3826 "pascal.tab.c"
    break;

  case 263: /* statement_no_label: %empty  */
#line 622 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3832 "pascal.tab.c"
    break;

  case 264: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 625 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3838 "pascal.tab.c"
    break;

  case 265: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 626 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3844 "pascal.tab.c"
    break;

  case 266: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 627 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3850 "pascal.tab.c"
    break;

  case 267: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 628 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3856 "pascal.tab.c"
    break;

  case 268: /* handler_list: handler  */
#line 631 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3862 "pascal.tab.c"
    break;

  case 269: /* handler_list: handler_list SEMICOLON handler  */
#line 632 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3868 "pascal.tab.c"
    break;

  case 270: /* handler_list: handler_list SEMICOLON  */
#line 633 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3874 "pascal.tab.c"
    break;

  case 271: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 636 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3880 "pascal.tab.c"
    break;

  case 272: /* handler: ONSY IDENT DOSY body_stmt  */
#line 637 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3886 "pascal.tab.c"
    break;

  case 273: /* raise_statement: RAISESY  */
#line 640 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3892 "pascal.tab.c"
    break;

  case 274: /* raise_statement: RAISESY expression  */
#line 641 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 275: /* inherited_call: INHERITEDSY IDENT  */
#line 644 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3904 "pascal.tab.c"
    break;

  case 276: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 645 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3910 "pascal.tab.c"
    break;

  case 277: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 646 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3916 "pascal.tab.c"
    break;

  case 278: /* call: IDENT  */
#line 649 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3922 "pascal.tab.c"
    break;

  case 279: /* call: spec_expr  */
#line 650 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3928 "pascal.tab.c"
    break;

  case 280: /* call: selector PERIOD IDENT  */
#line 651 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3934 "pascal.tab.c"
    break;

  case 281: /* call: call_with_args  */
#line 652 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3940 "pascal.tab.c"
    break;

  case 282: /* recv_primary: STRINGCONST  */
#line 655 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3946 "pascal.tab.c"
    break;

  case 283: /* recv_primary: LPARENT expression RPARENT  */
#line 656 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3952 "pascal.tab.c"
    break;

  case 284: /* recv_primary: spec_expr  */
#line 657 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3958 "pascal.tab.c"
    break;

  case 285: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 660 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3964 "pascal.tab.c"
    break;

  case 286: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 661 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3970 "pascal.tab.c"
    break;

  case 287: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 662 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3976 "pascal.tab.c"
    break;

  case 288: /* call_with_args: IDENT LPARENT RPARENT  */
#line 663 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3982 "pascal.tab.c"
    break;

  case 289: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 664 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3988 "pascal.tab.c"
    break;

  case 290: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 665 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3994 "pascal.tab.c"
    break;

  case 291: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 666 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4000 "pascal.tab.c"
    break;

  case 292: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 667 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4006 "pascal.tab.c"
    break;

  case 293: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 668 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4012 "pascal.tab.c"
    break;

  case 294: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 669 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4018 "pascal.tab.c"
    break;

  case 295: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 670 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4024 "pascal.tab.c"
    break;

  case 296: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 671 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4030 "pascal.tab.c"
    break;

  case 297: /* argument_list: argument_list COMMA argument  */
#line 674 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4036 "pascal.tab.c"
    break;

  case 298: /* argument_list: argument  */
#line 675 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4042 "pascal.tab.c"
    break;

  case 299: /* argument: expression  */
#line 678 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4048 "pascal.tab.c"
    break;

  case 300: /* argument: expression COLON expression  */
#line 679 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4054 "pascal.tab.c"
    break;

  case 301: /* argument: expression COLON expression COLON expression  */
#line 680 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4060 "pascal.tab.c"
    break;

  case 302: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 681 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4066 "pascal.tab.c"
    break;

  case 303: /* assignment: selector BECOMES expression  */
#line 684 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4072 "pascal.tab.c"
    break;

  case 304: /* assignment: selector ADDBECOMES expression  */
#line 685 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4078 "pascal.tab.c"
    break;

  case 305: /* assignment: selector SUBBECOMES expression  */
#line 686 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4084 "pascal.tab.c"
    break;

  case 306: /* assignment: selector MULBECOMES expression  */
#line 687 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4090 "pascal.tab.c"
    break;

  case 307: /* assignment: selector DIVBECOMES expression  */
#line 688 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4096 "pascal.tab.c"
    break;

  case 308: /* assignment: call_with_args BECOMES expression  */
#line 689 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4102 "pascal.tab.c"
    break;

  case 309: /* selector: selector LBRACK expression_list RBRACK  */
#line 692 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4108 "pascal.tab.c"
    break;

  case 310: /* selector: selector PERIOD IDENT  */
#line 693 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4114 "pascal.tab.c"
    break;

  case 311: /* selector: selector ARROW  */
#line 694 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4120 "pascal.tab.c"
    break;

  case 312: /* selector: call_with_args PERIOD IDENT  */
#line 695 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 313: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 696 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4132 "pascal.tab.c"
    break;

  case 314: /* selector: call_with_args ARROW  */
#line 697 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4138 "pascal.tab.c"
    break;

  case 315: /* selector: recv_primary PERIOD IDENT  */
#line 698 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4144 "pascal.tab.c"
    break;

  case 316: /* selector: recv_primary ARROW  */
#line 699 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4150 "pascal.tab.c"
    break;

  case 317: /* selector: IDENT  */
#line 700 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4156 "pascal.tab.c"
    break;

  case 318: /* expression_list: expression_list COMMA expression  */
#line 703 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4162 "pascal.tab.c"
    break;

  case 319: /* expression_list: expression  */
#line 704 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4168 "pascal.tab.c"
    break;

  case 320: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 707 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4174 "pascal.tab.c"
    break;

  case 321: /* goto_statement: GOTOSY INTCONST  */
#line 710 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4180 "pascal.tab.c"
    break;

  case 322: /* goto_statement: GOTOSY IDENT  */
#line 711 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4186 "pascal.tab.c"
    break;

  case 323: /* if_statement: IFSY expression THENSY body_stmt  */
#line 714 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4192 "pascal.tab.c"
    break;

  case 324: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 715 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4198 "pascal.tab.c"
    break;

  case 325: /* $@21: %empty  */
#line 718 "pascal.y"
                           { }
#line 4204 "pascal.tab.c"
    break;

  case 326: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 718 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4210 "pascal.tab.c"
    break;

  case 327: /* case_tail: %empty  */
#line 721 "pascal.y"
    { (yyval.node) = NULL; }
#line 4216 "pascal.tab.c"
    break;

  case 328: /* case_tail: ELSESY statement_list  */
#line 722 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4222 "pascal.tab.c"
    break;

  case 329: /* case_tail: OTHERWISESY statement_list  */
#line 723 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4228 "pascal.tab.c"
    break;

  case 330: /* case_list: case_list SEMICOLON case_elem  */
#line 726 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4234 "pascal.tab.c"
    break;

  case 331: /* case_list: case_elem  */
#line 727 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4240 "pascal.tab.c"
    break;

  case 332: /* case_elem: constant_list COLON body_stmt  */
#line 730 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4246 "pascal.tab.c"
    break;

  case 333: /* case_elem: %empty  */
#line 731 "pascal.y"
     { (yyval.node) = NULL; }
#line 4252 "pascal.tab.c"
    break;

  case 334: /* constant_list: constant_list COMMA case_label  */
#line 734 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4258 "pascal.tab.c"
    break;

  case 335: /* constant_list: case_label  */
#line 735 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4264 "pascal.tab.c"
    break;

  case 336: /* case_label: b_expr  */
#line 738 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4270 "pascal.tab.c"
    break;

  case 337: /* case_label: b_expr DOTDOT b_expr  */
#line 739 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4276 "pascal.tab.c"
    break;

  case 338: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 742 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4282 "pascal.tab.c"
    break;

  case 339: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 745 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4288 "pascal.tab.c"
    break;

  case 340: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 748 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4294 "pascal.tab.c"
    break;

  case 341: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 749 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4300 "pascal.tab.c"
    break;

  case 342: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 750 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4306 "pascal.tab.c"
    break;

  case 343: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 753 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4312 "pascal.tab.c"
    break;

  case 344: /* with_open: with_open COMMA selector  */
#line 756 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4318 "pascal.tab.c"
    break;

  case 345: /* with_open: selector  */
#line 757 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 346: /* expression: simple_expression  */
#line 764 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4330 "pascal.tab.c"
    break;

  case 347: /* expression: expression INOP simple_expression  */
#line 765 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4336 "pascal.tab.c"
    break;

  case 348: /* expression: expression LTOP simple_expression  */
#line 766 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4342 "pascal.tab.c"
    break;

  case 349: /* expression: expression LEOP simple_expression  */
#line 767 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4348 "pascal.tab.c"
    break;

  case 350: /* expression: expression GTOP simple_expression  */
#line 768 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4354 "pascal.tab.c"
    break;

  case 351: /* expression: expression GEOP simple_expression  */
#line 769 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 352: /* expression: expression NEOP simple_expression  */
#line 770 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4366 "pascal.tab.c"
    break;

  case 353: /* expression: expression EQOP simple_expression  */
#line 771 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4372 "pascal.tab.c"
    break;

  case 354: /* expression: expression ISOP simple_expression  */
#line 772 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4378 "pascal.tab.c"
    break;

  case 355: /* simple_expression: term  */
#line 775 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4384 "pascal.tab.c"
    break;

  case 356: /* simple_expression: PLUS term  */
#line 776 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4390 "pascal.tab.c"
    break;

  case 357: /* simple_expression: MINUS term  */
#line 777 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4396 "pascal.tab.c"
    break;

  case 358: /* simple_expression: simple_expression PLUS term  */
#line 778 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4402 "pascal.tab.c"
    break;

  case 359: /* simple_expression: simple_expression MINUS term  */
#line 779 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4408 "pascal.tab.c"
    break;

  case 360: /* simple_expression: simple_expression OROP term  */
#line 780 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4414 "pascal.tab.c"
    break;

  case 361: /* simple_expression: simple_expression XOROP term  */
#line 781 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4420 "pascal.tab.c"
    break;

  case 362: /* term: power  */
#line 784 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4426 "pascal.tab.c"
    break;

  case 363: /* term: term MUL power  */
#line 785 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4432 "pascal.tab.c"
    break;

  case 364: /* term: term RDIV power  */
#line 786 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4438 "pascal.tab.c"
    break;

  case 365: /* term: term IDIV power  */
#line 787 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4444 "pascal.tab.c"
    break;

  case 366: /* term: term IMOD power  */
#line 788 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4450 "pascal.tab.c"
    break;

  case 367: /* term: term ANDOP power  */
#line 789 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4456 "pascal.tab.c"
    break;

  case 368: /* term: term MUL MINUS power  */
#line 790 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4462 "pascal.tab.c"
    break;

  case 369: /* term: term RDIV MINUS power  */
#line 791 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4468 "pascal.tab.c"
    break;

  case 370: /* term: term IDIV MINUS power  */
#line 792 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4474 "pascal.tab.c"
    break;

  case 371: /* term: term IMOD MINUS power  */
#line 793 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4480 "pascal.tab.c"
    break;

  case 372: /* term: term ANDOP MINUS power  */
#line 794 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4486 "pascal.tab.c"
    break;

  case 373: /* term: term SHIFTOP power  */
#line 795 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4492 "pascal.tab.c"
    break;

  case 374: /* term: term ASOP power  */
#line 796 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4498 "pascal.tab.c"
    break;

  case 375: /* power: factor  */
#line 799 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4504 "pascal.tab.c"
    break;

  case 376: /* power: power POWOP factor  */
#line 800 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4510 "pascal.tab.c"
    break;

  case 377: /* factor: selector  */
#line 803 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4516 "pascal.tab.c"
    break;

  case 378: /* factor: call_with_args  */
#line 804 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4522 "pascal.tab.c"
    break;

  case 379: /* factor: INTCONST  */
#line 805 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4528 "pascal.tab.c"
    break;

  case 380: /* factor: REALCONST  */
#line 806 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4534 "pascal.tab.c"
    break;

  case 381: /* factor: STRINGCONST  */
#line 807 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4540 "pascal.tab.c"
    break;

  case 382: /* factor: CHARCODE  */
#line 808 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4546 "pascal.tab.c"
    break;

  case 383: /* factor: LPARENT expression RPARENT  */
#line 809 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4552 "pascal.tab.c"
    break;

  case 384: /* factor: inherited_call  */
#line 810 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4558 "pascal.tab.c"
    break;

  case 385: /* factor: NOTSY factor  */
#line 811 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4564 "pascal.tab.c"
    break;

  case 386: /* factor: ATSIGN factor  */
#line 812 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4570 "pascal.tab.c"
    break;

  case 387: /* factor: LBRACK RBRACK  */
#line 813 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4576 "pascal.tab.c"
    break;

  case 388: /* factor: LBRACK set_member_list RBRACK  */
#line 814 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4582 "pascal.tab.c"
    break;

  case 389: /* set_member_list: set_member  */
#line 817 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4588 "pascal.tab.c"
    break;

  case 390: /* set_member_list: set_member_list COMMA set_member  */
#line 818 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4594 "pascal.tab.c"
    break;

  case 391: /* set_member: expression  */
#line 821 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4600 "pascal.tab.c"
    break;

  case 392: /* set_member: expression DOTDOT expression  */
#line 822 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4606 "pascal.tab.c"
    break;


#line 4610 "pascal.tab.c"

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

#line 828 "pascal.y"

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
