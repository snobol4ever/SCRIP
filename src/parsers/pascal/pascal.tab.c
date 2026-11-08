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
#define YYLAST   1578

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  383
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  884

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
     455,   458,   459,   462,   462,   463,   466,   469,   472,   473,
     474,   477,   478,   481,   482,   485,   486,   489,   490,   491,
     494,   495,   496,   496,   497,   497,   498,   498,   499,   499,
     500,   500,   501,   501,   502,   502,   503,   503,   504,   504,
     507,   507,   510,   510,   513,   513,   516,   516,   521,   524,
     525,   526,   529,   530,   533,   534,   535,   536,   537,   538,
     539,   540,   543,   544,   545,   546,   549,   550,   560,   561,
     562,   565,   566,   569,   570,   571,   572,   573,   576,   577,
     580,   583,   584,   587,   590,   590,   591,   591,   594,   594,
     597,   598,   599,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   619,   620,
     621,   622,   625,   626,   627,   630,   631,   634,   635,   638,
     639,   640,   643,   644,   645,   646,   649,   650,   651,   654,
     655,   656,   657,   658,   659,   660,   661,   662,   663,   666,
     667,   670,   671,   672,   675,   676,   677,   678,   679,   680,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   694,
     695,   698,   701,   702,   705,   706,   709,   709,   712,   713,
     714,   717,   718,   721,   722,   725,   726,   729,   730,   733,
     736,   739,   740,   741,   744,   747,   748,   755,   756,   757,
     758,   759,   760,   761,   762,   763,   766,   767,   768,   769,
     770,   771,   772,   775,   776,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   787,   790,   791,   794,   795,
     796,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     808,   809,   812,   813
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

#define YYPACT_NINF (-539)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-309)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     272,   -31,    -3,   253,  -539,   282,   883,  -539,  -539,   328,
    -539,    25,  -539,  -539,    78,   211,    -3,   235,   255,   255,
     274,  -539,   -39,   347,   277,   283,   350,  -539,  -539,  -539,
     293,   255,   383,   435,   377,  -539,  -539,    22,   421,   211,
    -539,   117,   412,    92,  -539,   255,  -539,     0,   129,    98,
      42,   594,   501,   414,   -39,  -539,   472,   497,   481,   535,
     529,   541,  -539,   471,  -539,  -539,  -539,  -539,   416,  1177,
     905,  -539,  -539,    -3,   255,   544,   568,  -539,   561,   905,
    -539,   255,   569,    36,  -539,  -539,   427,   570,  -539,  1177,
     248,  1177,  -539,  1177,  1177,   589,  -539,   951,  -539,  1177,
     579,   580,  -539,   256,  -539,  -539,  -539,  -539,  -539,  -539,
    -539,   513,   408,  -539,   658,  -539,  -539,  -539,  -539,  -539,
    -539,  -539,  -539,   255,   929,   547,  -539,   603,   606,   602,
     609,   638,   649,  -539,   620,   850,  -539,  -539,  1378,  1378,
    1378,  1011,  1177,  1378,  -539,  -539,  -539,   562,   142,   579,
     644,  -539,   581,   598,    37,   227,   379,   637,  -539,   654,
    -539,   654,   599,   599,    68,   660,  -539,  -539,  -539,  -539,
    1222,   661,  -539,  -539,  -539,  -539,   698,   320,  -539,   306,
     244,   412,   246,  -539,    67,   732,  -539,    52,   371,  -539,
    -539,   683,   594,  -539,  -539,   490,   268,   174,   581,   598,
     204,   999,   210,  1425,  1442,   798,   798,  1039,  1407,    35,
    1497,   691,   708,  1065,   685,  -539,  1177,   697,  -539,  1177,
    1177,   701,  -539,  1177,  1177,  1177,  1177,  1177,   372,   137,
     773,   255,   710,   715,  -539,  -539,   255,   255,  -539,   211,
      -3,  -539,   716,   255,   717,   -39,  -539,  -539,   379,   379,
    -539,  1460,   494,  -539,  1479,  -539,   719,  -539,  1177,  1177,
    1177,  1177,  1177,  1177,  1177,  1177,  1378,  1378,  1378,  1378,
    1268,  1294,  1310,  1336,  1352,  1378,  1378,  1378,   173,   759,
     728,   734,   244,   244,   108,  -539,  -539,   473,  -539,  1407,
     720,  1498,  1498,   156,   723,  1218,   208,   772,   774,  -539,
     434,   434,  1498,   434,   434,   434,   434,  -539,   568,   255,
     729,   255,   730,  -539,   255,    41,  -539,   153,   795,  -539,
    1218,   733,  -539,   568,  -539,   264,  -539,  1177,  1177,  1177,
    -539,  -539,   248,  -539,  -539,  -539,  -539,   969,  -539,  -539,
    -539,   486,  -539,   315,   -25,  -539,   739,  -539,  1081,  1407,
    -539,   500,   744,  1497,   756,   546,  1497,  1497,  1495,   549,
    1497,  1497,  1497,  1497,   768,  -539,   156,  -539,   381,  -539,
    -539,   568,   568,   418,   433,   211,   144,   818,  -539,   255,
    -539,   -39,  1177,  1177,  -539,   574,   760,   227,   227,   227,
     227,   227,   227,   227,   227,   379,   379,   379,   379,  1378,
     637,  1378,   637,  1378,   637,  1378,   637,  1378,   637,   637,
     637,  -539,   255,   755,   255,   758,  -539,    60,  -539,   592,
     763,   264,   660,  -539,   454,  -539,   434,   434,  1498,   776,
    -539,   -11,   333,  -539,   373,   108,   517,  -539,  -539,   765,
    -539,   766,   140,   368,   769,   770,   771,  -539,   631,  1252,
     831,  1497,   905,  1407,   905,  1407,   255,   333,   333,   447,
    -539,  -539,  -539,  -539,   568,   843,   634,   654,   642,   654,
     684,   152,  -539,    15,   844,   775,   847,   851,   568,   799,
     859,    79,  -539,   860,   895,   367,  1497,  -539,   594,  -539,
     598,  1498,   825,  1177,  -539,  1177,  1407,  -539,   791,   267,
     230,  -539,   288,  -539,   519,   464,  -539,  1119,  1135,  1177,
    -539,  1161,  -539,   905,   525,   819,   568,   568,   866,   872,
    -539,  -539,  -539,  -539,  -539,   863,   568,   568,  1497,  -539,
     637,   637,   637,   637,   637,   690,   654,   702,   654,   178,
    -539,    15,  -539,   852,  -539,  -539,   333,   333,   410,  1498,
     434,   434,  -539,   434,   434,   434,   434,  -539,   156,  -539,
    -539,  -539,  -539,   803,   812,  -539,  -539,   832,   905,    69,
     841,    61,  1479,  -539,  -539,   553,  -539,  -539,   870,   252,
    -539,   707,   896,  -539,    15,  -539,    15,   842,    15,  -539,
     876,   171,  -539,   855,  -539,   900,  -539,  -539,  -539,   856,
     264,  -539,   836,   912,  1177,  1177,  -539,  -539,   336,   228,
    -539,   711,  -539,  -539,  -539,  1019,  -539,   191,  -539,   739,
    -539,  -539,  -539,  -539,  -539,  -539,   527,  -539,   531,  1497,
    -539,   533,   918,   919,   905,   921,   867,  -539,  -539,   568,
     568,   923,   923,  -539,   899,   925,   873,    15,  -539,    15,
     874,  -539,  -539,   862,  -539,   432,   333,   333,  -539,  -539,
    -539,  -539,  -539,   807,   568,   568,  -539,  -539,   568,   568,
     152,   875,   934,  1218,  1218,    83,  -539,  1407,   914,  -539,
     255,   864,  -539,  -539,  -539,   775,   897,  -539,   264,  -539,
     877,   878,  1177,  -539,   944,   264,   946,  -539,   947,   775,
     518,   682,  -539,  1498,  1498,  -539,  -539,   927,  1498,  -539,
    -539,  1177,  -539,   882,  -539,   305,  -539,  -539,  -539,  -539,
    -539,   956,  -539,   264,   775,   775,   957,   904,   923,   908,
    -539,   264,  -539,  -539,   264,  -539,  -539,  -539,   965,   926,
     568,   568,   982,   983,    31,   156,  -539,  -539,  -539,   545,
    -539,   931,   911,  -539,  -539,   203,   905,   775,  -539,  1177,
    -539,  -539,  -539,  1497,  -539,   987,  -539,  -539,  -539,  -539,
    -539,   447,  -539,   923,   923,  -539,  -539,  -539,  1497,  -539,
     977,  -539,  -539,   775,   990,  -539,  -539,  -539,   264,  -539,
     992,  -539,  -539,   264,   993,   940,  -539,  -539,  -539,  -539,
    1218,  -539,  1218,   980,  -539,  -539,   938,  -539,  -539,  1012,
    1497,  -539,   775,  1013,  -539,  -539,  -539,  -539,  -539,  1020,
    1021,   775,  1024,  -539,  1027,  -539,   264,    45,  -539,  -539,
     905,   905,  1498,   988,  1029,  -539,   775,  -539,  -539,  -539,
    1030,   775,  -539,  -539,  -539,  -539,  -539,  1031,  -539,   458,
    -539,  -539,  1037,  -539,   712,  -539,  -539,  -539,  1038,  -539,
    -539,  1040,   775,  -539,  -539,  -539,  1498,   960,  1498,  1048,
    -539,  1050,  -539,  -539,  -539,  -539,  1037,  -539,  -539,  1051,
     255,  -539,   979,  -539
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   198,   232,    16,   166,     0,     0,   198,
       0,   257,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     7,     0,    12,    22,    65,    13,     0,     0,
     119,    38,    18,     0,     0,     0,   201,   165,     0,   119,
      19,     0,     0,   201,   234,   233,     0,     0,   236,     0,
       0,     0,   236,     0,     0,     0,   276,   272,   236,   267,
     256,     0,    68,   273,    67,   237,   240,   253,   254,   255,
     244,     0,   275,   243,     0,   245,   246,   247,   248,   249,
     250,   251,   252,     0,   119,     0,    55,     0,     0,     0,
       0,     0,     0,     8,     0,     0,    34,    35,     0,     0,
       0,     0,     0,     0,   370,   373,   371,   372,   308,     0,
     278,   375,   369,   368,     0,   337,   346,   353,   366,   220,
     118,   220,     0,     0,     0,     0,    51,    54,    52,    53,
     121,   101,   131,    75,    74,   124,     0,     0,    79,     0,
     126,    63,     0,   198,     0,     0,   231,     0,     0,   198,
     176,     0,   257,   312,   313,     0,     0,     0,     0,   336,
       0,     0,     0,     0,     0,   257,   257,     0,     0,     0,
     268,   269,     0,     0,     0,   307,     0,     0,   305,     0,
       0,     0,   302,     0,     0,     0,     0,     0,     0,   101,
       0,     0,     0,     0,   198,   198,     0,     0,     4,     0,
       0,    12,     0,     0,     0,     0,    21,   376,   347,   348,
     378,   382,     0,   380,     0,   377,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,    50,   127,   128,    80,    81,   230,     0,   229,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,    91,
       0,     0,     0,     0,     0,     0,     0,   198,   201,     0,
       0,     0,     0,   200,   232,     0,   203,     0,   216,   167,
       0,     0,   198,   201,    12,     0,   235,     0,     0,     0,
     238,   238,     0,   316,   311,   238,   277,   272,   241,   242,
     282,     0,   290,   291,     0,    77,   236,   236,     0,     0,
     281,     0,   306,   299,   303,     0,   310,   294,   274,     0,
     295,   296,   297,   298,     0,    60,     0,    57,     0,   198,
     198,   201,   201,     0,     0,    24,     0,    31,   198,    26,
     198,    25,     0,     0,   379,   374,   301,   338,   339,   340,
     341,   342,   343,   344,   345,   351,   349,   350,   352,     0,
     356,     0,   357,     0,   358,     0,   354,     0,   355,   364,
     365,   367,     0,     0,     0,     0,   219,     0,   222,     0,
       0,     0,     0,   120,     0,   123,     0,     0,     0,    50,
     147,     0,   137,   142,     0,    71,     0,    69,    72,     0,
      96,     0,   103,     0,     0,     0,     0,   102,     0,     0,
       0,    42,   119,     0,   119,     0,   155,   129,   130,   125,
     134,   135,   133,   136,   201,     0,     0,   220,     0,   220,
       0,     0,   199,     0,     0,   172,     0,     0,   201,     0,
       0,    98,   100,     0,     0,     0,   330,   329,   257,   334,
     335,   324,   314,     0,   279,     0,     0,    66,     0,     0,
       0,   262,     0,   270,     0,     0,   280,     0,     0,     0,
     304,     0,   300,   119,     0,     0,   201,   201,     0,     0,
     198,   198,    23,   236,   236,    33,   201,   201,   383,   381,
     361,   362,   363,   359,   360,     0,   220,     0,   220,     0,
     218,     0,    87,    88,   228,    76,   138,   139,     0,     0,
       0,     0,   122,     0,     0,     0,     0,   132,     0,   101,
     198,   198,   104,     0,     0,   198,   198,   113,   119,   308,
       0,     0,    42,    41,    85,     0,    95,    93,     0,   160,
     152,     0,     0,   216,     0,   204,     0,     0,     0,   202,
       0,   212,   215,   207,   170,     0,    12,   168,   169,     0,
       0,   177,     0,   216,     0,     0,   238,   239,   327,   318,
     322,     0,   326,   238,   289,   292,    78,     0,   258,   264,
     259,   236,   261,   271,    73,   288,     0,   286,     0,   309,
     284,     0,     0,   101,   119,     0,     0,   216,   216,   201,
     201,    29,    30,   236,     0,     0,     0,     0,   226,     0,
       0,   221,   223,     0,   149,     0,   140,   141,   144,   145,
     143,   146,    70,     0,   201,   201,   198,   198,   201,   201,
       0,     0,     0,     0,     0,     0,    44,     0,     0,    92,
     155,     0,   150,   153,   216,   186,   208,   206,     0,   211,
       0,     0,     0,   217,     0,     0,     0,    99,     0,   174,
       0,     0,   333,     0,   324,   236,   236,     0,     0,   238,
     315,     0,   238,     0,   263,     0,   287,   285,   283,    59,
      61,     0,   216,     0,   190,   192,     0,     0,    32,     0,
      27,     0,   225,   224,     0,    89,   148,    97,     0,     0,
     201,   201,     0,     0,     0,     0,   105,    48,    46,     0,
      45,     0,     0,    82,   151,     0,   119,   182,    12,     0,
     205,   214,   213,   209,   173,     0,   216,   171,    12,   238,
     238,   328,   321,   320,   319,   317,   325,   323,   293,   266,
       0,   260,    58,   194,     0,    12,    12,   216,     0,    20,
       0,   227,   106,     0,     0,     0,   108,   109,   114,   115,
       0,    43,     0,     0,    83,   157,     0,   154,    12,     0,
     210,   216,   188,     0,   332,   331,   238,    12,   216,     0,
       0,   178,     0,    28,     0,   110,     0,     0,    47,    49,
     119,   119,   164,     0,     0,   187,   184,    12,   175,   265,
       0,   196,   191,   193,    12,   216,   107,     0,   112,     0,
      90,    84,   159,   162,     0,   157,   183,    12,     0,   195,
      12,     0,   180,   111,   117,   116,   164,     0,   164,     0,
     189,     0,   179,    12,   161,   156,   158,   185,   197,     0,
     155,   181,     0,   163
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -539,  -539,  -539,  -539,  -539,   -63,   816,  -539,  -539,  -539,
    -539,  -539,  -539,  -539,   820,   -36,  -318,  -539,  -539,   -55,
     813,   -44,   822,    51,   -47,   -37,   694,  -538,   -41,   180,
    -219,   -73,  -539,  -539,  -539,  -238,  -531,  -539,  -539,  -539,
    -539,  -197,  -539,   271,  -243,  -237,  -208,   183,  -539,   396,
    -539,  -539,   222,  -539,   213,   223,   845,   -27,  -539,  -539,
    -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,
    -539,  -539,   -42,    -1,   420,   624,  -510,  -502,  -135,  -539,
     557,  -539,   675,   -19,  -539,   -79,  -539,  -539,  -327,  -539,
    -187,   357,  -539,  -539,   480,  -539,   -26,  -539,  -539,   -34,
    -178,   614,  -539,   -29,   879,  -539,  -539,  -539,  -539,  -539,
    -539,  -539,   405,  -475,   402,  -539,  -539,  -539,  -539,  -539,
     487,  1173,   -72,  -160,  -104,  -539,   735
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   135,
     246,   525,   644,    37,    39,    40,   450,   749,   571,   430,
      54,    55,    41,    42,   102,   150,   436,   437,   104,   175,
     344,   176,   804,   831,   456,   483,   294,   447,   671,   827,
     177,   178,   179,   180,   608,   432,   433,   578,   579,   580,
     756,   880,   832,   682,   852,   853,    45,    46,    28,   596,
     768,   324,   844,   873,   808,   857,   758,   837,   785,   786,
     817,   860,    76,   185,   315,   316,   593,   475,   279,   417,
     418,   287,   288,    47,    29,    50,   192,    51,   487,   488,
     105,   106,   107,   500,   501,   108,   151,   110,   111,   152,
     341,   342,   113,   153,   355,   115,   116,   117,   118,   491,
     707,   609,   610,   854,   612,   119,   120,   121,   122,   200,
     343,   155,   156,   157,   158,   252,   253
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   134,   476,    71,   489,   326,   187,    83,   492,   196,
     126,   345,    63,   202,   103,   172,   611,   112,    77,   209,
     662,   590,   114,   173,   172,   109,   280,    67,   663,   174,
      33,   652,   173,   496,   247,   351,   471,    52,   174,   255,
      84,   190,   257,   550,   551,     8,   471,    84,   431,   434,
     848,   230,   552,    11,    53,   182,   198,   319,    78,   459,
      79,   199,   188,   457,   458,   539,   675,   248,   249,   172,
     424,   497,    85,    10,   686,   309,   687,   173,   689,   258,
      68,   685,   191,   174,    34,   259,   260,   261,   262,   263,
     264,   591,   345,   310,   798,   460,   461,   462,   463,   311,
     184,   699,   663,   312,   228,   320,   472,   172,   172,   101,
     400,   402,   404,   406,   408,   409,   410,   173,   265,   346,
     347,   849,    72,   174,   181,   540,   676,   289,   321,   673,
     505,   570,   313,   207,    80,   724,   725,   732,   602,   733,
      74,   308,   365,   314,   284,   562,    81,   323,   750,   522,
      35,    75,   345,   172,    36,   103,   289,    82,   112,   751,
     309,   173,   101,   114,   208,   317,   109,   174,   103,   103,
     504,   112,   112,   411,   208,    73,   114,   114,   310,   109,
     109,   412,   757,   543,   311,   548,   412,    78,   312,   546,
     547,   330,   371,   372,   395,   396,   397,   398,  -232,   413,
    -232,   366,    73,   208,   413,   414,   207,   799,   712,   415,
     414,    78,   368,   473,   415,    84,   258,   373,   374,   289,
     783,   331,   259,   260,   261,   262,   263,   264,   314,   611,
     691,   805,   435,   704,   172,   619,   452,   208,   416,   530,
     334,   531,   173,   532,   705,   533,   173,   534,   174,    44,
     101,   713,   174,    12,    44,   265,   575,   680,   577,   419,
     620,   480,   332,   806,   812,   464,   208,   499,   502,   706,
     453,   621,    84,    84,   266,   448,     1,   681,   173,   702,
     478,   267,   268,     2,   174,   821,   710,    38,   303,   304,
     466,   329,   468,    84,   172,   470,   307,   618,   198,   616,
     305,   607,   173,   490,    78,   269,   655,   465,   174,   836,
      84,    43,    94,   656,   657,  -278,   841,  -278,   622,   173,
     213,   306,   479,    96,   148,   174,   296,   516,   517,   626,
     628,    44,   585,   631,   587,   781,   526,   126,   527,    71,
     481,    13,   101,   862,   285,   658,   659,   660,   661,   297,
      49,   298,    77,    58,   299,   747,   748,   258,   101,    59,
     300,   301,   696,   259,   260,   261,   262,   263,   264,    62,
     518,   519,   302,    56,   173,   495,    60,   553,   554,   574,
     174,   576,   777,    57,   606,   779,    61,    30,    64,   555,
     550,   551,    31,   535,   563,   537,   265,   172,   172,   172,
     172,   648,   703,   650,   564,   173,   173,   173,   173,   258,
     556,   174,   174,   174,   174,   259,   260,   261,   262,   263,
     264,   322,   364,   270,   271,   272,   173,   550,   551,    78,
      78,   515,   174,   282,   283,   273,   274,   581,   557,    78,
     632,   172,   814,   815,   641,   642,   216,    65,   265,   173,
     760,   103,   317,    66,   112,   174,   275,   765,   172,   114,
     771,   276,   109,   582,   550,   551,   173,   217,   520,   218,
     219,    34,   174,   438,    69,   654,    78,   599,   639,   640,
     752,    70,   828,   521,   829,   784,   550,   551,   136,   839,
     125,    78,   137,   790,   173,   672,   791,   736,   428,   193,
     174,   550,   551,   194,   545,   482,   166,   167,   168,   169,
     429,   173,   496,   172,   624,   635,   636,   174,   664,   665,
     419,   173,   496,   668,   669,   645,   646,   174,   327,    78,
     864,   422,   328,   694,   865,   769,   133,   173,   423,   173,
     129,   173,   715,   174,   493,   174,   438,   174,   127,   123,
     822,   494,   383,   173,   124,   824,   154,   384,   493,   174,
     258,   721,   338,   339,   728,   506,   259,   260,   261,   262,
     263,   264,   214,   128,   215,   558,   197,   493,   201,   172,
     203,   204,   559,   558,   623,   493,   210,   173,   847,   493,
     633,   493,   716,   174,   130,   231,   717,    86,   718,   265,
     173,   482,   173,   800,   509,   131,   174,   509,   174,   510,
     801,   677,   512,    87,    88,    89,   678,   132,    90,    91,
     183,  -276,   172,  -276,   740,   741,   773,   774,   251,   254,
     173,    92,   184,  -277,    93,  -277,   174,   186,   726,   727,
     217,   173,   218,   219,   448,   189,   195,   174,   173,   205,
      78,   317,   541,   592,   174,   211,   212,   256,    94,   222,
     223,   581,   232,   738,   739,   233,    95,   742,   743,    96,
      97,   166,   167,   168,   169,   281,   173,    98,   234,   238,
      99,   100,   174,   807,   173,   235,   236,   173,   101,    78,
     174,   568,    78,   174,   584,   809,   220,   237,   173,   770,
      78,   172,   586,   353,   174,   813,   356,   357,   213,   173,
     356,   360,   361,   362,   363,   174,   277,   221,   278,   222,
     223,   592,   819,   820,   258,   293,   224,   225,   226,   227,
     259,   260,   261,   262,   263,   264,   286,   318,   438,   794,
     795,   173,    78,   325,   588,   834,   173,   174,    78,   439,
     647,   295,   174,   440,   840,   348,   349,   850,   851,   441,
      78,   352,   649,   265,   592,    78,   592,   683,   592,   708,
     708,   709,   867,   354,   858,   172,   172,   358,   367,   173,
     482,   861,   451,   173,   173,   174,   369,   420,   421,   174,
     174,   370,   378,   380,   869,   386,   425,   871,   292,   442,
     454,    86,   455,   443,   474,   467,   469,   451,   507,   477,
     879,   444,   445,   446,   484,   485,   486,    87,    88,    89,
     508,   513,    90,    91,   511,    14,    15,   592,    16,   592,
     498,   536,   523,   439,   538,    92,   573,   737,    93,   542,
     549,   560,   561,   441,    17,   565,   566,   567,   583,   594,
      18,   595,   597,    19,    20,   524,   598,    22,   239,   600,
     240,   581,    94,   241,   601,   603,   613,   617,   482,   528,
     251,   637,   634,    96,   337,   482,   242,   638,   643,   666,
     653,    98,   243,   442,    99,   100,   244,   443,   667,   245,
      14,    15,   101,    16,   670,   444,   445,   446,    23,   674,
     679,   684,   688,   482,   690,   693,    24,    25,   692,    17,
      26,   482,   697,   604,   482,    18,   695,   605,    19,    20,
      21,   698,    22,   719,   720,   438,   722,   723,    84,   729,
     730,   159,   160,   731,   734,   745,   572,   258,   735,   746,
     755,   161,   753,   259,   260,   261,   262,   263,   264,   764,
     759,   766,   767,   761,   762,   159,   160,   775,   780,   162,
     163,   782,   787,    23,   788,   161,   164,   789,   482,   165,
     792,    24,    25,   482,   803,    26,   265,   166,   167,   168,
     169,   170,   615,   162,   163,   171,   793,   796,   797,  -308,
     164,   802,   811,   165,   816,   818,   629,   823,   825,   101,
     826,   166,   167,   168,   169,   170,   482,  -308,   830,   229,
    -308,   206,  -308,  -308,   833,   207,   855,   835,   838,  -308,
    -308,  -308,  -308,   101,   875,   842,   843,   333,  -308,   845,
    -308,  -308,   846,   207,   856,   859,   863,  -308,  -308,  -308,
    -308,   258,   866,   870,   883,   872,   208,   259,   260,   261,
     262,   263,   264,   877,   138,   878,   881,   377,   381,   375,
     514,   258,   376,   882,   208,   139,   140,   259,   260,   261,
     262,   263,   264,   141,   250,   142,   754,   868,   143,   711,
     265,   876,   138,   144,   145,   146,   147,   148,   379,   874,
     744,   700,   701,   139,   140,   589,   651,   544,   149,   714,
     265,   141,   359,   142,   340,   101,   143,   614,   138,   772,
     776,   144,   145,   146,   147,   148,     0,     0,   529,   139,
     140,     0,     0,     0,   138,     0,   149,   141,     0,   142,
     350,     0,   143,   101,     0,   139,   140,   144,   145,   146,
     147,   148,     0,   141,     0,   142,   503,     0,   143,     0,
       0,     0,   149,   144,   145,   146,   147,   148,     0,   101,
     451,   451,   138,     0,     0,     0,     0,     0,   149,     0,
       0,     0,     0,   139,   140,   101,     0,     0,   138,   763,
       0,   141,     0,   142,   625,     0,   143,     0,     0,   139,
     140,   144,   145,   146,   147,   148,     0,   141,   778,   142,
     627,     0,   143,     0,   138,     0,   149,   144,   145,   146,
     147,   148,     0,   101,     0,   139,   140,     0,     0,     0,
     138,     0,   149,   141,     0,   142,   630,     0,   143,   101,
       0,   139,   140,   144,   145,   146,   147,   148,     0,   141,
       0,   142,     0,     0,   143,     0,   810,     0,   149,   144,
     145,   146,   147,   148,     0,   101,     0,     0,     0,     0,
       0,   138,     0,     0,   149,     0,   -50,   -50,     0,     0,
     289,   101,   139,   140,     0,     0,   -50,   -50,   -50,     0,
     141,   290,   449,     0,   291,   143,   292,   451,   -50,   451,
     144,   145,   146,   147,   148,   138,     0,     0,     0,   -50,
       0,     0,     0,     0,     0,   149,   139,   140,     0,     0,
       0,   138,   101,     0,   141,     0,   449,   208,     0,   143,
       0,     0,     0,   399,   144,   145,   146,   147,   569,     0,
     141,     0,   142,     0,     0,   143,     0,   138,     0,   149,
     144,   145,   146,   147,   148,     0,   101,     0,     0,   401,
       0,     0,     0,   138,     0,   149,   141,     0,   142,     0,
       0,   143,   101,     0,     0,   403,   144,   145,   146,   147,
     148,     0,   141,     0,   142,     0,     0,   143,     0,   138,
       0,   149,   144,   145,   146,   147,   148,     0,   101,     0,
       0,   405,     0,     0,     0,   138,     0,   149,   141,     0,
     142,     0,     0,   143,   101,     0,     0,   407,   144,   145,
     146,   147,   148,     0,   141,     0,   142,     0,     0,   143,
       0,   138,     0,   149,   144,   145,   146,   147,   148,     0,
     101,   387,   388,   389,   390,   391,   392,   393,   394,   149,
     141,     0,   142,     0,     0,   143,   101,     0,     0,     0,
     144,   145,   146,   147,   148,     0,     0,     0,   335,     0,
       0,   162,   163,     0,     0,   149,     0,   258,     0,     0,
       0,   165,   101,   259,   260,   261,   262,   263,   264,   166,
     167,   168,   169,   170,   258,     0,     0,     0,     0,     0,
     259,   260,   261,   262,   263,   264,     0,     0,     0,     0,
       0,   101,   258,     0,     0,     0,   265,   336,   259,   260,
     261,   262,   263,   264,     0,     0,     0,     0,     0,     0,
       0,   258,     0,   265,     0,     0,   382,   259,   260,   261,
     262,   263,   264,  -301,     0,     0,     0,     0,     0,   258,
       0,   265,     0,     0,   385,   259,   260,   261,   262,   263,
     264,     0,   426,   427,  -301,     0,  -301,  -301,     0,   511,
     265,     0,   428,  -301,  -301,  -301,  -301,     0,     0,     0,
     166,   167,   168,   169,   429,     0,     0,     0,   265
};

static const yytype_int16 yycheck[] =
{
      19,    64,   320,    39,   331,   192,    79,    49,   335,    88,
      54,   208,    31,    92,    51,    70,   491,    51,    45,    98,
     558,     6,    51,    70,    79,    51,   161,     5,   559,    70,
       5,   541,    79,    58,   138,   213,     5,    76,    79,   143,
       5,     5,     5,    54,    55,    76,     5,     5,   291,   292,
       5,   124,    63,     2,    93,    74,    90,     5,    58,   302,
      60,    90,    81,   300,   301,     5,     5,   139,   140,   124,
     289,    96,    30,    76,   584,     8,   586,   124,   588,    42,
      58,   583,    83,   124,    59,    48,    49,    50,    51,    52,
      53,    76,   289,    26,    63,   303,   304,   305,   306,    32,
      64,   603,   633,    36,   123,    53,    65,   162,   163,    94,
     270,   271,   272,   273,   274,   275,   276,   164,    81,    84,
      85,    76,     5,   164,    73,    65,    65,    48,    76,    60,
     349,   449,    65,    64,     5,   637,   638,   647,    59,   649,
      48,   183,     5,    76,    76,     5,    48,   189,    65,     5,
      72,    59,   349,   208,    76,   192,    48,    59,   192,    76,
       8,   208,    94,   192,    95,   184,   192,   208,   205,   206,
     348,   205,   206,   277,    95,    58,   205,   206,    26,   205,
     206,     8,   684,   421,    32,   428,     8,    58,    36,   426,
     427,    17,   234,   235,   266,   267,   268,   269,    58,    26,
      60,    64,    58,    95,    26,    32,    64,   745,    17,    36,
      32,    58,   231,    60,    36,     5,    42,   236,   237,    48,
     722,    17,    48,    49,    50,    51,    52,    53,    76,   704,
      59,    28,    76,     5,   289,     5,    28,    95,    65,   399,
      30,   401,   289,   403,    16,   405,   293,   407,   289,    76,
      94,    60,   293,     0,    76,    81,   453,     5,   455,   278,
      30,   324,    58,    60,   766,   307,    95,   346,   347,    41,
      62,    41,     5,     5,    47,   294,     4,    25,   325,   606,
     322,    54,    55,    11,   325,   787,   613,    76,    44,    45,
     309,    23,   311,     5,   349,   314,    50,    30,   332,   496,
      56,   488,   349,   332,    58,    78,   549,   308,   349,   811,
       5,    76,    64,   550,   551,    59,   818,    61,    30,   366,
      64,    77,   323,    75,    76,   366,     6,   369,   370,   507,
     508,    76,   467,   511,   469,    30,   378,   381,   380,   375,
      76,    59,    94,   845,   164,   553,   554,   555,   556,    29,
      76,    31,   379,    76,    34,   673,   674,    42,    94,    76,
      54,    55,   600,    48,    49,    50,    51,    52,    53,    76,
     371,   372,    66,    26,   421,    60,    26,    44,    45,   452,
     421,   454,   709,    36,    17,   712,    36,    59,     5,    56,
      54,    55,    64,   412,    26,   414,    81,   452,   453,   454,
     455,   536,    66,   538,    36,   452,   453,   454,   455,    42,
      77,   452,   453,   454,   455,    48,    49,    50,    51,    52,
      53,    50,    50,    44,    45,    46,   473,    54,    55,    58,
      58,    50,   473,   162,   163,    56,    57,   456,    65,    58,
     513,   496,   769,   770,   523,   524,    38,    12,    81,   496,
     688,   488,   471,    76,   488,   496,    77,   695,   513,   488,
     703,    82,   488,   464,    54,    55,   513,    59,    50,    61,
      62,    59,   513,   293,    53,    65,    58,   478,   520,   521,
     677,    60,   800,    50,   802,   723,    54,    55,    72,   816,
      76,    58,    76,   731,   541,   568,   734,    65,    64,    72,
     541,    54,    55,    76,    50,   325,    72,    73,    74,    75,
      76,   558,    58,   568,    50,   516,   517,   558,   560,   561,
     539,   568,    58,   565,   566,   526,   527,   568,    38,    58,
      72,    58,    42,   596,    76,    17,    65,   584,    65,   586,
      59,   588,   621,   584,    58,   586,   366,   588,    76,    48,
     788,    65,    58,   600,    53,   793,    69,    63,    58,   600,
      42,   634,   205,   206,   643,    65,    48,    49,    50,    51,
      52,    53,    59,    76,    61,    58,    89,    58,    91,   634,
      93,    94,    65,    58,    65,    58,    99,   634,   826,    58,
      65,    58,    65,   634,    59,    48,    65,     3,    65,    81,
     647,   421,   649,    58,    58,    76,   647,    58,   649,    63,
      65,    58,    63,    19,    20,    21,    63,    76,    24,    25,
      76,    59,   677,    61,   666,   667,   705,   706,   141,   142,
     677,    37,    64,    59,    40,    61,   677,    76,   639,   640,
      59,   688,    61,    62,   663,    76,    76,   688,   695,    60,
      58,   670,    60,   473,   695,    76,    76,    59,    64,    61,
      62,   680,    59,   664,   665,    59,    72,   668,   669,    75,
      76,    72,    73,    74,    75,    76,   723,    83,    76,    59,
      86,    87,   723,   756,   731,    76,    48,   734,    94,    58,
     731,    60,    58,   734,    60,   758,    38,    48,   745,    17,
      58,   756,    60,   216,   745,   768,   219,   220,    64,   756,
     223,   224,   225,   226,   227,   756,    79,    59,    64,    61,
      62,   541,   785,   786,    42,    64,    68,    69,    70,    71,
      48,    49,    50,    51,    52,    53,    76,     5,   558,   740,
     741,   788,    58,    60,    60,   808,   793,   788,    58,    26,
      60,    53,   793,    30,   817,    64,    48,   830,   831,    36,
      58,    76,    60,    81,   584,    58,   586,    60,   588,    58,
      58,    60,    60,    76,   837,   830,   831,    76,     5,   826,
     600,   844,   295,   830,   831,   826,    76,    28,    60,   830,
     831,    76,    76,    76,   857,    76,    76,   860,    64,    76,
      28,     3,    28,    80,     9,    76,    76,   320,    64,    76,
     873,    88,    89,    90,   327,   328,   329,    19,    20,    21,
      64,    53,    24,    25,    64,     7,     8,   647,    10,   649,
      91,    76,    14,    26,    76,    37,     5,    30,    40,    76,
      64,    76,    76,    36,    26,    76,    76,    76,     5,     5,
      32,    76,     5,    35,    36,    37,     5,    39,     8,    60,
      10,   880,    64,    13,     5,     5,    41,    76,   688,   382,
     383,     5,    53,    75,    76,   695,    26,     5,    15,    76,
      28,    83,    32,    76,    86,    87,    36,    80,    76,    39,
       7,     8,    94,    10,    62,    88,    89,    90,    80,    58,
      30,     5,    60,   723,    28,     5,    88,    89,    53,    26,
      92,   731,    76,    18,   734,    32,    60,    22,    35,    36,
      37,     9,    39,     5,     5,   745,     5,    60,     5,    30,
       5,    26,    27,    60,    60,    60,   449,    42,    76,     5,
      76,    36,    28,    48,    49,    50,    51,    52,    53,     5,
      53,     5,     5,    76,    76,    26,    27,    30,    76,    54,
      55,     5,     5,    80,    60,    36,    61,    59,   788,    64,
       5,    88,    89,   793,    63,    92,    81,    72,    73,    74,
      75,    76,   495,    54,    55,    80,    60,     5,     5,    38,
      61,    60,     5,    64,    17,     5,   509,     5,     5,    94,
      60,    72,    73,    74,    75,    76,   826,    38,    28,    80,
      59,    60,    61,    62,    76,    64,    28,     5,     5,    68,
      69,    70,    71,    94,    64,     5,     5,    28,    59,     5,
      61,    62,     5,    64,     5,     5,     5,    68,    69,    70,
      71,    42,     5,     5,    65,     5,    95,    48,    49,    50,
      51,    52,    53,     5,    43,     5,     5,   241,   245,   239,
     366,    42,   240,   880,    95,    54,    55,    48,    49,    50,
      51,    52,    53,    62,    63,    64,   680,   855,    67,    60,
      81,   868,    43,    72,    73,    74,    75,    76,   243,   866,
     670,   604,   605,    54,    55,   471,   539,   422,    87,   619,
      81,    62,   223,    64,    65,    94,    67,   493,    43,   704,
     708,    72,    73,    74,    75,    76,    -1,    -1,   383,    54,
      55,    -1,    -1,    -1,    43,    -1,    87,    62,    -1,    64,
      65,    -1,    67,    94,    -1,    54,    55,    72,    73,    74,
      75,    76,    -1,    62,    -1,    64,    65,    -1,    67,    -1,
      -1,    -1,    87,    72,    73,    74,    75,    76,    -1,    94,
     673,   674,    43,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    54,    55,    94,    -1,    -1,    43,   692,
      -1,    62,    -1,    64,    65,    -1,    67,    -1,    -1,    54,
      55,    72,    73,    74,    75,    76,    -1,    62,   711,    64,
      65,    -1,    67,    -1,    43,    -1,    87,    72,    73,    74,
      75,    76,    -1,    94,    -1,    54,    55,    -1,    -1,    -1,
      43,    -1,    87,    62,    -1,    64,    65,    -1,    67,    94,
      -1,    54,    55,    72,    73,    74,    75,    76,    -1,    62,
      -1,    64,    -1,    -1,    67,    -1,   759,    -1,    87,    72,
      73,    74,    75,    76,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    87,    -1,    44,    45,    -1,    -1,
      48,    94,    54,    55,    -1,    -1,    54,    55,    56,    -1,
      62,    59,    64,    -1,    62,    67,    64,   800,    66,   802,
      72,    73,    74,    75,    76,    43,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    87,    54,    55,    -1,    -1,
      -1,    43,    94,    -1,    62,    -1,    64,    95,    -1,    67,
      -1,    -1,    -1,    55,    72,    73,    74,    75,    76,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    43,    -1,    87,
      72,    73,    74,    75,    76,    -1,    94,    -1,    -1,    55,
      -1,    -1,    -1,    43,    -1,    87,    62,    -1,    64,    -1,
      -1,    67,    94,    -1,    -1,    55,    72,    73,    74,    75,
      76,    -1,    62,    -1,    64,    -1,    -1,    67,    -1,    43,
      -1,    87,    72,    73,    74,    75,    76,    -1,    94,    -1,
      -1,    55,    -1,    -1,    -1,    43,    -1,    87,    62,    -1,
      64,    -1,    -1,    67,    94,    -1,    -1,    55,    72,    73,
      74,    75,    76,    -1,    62,    -1,    64,    -1,    -1,    67,
      -1,    43,    -1,    87,    72,    73,    74,    75,    76,    -1,
      94,   258,   259,   260,   261,   262,   263,   264,   265,    87,
      62,    -1,    64,    -1,    -1,    67,    94,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    33,    -1,
      -1,    54,    55,    -1,    -1,    87,    -1,    42,    -1,    -1,
      -1,    64,    94,    48,    49,    50,    51,    52,    53,    72,
      73,    74,    75,    76,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    94,    42,    -1,    -1,    -1,    81,    65,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    81,    -1,    -1,    66,    48,    49,    50,
      51,    52,    53,    38,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    81,    -1,    -1,    65,    48,    49,    50,    51,    52,
      53,    -1,    54,    55,    59,    -1,    61,    62,    -1,    64,
      81,    -1,    64,    68,    69,    70,    71,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    81
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
      38,    59,    61,    62,    68,    69,    70,    71,   180,    80,
     128,    48,    59,    59,    76,    76,    48,    48,    59,     8,
      10,    13,    26,    32,    36,    39,   107,   221,   219,   219,
      63,   217,   222,   223,   217,   221,    59,     5,    42,    48,
      49,    50,    51,    52,    53,    81,    47,    54,    55,    78,
      44,    45,    46,    56,    57,    77,    82,    79,    64,   175,
     175,    76,   140,   140,    76,   126,    76,   178,   179,    48,
      59,    62,    64,    64,   133,    53,     6,    29,    31,    34,
      54,    55,    66,    44,    45,    56,    77,    50,   169,     8,
      26,    32,    36,    65,    76,   171,   172,   180,     5,     5,
      53,    76,    50,   169,   158,    60,   187,    38,    42,    23,
      17,    17,    58,    28,    30,    33,    65,    76,   188,   188,
      65,   197,   198,   217,   127,   138,    84,    85,    64,    48,
      65,   197,    76,   217,    76,   201,   217,   217,    76,   201,
     217,   217,   217,   217,    50,     5,    64,     5,   180,    76,
      76,   169,   169,   180,   180,   111,   119,   103,    76,   153,
      76,   117,    66,    58,    63,    65,    76,   218,   218,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   219,    55,
     220,    55,   220,    55,   220,    55,   220,    55,   220,   220,
     220,   221,     8,    26,    32,    36,    65,   176,   177,   180,
      28,    60,    58,    65,   127,    76,    54,    55,    64,    76,
     116,   141,   142,   143,   141,    76,   123,   124,   126,    26,
      30,    36,    76,    80,    88,    89,    90,   134,   180,    64,
     113,   217,    28,    62,    28,    28,   131,   142,   142,   141,
     143,   143,   143,   143,   169,   170,   180,    76,   180,    76,
     180,     5,    65,    60,     9,   174,   113,    76,   169,   170,
     102,    76,   126,   132,   217,   217,   217,   185,   186,   185,
     200,   206,   185,    58,    65,    60,    58,    96,    91,   182,
     190,   191,   182,    65,   197,   127,    65,    64,    64,    58,
      63,    64,    63,    53,   123,    50,   169,   169,   170,   170,
      50,    50,     5,    14,    37,   108,   169,   169,   217,   223,
     220,   220,   220,   220,   220,   180,    76,   180,    76,     5,
      65,    60,    76,   132,   179,    50,   142,   142,   141,    64,
      54,    55,    63,    44,    45,    56,    77,    65,    58,    65,
      76,    76,     5,    26,    36,    76,    76,    76,    60,    76,
     113,   115,   217,     5,   128,   138,   128,   138,   144,   145,
     146,   180,   170,     5,    60,   175,    60,   175,    60,   172,
       6,    76,   126,   173,     5,    76,   156,     5,     5,   170,
      60,     5,    59,     5,    18,    22,    17,   187,   141,   208,
     209,   210,   211,    41,   198,   217,   138,    76,    30,     5,
      30,    41,    30,    65,    50,    65,   197,    65,   197,   217,
      65,   197,   128,    65,    53,   170,   170,     5,     5,   169,
     169,   182,   182,    15,   109,   170,   170,    60,   175,    60,
     175,   177,   173,    28,    65,   141,   142,   142,   143,   143,
     143,   143,   124,   133,   169,   169,    76,    76,   169,   169,
      62,   135,   128,    60,    58,     5,    65,    58,    63,    30,
       5,    25,   150,    60,     5,   174,   173,   173,    60,   173,
      28,    59,    53,     5,   102,    60,   132,    76,     9,   174,
     217,   217,   185,    66,     5,    16,    41,   207,    58,    60,
     185,    60,    17,    60,   191,   182,    65,    65,    65,     5,
       5,   128,     5,    60,   174,   174,   170,   170,   182,    30,
       5,    60,   173,   173,    60,    76,    65,    30,   170,   170,
     169,   169,   170,   170,   171,    60,     5,   113,   113,   114,
      65,    76,   138,    28,   146,    76,   147,   174,   163,    53,
     132,    76,    76,   217,     5,   132,     5,     5,   157,    17,
      17,   141,   209,   182,   182,    30,   211,   185,   217,   185,
      76,    30,     5,   174,   132,   165,   166,     5,    60,    59,
     132,   132,     5,    60,   170,   170,     5,     5,    63,   124,
      58,    65,    60,    63,   129,    28,    60,   128,   161,   102,
     217,     5,   174,   102,   185,   185,    17,   167,     5,   102,
     102,   174,   132,     5,   132,     5,    60,   136,   113,   113,
      28,   130,   149,    76,   102,     5,   174,   164,     5,   185,
     102,   174,     5,     5,   159,     5,     5,   132,     5,    76,
     128,   128,   151,   152,   210,    28,     5,   162,   102,     5,
     168,   102,   174,     5,    72,    76,     5,    60,   149,   102,
       5,   102,     5,   160,   152,    64,   151,     5,     5,   102,
     148,     5,   144,    65
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
     144,   145,   145,   147,   146,   146,   148,   149,   150,   150,
     150,   151,   151,   152,   152,   153,   153,   154,   154,   154,
     155,   155,   156,   155,   157,   155,   158,   155,   159,   155,
     160,   155,   161,   155,   162,   155,   163,   155,   164,   155,
     165,   155,   166,   155,   167,   155,   168,   155,   169,   170,
     170,   170,   171,   171,   172,   172,   172,   172,   172,   172,
     172,   172,   173,   173,   173,   173,   174,   174,   175,   175,
     175,   176,   176,   177,   177,   177,   177,   177,   178,   178,
     179,   180,   180,   181,   183,   182,   184,   182,   186,   185,
     187,   187,   187,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   189,   189,
     189,   189,   190,   190,   190,   191,   191,   192,   192,   193,
     193,   193,   194,   194,   194,   194,   195,   195,   195,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   197,
     197,   198,   198,   198,   199,   199,   199,   199,   199,   199,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   201,
     201,   202,   203,   203,   204,   204,   206,   205,   207,   207,
     207,   208,   208,   209,   209,   210,   210,   211,   211,   212,
     213,   214,   214,   214,   215,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   218,   218,   218,
     218,   218,   218,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   220,   220,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   223,   223
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
       4,     7,     1,     3,     1,     3,     4,     1,     1,     1,
       3,     1,     1,     5,     1,     1,     4,     1,     3,     1,
       2,     2,     0,     0,     9,     4,     2,     4,     4,     6,
       9,     0,     5,     4,     2,     4,     3,     6,     1,     3,
       1,     0,     2,     1,     2,     4,     5,     7,     5,     5,
       6,     8,     7,     0,     3,     0,     3,     3,     1,     0,
       3,     1,     4,     3,     1,     3,     1,     2,     2,     3,
       3,     1,     4,     3,     3,     3,     3,     1,     2,     2,
       3,     3,     1,     3,     3,     3,     3,     1,     4,     3,
       2,     3,     1,     0,     4,     0,     0,     0,     7,     5,
       0,     3,     1,     6,     0,     2,     1,     4,     6,     6,
       7,     9,     0,     9,     0,    11,     0,     7,     0,    13,
       0,    15,     0,    12,     0,    14,     0,    11,     0,    13,
       0,    11,     0,    11,     0,    12,     0,    14,     0,     3,
       2,     0,     3,     1,     3,     5,     4,     3,     4,     5,
       6,     4,     1,     3,     3,     1,     0,     3,     3,     2,
       0,     3,     1,     3,     4,     4,     3,     5,     3,     1,
       1,     3,     1,     3,     0,     4,     0,     2,     0,     2,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     5,     5,
       7,     5,     1,     3,     2,     6,     4,     1,     2,     2,
       4,     5,     1,     1,     3,     1,     1,     3,     1,     4,
       4,     3,     3,     6,     5,     6,     5,     6,     5,     3,
       1,     1,     3,     5,     3,     3,     3,     3,     3,     3,
       4,     3,     2,     3,     4,     2,     3,     2,     1,     3,
       1,     3,     2,     2,     4,     6,     0,     7,     0,     2,
       2,     3,     1,     3,     0,     3,     1,     1,     3,     4,
       4,     8,     8,     6,     4,     3,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     3,
       3,     3,     3,     1,     3,     3,     3,     3,     3,     4,
       4,     4,     4,     4,     3,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     3,     1,     2,     2,     2,     3,
       1,     3,     1,     3
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
#line 2207 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2213 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2219 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2225 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2231 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2237 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2243 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2249 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2255 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2261 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2267 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2273 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2279 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2285 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2291 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2297 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2303 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2309 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2315 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2321 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2327 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2333 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2339 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2345 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2351 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 267 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2357 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 268 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2363 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2369 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2375 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2381 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2387 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2393 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2399 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2423 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2429 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2435 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2441 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2447 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2459 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2465 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2477 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2483 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2489 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2495 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2501 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2507 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2513 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2519 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2525 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2531 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2543 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2549 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2555 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2561 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2579 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2585 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2591 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2597 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2603 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2609 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2615 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2621 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2627 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2639 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2645 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2651 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2657 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2663 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2669 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2681 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2687 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2693 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2705 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2711 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2729 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2735 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2741 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2753 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 96: /* type: CLASSSY class_members ENDSY  */
#line 375 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 97: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 376 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 98: /* result_type: IDENT  */
#line 379 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2783 "pascal.tab.c"
    break;

  case 99: /* result_type: IDENT PERIOD IDENT  */
#line 380 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2789 "pascal.tab.c"
    break;

  case 100: /* result_type: spec_ref  */
#line 381 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2795 "pascal.tab.c"
    break;

  case 101: /* class_members: %empty  */
#line 384 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2801 "pascal.tab.c"
    break;

  case 102: /* class_members: class_members class_member  */
#line 385 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 103: /* class_member: IDENT  */
#line 388 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2813 "pascal.tab.c"
    break;

  case 104: /* class_member: IDENT SEMICOLON  */
#line 389 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2819 "pascal.tab.c"
    break;

  case 105: /* class_member: id_list COLON type SEMICOLON  */
#line 390 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 106: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 391 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 107: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 392 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2837 "pascal.tab.c"
    break;

  case 108: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2843 "pascal.tab.c"
    break;

  case 109: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 394 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2849 "pascal.tab.c"
    break;

  case 110: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2855 "pascal.tab.c"
    break;

  case 111: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 396 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2861 "pascal.tab.c"
    break;

  case 112: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 397 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 113: /* prop_index_opt: %empty  */
#line 400 "pascal.y"
    { (yyval.node) = NULL; }
#line 2873 "pascal.tab.c"
    break;

  case 114: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 401 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2879 "pascal.tab.c"
    break;

  case 115: /* prop_specs: %empty  */
#line 404 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2885 "pascal.tab.c"
    break;

  case 116: /* prop_specs: prop_specs IDENT IDENT  */
#line 405 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 117: /* prop_specs: prop_specs IDENT INTCONST  */
#line 406 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2897 "pascal.tab.c"
    break;

  case 118: /* packed_opt: PACKEDSY  */
#line 409 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2903 "pascal.tab.c"
    break;

  case 119: /* packed_opt: %empty  */
#line 410 "pascal.y"
     { (yyval.node) = NULL; }
#line 2909 "pascal.tab.c"
    break;

  case 120: /* simple_type: LPARENT enum_list RPARENT  */
#line 413 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2915 "pascal.tab.c"
    break;

  case 121: /* simple_type: IDENT  */
#line 414 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2921 "pascal.tab.c"
    break;

  case 122: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 415 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2927 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT PERIOD IDENT  */
#line 416 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2933 "pascal.tab.c"
    break;

  case 124: /* simple_type: spec_ref  */
#line 417 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2939 "pascal.tab.c"
    break;

  case 125: /* simple_type: lo_bound DOTDOT b_expr  */
#line 418 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2945 "pascal.tab.c"
    break;

  case 126: /* lo_bound: lo_term  */
#line 421 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2951 "pascal.tab.c"
    break;

  case 127: /* lo_bound: PLUS lo_term  */
#line 422 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2957 "pascal.tab.c"
    break;

  case 128: /* lo_bound: MINUS lo_term  */
#line 423 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2963 "pascal.tab.c"
    break;

  case 129: /* lo_bound: lo_bound PLUS b_term  */
#line 424 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2969 "pascal.tab.c"
    break;

  case 130: /* lo_bound: lo_bound MINUS b_term  */
#line 425 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2975 "pascal.tab.c"
    break;

  case 131: /* lo_term: scalar_constant  */
#line 428 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2981 "pascal.tab.c"
    break;

  case 132: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 429 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2987 "pascal.tab.c"
    break;

  case 133: /* lo_term: lo_term MUL b_factor  */
#line 430 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2993 "pascal.tab.c"
    break;

  case 134: /* lo_term: lo_term IDIV b_factor  */
#line 431 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2999 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term IMOD b_factor  */
#line 432 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3005 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term SHIFTOP b_factor  */
#line 433 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3011 "pascal.tab.c"
    break;

  case 137: /* b_expr: b_term  */
#line 436 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3017 "pascal.tab.c"
    break;

  case 138: /* b_expr: PLUS b_term  */
#line 437 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3023 "pascal.tab.c"
    break;

  case 139: /* b_expr: MINUS b_term  */
#line 438 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3029 "pascal.tab.c"
    break;

  case 140: /* b_expr: b_expr PLUS b_term  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3035 "pascal.tab.c"
    break;

  case 141: /* b_expr: b_expr MINUS b_term  */
#line 440 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 142: /* b_term: b_factor  */
#line 443 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3047 "pascal.tab.c"
    break;

  case 143: /* b_term: b_term MUL b_factor  */
#line 444 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3053 "pascal.tab.c"
    break;

  case 144: /* b_term: b_term IDIV b_factor  */
#line 445 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3059 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term IMOD b_factor  */
#line 446 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3065 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term SHIFTOP b_factor  */
#line 447 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3071 "pascal.tab.c"
    break;

  case 147: /* b_factor: scalar_constant  */
#line 450 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3077 "pascal.tab.c"
    break;

  case 148: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 451 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 149: /* b_factor: LPARENT b_expr RPARENT  */
#line 452 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3089 "pascal.tab.c"
    break;

  case 150: /* record_body: record_field_list record_case_opt  */
#line 455 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3095 "pascal.tab.c"
    break;

  case 151: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 458 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 152: /* record_field_list: record_field  */
#line 459 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 153: /* $@4: %empty  */
#line 462 "pascal.y"
                  { }
#line 3113 "pascal.tab.c"
    break;

  case 154: /* record_field: id_list COLON $@4 type  */
#line 462 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3119 "pascal.tab.c"
    break;

  case 155: /* record_field: %empty  */
#line 463 "pascal.y"
     { (yyval.node) = NULL; }
#line 3125 "pascal.tab.c"
    break;

  case 156: /* case_arm_mark: %empty  */
#line 466 "pascal.y"
    { }
#line 3131 "pascal.tab.c"
    break;

  case 157: /* case_open: %empty  */
#line 469 "pascal.y"
    { }
#line 3137 "pascal.tab.c"
    break;

  case 158: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 472 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 159: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 473 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 160: /* record_case_opt: %empty  */
#line 474 "pascal.y"
     { (yyval.node) = NULL; }
#line 3155 "pascal.tab.c"
    break;

  case 161: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 477 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 162: /* record_case_list: record_case_arm  */
#line 478 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 163: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 481 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3173 "pascal.tab.c"
    break;

  case 164: /* record_case_arm: %empty  */
#line 482 "pascal.y"
     { (yyval.node) = NULL; }
#line 3179 "pascal.tab.c"
    break;

  case 165: /* var_decl_list: var_decl_list var_decl  */
#line 485 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 166: /* var_decl_list: var_decl  */
#line 486 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3191 "pascal.tab.c"
    break;

  case 167: /* var_decl: id_list COLON type SEMICOLON  */
#line 489 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3197 "pascal.tab.c"
    break;

  case 168: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 490 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3203 "pascal.tab.c"
    break;

  case 169: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 491 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3209 "pascal.tab.c"
    break;

  case 170: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 494 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3215 "pascal.tab.c"
    break;

  case 171: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 495 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3221 "pascal.tab.c"
    break;

  case 172: /* $@5: %empty  */
#line 496 "pascal.y"
                                                                               { }
#line 3227 "pascal.tab.c"
    break;

  case 173: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 496 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3233 "pascal.tab.c"
    break;

  case 174: /* $@6: %empty  */
#line 497 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 175: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 497 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3245 "pascal.tab.c"
    break;

  case 176: /* $@7: %empty  */
#line 498 "pascal.y"
                                        { }
#line 3251 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 498 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 178: /* $@8: %empty  */
#line 499 "pascal.y"
                                                                                                           { }
#line 3263 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 499 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3269 "pascal.tab.c"
    break;

  case 180: /* $@9: %empty  */
#line 500 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3275 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 500 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3281 "pascal.tab.c"
    break;

  case 182: /* $@10: %empty  */
#line 501 "pascal.y"
                                                                                                 { }
#line 3287 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 501 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3293 "pascal.tab.c"
    break;

  case 184: /* $@11: %empty  */
#line 502 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3299 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 502 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3305 "pascal.tab.c"
    break;

  case 186: /* $@12: %empty  */
#line 503 "pascal.y"
                                                                                            { }
#line 3311 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 503 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3317 "pascal.tab.c"
    break;

  case 188: /* $@13: %empty  */
#line 504 "pascal.y"
                                                                                                             { }
#line 3323 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 504 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3331 "pascal.tab.c"
    break;

  case 190: /* $@14: %empty  */
#line 507 "pascal.y"
                                                                                              { }
#line 3337 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3345 "pascal.tab.c"
    break;

  case 192: /* $@15: %empty  */
#line 510 "pascal.y"
                                                                                             { }
#line 3351 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3359 "pascal.tab.c"
    break;

  case 194: /* $@16: %empty  */
#line 513 "pascal.y"
                                                                                                    { }
#line 3365 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3373 "pascal.tab.c"
    break;

  case 196: /* $@17: %empty  */
#line 516 "pascal.y"
                                                                                                                     { }
#line 3379 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3387 "pascal.tab.c"
    break;

  case 198: /* pv_mark: %empty  */
#line 521 "pascal.y"
    { }
#line 3393 "pascal.tab.c"
    break;

  case 199: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 524 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3399 "pascal.tab.c"
    break;

  case 200: /* parameter_list_opt: LPARENT RPARENT  */
#line 525 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3405 "pascal.tab.c"
    break;

  case 201: /* parameter_list_opt: %empty  */
#line 526 "pascal.y"
     { (yyval.node) = NULL; }
#line 3411 "pascal.tab.c"
    break;

  case 202: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 529 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3417 "pascal.tab.c"
    break;

  case 203: /* parameter_decl_list: parameter_decl  */
#line 530 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3423 "pascal.tab.c"
    break;

  case 204: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 533 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3429 "pascal.tab.c"
    break;

  case 205: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 534 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3435 "pascal.tab.c"
    break;

  case 206: /* parameter_decl: VARSY id_list COLON param_type  */
#line 535 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3441 "pascal.tab.c"
    break;

  case 207: /* parameter_decl: id_list COLON param_type  */
#line 536 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3447 "pascal.tab.c"
    break;

  case 208: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 537 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3453 "pascal.tab.c"
    break;

  case 209: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 538 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3459 "pascal.tab.c"
    break;

  case 210: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 539 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3465 "pascal.tab.c"
    break;

  case 211: /* parameter_decl: IDENT id_list COLON param_type  */
#line 540 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3471 "pascal.tab.c"
    break;

  case 212: /* param_type: IDENT  */
#line 543 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3477 "pascal.tab.c"
    break;

  case 213: /* param_type: IDENT PERIOD IDENT  */
#line 544 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3483 "pascal.tab.c"
    break;

  case 214: /* param_type: ARRAYSY OFSY IDENT  */
#line 545 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3489 "pascal.tab.c"
    break;

  case 215: /* param_type: spec_ref  */
#line 546 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3495 "pascal.tab.c"
    break;

  case 216: /* routine_directives: %empty  */
#line 549 "pascal.y"
    { (yyval.node) = NULL; }
#line 3501 "pascal.tab.c"
    break;

  case 217: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 550 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3514 "pascal.tab.c"
    break;

  case 218: /* pf_params: LPARENT pf_sections RPARENT  */
#line 560 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3520 "pascal.tab.c"
    break;

  case 219: /* pf_params: LPARENT RPARENT  */
#line 561 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3526 "pascal.tab.c"
    break;

  case 220: /* pf_params: %empty  */
#line 562 "pascal.y"
     { (yyval.node) = NULL; }
#line 3532 "pascal.tab.c"
    break;

  case 221: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 565 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3538 "pascal.tab.c"
    break;

  case 222: /* pf_sections: pf_section  */
#line 566 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3544 "pascal.tab.c"
    break;

  case 223: /* pf_section: id_list COLON param_type  */
#line 569 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3550 "pascal.tab.c"
    break;

  case 224: /* pf_section: VARSY id_list COLON param_type  */
#line 570 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3556 "pascal.tab.c"
    break;

  case 225: /* pf_section: CONSTSY id_list COLON param_type  */
#line 571 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3562 "pascal.tab.c"
    break;

  case 226: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 572 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3568 "pascal.tab.c"
    break;

  case 227: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 573 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3574 "pascal.tab.c"
    break;

  case 228: /* enum_list: enum_list COMMA enum_member  */
#line 576 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3580 "pascal.tab.c"
    break;

  case 229: /* enum_list: enum_member  */
#line 577 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3586 "pascal.tab.c"
    break;

  case 230: /* enum_member: IDENT  */
#line 580 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3592 "pascal.tab.c"
    break;

  case 231: /* id_list: id_list COMMA IDENT  */
#line 583 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3598 "pascal.tab.c"
    break;

  case 232: /* id_list: IDENT  */
#line 584 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3604 "pascal.tab.c"
    break;

  case 233: /* body: BEGINSY statement_list ENDSY  */
#line 587 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3610 "pascal.tab.c"
    break;

  case 234: /* $@18: %empty  */
#line 590 "pascal.y"
                             { pas_stmt_mark(); }
#line 3616 "pascal.tab.c"
    break;

  case 235: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 590 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3622 "pascal.tab.c"
    break;

  case 236: /* $@19: %empty  */
#line 591 "pascal.y"
     { pas_stmt_mark(); }
#line 3628 "pascal.tab.c"
    break;

  case 237: /* statement_list: $@19 statement  */
#line 591 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3634 "pascal.tab.c"
    break;

  case 238: /* $@20: %empty  */
#line 594 "pascal.y"
    { pas_stmt_mark(); }
#line 3640 "pascal.tab.c"
    break;

  case 239: /* body_stmt: $@20 statement  */
#line 594 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3646 "pascal.tab.c"
    break;

  case 240: /* statement: statement_no_label  */
#line 597 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3652 "pascal.tab.c"
    break;

  case 241: /* statement: INTCONST COLON statement_no_label  */
#line 598 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3658 "pascal.tab.c"
    break;

  case 242: /* statement: IDENT COLON statement_no_label  */
#line 599 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3664 "pascal.tab.c"
    break;

  case 243: /* statement_no_label: assignment  */
#line 602 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3670 "pascal.tab.c"
    break;

  case 244: /* statement_no_label: call  */
#line 603 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3676 "pascal.tab.c"
    break;

  case 245: /* statement_no_label: compound_statement  */
#line 604 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3682 "pascal.tab.c"
    break;

  case 246: /* statement_no_label: goto_statement  */
#line 605 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3688 "pascal.tab.c"
    break;

  case 247: /* statement_no_label: if_statement  */
#line 606 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3694 "pascal.tab.c"
    break;

  case 248: /* statement_no_label: case_statement  */
#line 607 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3700 "pascal.tab.c"
    break;

  case 249: /* statement_no_label: while_statement  */
#line 608 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3706 "pascal.tab.c"
    break;

  case 250: /* statement_no_label: repeat_statement  */
#line 609 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3712 "pascal.tab.c"
    break;

  case 251: /* statement_no_label: for_statement  */
#line 610 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3718 "pascal.tab.c"
    break;

  case 252: /* statement_no_label: with_statement  */
#line 611 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3724 "pascal.tab.c"
    break;

  case 253: /* statement_no_label: try_statement  */
#line 612 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3730 "pascal.tab.c"
    break;

  case 254: /* statement_no_label: raise_statement  */
#line 613 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3736 "pascal.tab.c"
    break;

  case 255: /* statement_no_label: inherited_call  */
#line 614 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3742 "pascal.tab.c"
    break;

  case 256: /* statement_no_label: INHERITEDSY  */
#line 615 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3748 "pascal.tab.c"
    break;

  case 257: /* statement_no_label: %empty  */
#line 616 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3754 "pascal.tab.c"
    break;

  case 258: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 619 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3760 "pascal.tab.c"
    break;

  case 259: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 620 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3766 "pascal.tab.c"
    break;

  case 260: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 621 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3772 "pascal.tab.c"
    break;

  case 261: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 622 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3778 "pascal.tab.c"
    break;

  case 262: /* handler_list: handler  */
#line 625 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3784 "pascal.tab.c"
    break;

  case 263: /* handler_list: handler_list SEMICOLON handler  */
#line 626 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3790 "pascal.tab.c"
    break;

  case 264: /* handler_list: handler_list SEMICOLON  */
#line 627 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3796 "pascal.tab.c"
    break;

  case 265: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 630 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3802 "pascal.tab.c"
    break;

  case 266: /* handler: ONSY IDENT DOSY body_stmt  */
#line 631 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3808 "pascal.tab.c"
    break;

  case 267: /* raise_statement: RAISESY  */
#line 634 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3814 "pascal.tab.c"
    break;

  case 268: /* raise_statement: RAISESY expression  */
#line 635 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3820 "pascal.tab.c"
    break;

  case 269: /* inherited_call: INHERITEDSY IDENT  */
#line 638 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3826 "pascal.tab.c"
    break;

  case 270: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 639 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3832 "pascal.tab.c"
    break;

  case 271: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 640 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3838 "pascal.tab.c"
    break;

  case 272: /* call: IDENT  */
#line 643 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3844 "pascal.tab.c"
    break;

  case 273: /* call: spec_expr  */
#line 644 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3850 "pascal.tab.c"
    break;

  case 274: /* call: selector PERIOD IDENT  */
#line 645 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3856 "pascal.tab.c"
    break;

  case 275: /* call: call_with_args  */
#line 646 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3862 "pascal.tab.c"
    break;

  case 276: /* recv_primary: STRINGCONST  */
#line 649 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3868 "pascal.tab.c"
    break;

  case 277: /* recv_primary: LPARENT expression RPARENT  */
#line 650 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3874 "pascal.tab.c"
    break;

  case 278: /* recv_primary: spec_expr  */
#line 651 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3880 "pascal.tab.c"
    break;

  case 279: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 654 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3886 "pascal.tab.c"
    break;

  case 280: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 655 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3892 "pascal.tab.c"
    break;

  case 281: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 656 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 282: /* call_with_args: IDENT LPARENT RPARENT  */
#line 657 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3904 "pascal.tab.c"
    break;

  case 283: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 658 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3910 "pascal.tab.c"
    break;

  case 284: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 659 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3916 "pascal.tab.c"
    break;

  case 285: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 660 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3922 "pascal.tab.c"
    break;

  case 286: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 661 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3928 "pascal.tab.c"
    break;

  case 287: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 662 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3934 "pascal.tab.c"
    break;

  case 288: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 663 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3940 "pascal.tab.c"
    break;

  case 289: /* argument_list: argument_list COMMA argument  */
#line 666 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3946 "pascal.tab.c"
    break;

  case 290: /* argument_list: argument  */
#line 667 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3952 "pascal.tab.c"
    break;

  case 291: /* argument: expression  */
#line 670 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3958 "pascal.tab.c"
    break;

  case 292: /* argument: expression COLON expression  */
#line 671 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3964 "pascal.tab.c"
    break;

  case 293: /* argument: expression COLON expression COLON expression  */
#line 672 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3970 "pascal.tab.c"
    break;

  case 294: /* assignment: selector BECOMES expression  */
#line 675 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3976 "pascal.tab.c"
    break;

  case 295: /* assignment: selector ADDBECOMES expression  */
#line 676 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3982 "pascal.tab.c"
    break;

  case 296: /* assignment: selector SUBBECOMES expression  */
#line 677 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3988 "pascal.tab.c"
    break;

  case 297: /* assignment: selector MULBECOMES expression  */
#line 678 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3994 "pascal.tab.c"
    break;

  case 298: /* assignment: selector DIVBECOMES expression  */
#line 679 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4000 "pascal.tab.c"
    break;

  case 299: /* assignment: call_with_args BECOMES expression  */
#line 680 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4006 "pascal.tab.c"
    break;

  case 300: /* selector: selector LBRACK expression_list RBRACK  */
#line 683 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4012 "pascal.tab.c"
    break;

  case 301: /* selector: selector PERIOD IDENT  */
#line 684 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4018 "pascal.tab.c"
    break;

  case 302: /* selector: selector ARROW  */
#line 685 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4024 "pascal.tab.c"
    break;

  case 303: /* selector: call_with_args PERIOD IDENT  */
#line 686 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4030 "pascal.tab.c"
    break;

  case 304: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 687 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4036 "pascal.tab.c"
    break;

  case 305: /* selector: call_with_args ARROW  */
#line 688 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4042 "pascal.tab.c"
    break;

  case 306: /* selector: recv_primary PERIOD IDENT  */
#line 689 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4048 "pascal.tab.c"
    break;

  case 307: /* selector: recv_primary ARROW  */
#line 690 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4054 "pascal.tab.c"
    break;

  case 308: /* selector: IDENT  */
#line 691 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4060 "pascal.tab.c"
    break;

  case 309: /* expression_list: expression_list COMMA expression  */
#line 694 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4066 "pascal.tab.c"
    break;

  case 310: /* expression_list: expression  */
#line 695 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4072 "pascal.tab.c"
    break;

  case 311: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 698 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4078 "pascal.tab.c"
    break;

  case 312: /* goto_statement: GOTOSY INTCONST  */
#line 701 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4084 "pascal.tab.c"
    break;

  case 313: /* goto_statement: GOTOSY IDENT  */
#line 702 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4090 "pascal.tab.c"
    break;

  case 314: /* if_statement: IFSY expression THENSY body_stmt  */
#line 705 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4096 "pascal.tab.c"
    break;

  case 315: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 706 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4102 "pascal.tab.c"
    break;

  case 316: /* $@21: %empty  */
#line 709 "pascal.y"
                           { }
#line 4108 "pascal.tab.c"
    break;

  case 317: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 709 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4114 "pascal.tab.c"
    break;

  case 318: /* case_tail: %empty  */
#line 712 "pascal.y"
    { (yyval.node) = NULL; }
#line 4120 "pascal.tab.c"
    break;

  case 319: /* case_tail: ELSESY statement_list  */
#line 713 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 320: /* case_tail: OTHERWISESY statement_list  */
#line 714 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4132 "pascal.tab.c"
    break;

  case 321: /* case_list: case_list SEMICOLON case_elem  */
#line 717 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4138 "pascal.tab.c"
    break;

  case 322: /* case_list: case_elem  */
#line 718 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4144 "pascal.tab.c"
    break;

  case 323: /* case_elem: constant_list COLON body_stmt  */
#line 721 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4150 "pascal.tab.c"
    break;

  case 324: /* case_elem: %empty  */
#line 722 "pascal.y"
     { (yyval.node) = NULL; }
#line 4156 "pascal.tab.c"
    break;

  case 325: /* constant_list: constant_list COMMA case_label  */
#line 725 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4162 "pascal.tab.c"
    break;

  case 326: /* constant_list: case_label  */
#line 726 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4168 "pascal.tab.c"
    break;

  case 327: /* case_label: b_expr  */
#line 729 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4174 "pascal.tab.c"
    break;

  case 328: /* case_label: b_expr DOTDOT b_expr  */
#line 730 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4180 "pascal.tab.c"
    break;

  case 329: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 733 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4186 "pascal.tab.c"
    break;

  case 330: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 736 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4192 "pascal.tab.c"
    break;

  case 331: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 739 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4198 "pascal.tab.c"
    break;

  case 332: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 740 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4204 "pascal.tab.c"
    break;

  case 333: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 741 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4210 "pascal.tab.c"
    break;

  case 334: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 744 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4216 "pascal.tab.c"
    break;

  case 335: /* with_open: with_open COMMA selector  */
#line 747 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4222 "pascal.tab.c"
    break;

  case 336: /* with_open: selector  */
#line 748 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4228 "pascal.tab.c"
    break;

  case 337: /* expression: simple_expression  */
#line 755 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4234 "pascal.tab.c"
    break;

  case 338: /* expression: expression INOP simple_expression  */
#line 756 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4240 "pascal.tab.c"
    break;

  case 339: /* expression: expression LTOP simple_expression  */
#line 757 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4246 "pascal.tab.c"
    break;

  case 340: /* expression: expression LEOP simple_expression  */
#line 758 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4252 "pascal.tab.c"
    break;

  case 341: /* expression: expression GTOP simple_expression  */
#line 759 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4258 "pascal.tab.c"
    break;

  case 342: /* expression: expression GEOP simple_expression  */
#line 760 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4264 "pascal.tab.c"
    break;

  case 343: /* expression: expression NEOP simple_expression  */
#line 761 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4270 "pascal.tab.c"
    break;

  case 344: /* expression: expression EQOP simple_expression  */
#line 762 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4276 "pascal.tab.c"
    break;

  case 345: /* expression: expression ISOP simple_expression  */
#line 763 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4282 "pascal.tab.c"
    break;

  case 346: /* simple_expression: term  */
#line 766 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4288 "pascal.tab.c"
    break;

  case 347: /* simple_expression: PLUS term  */
#line 767 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4294 "pascal.tab.c"
    break;

  case 348: /* simple_expression: MINUS term  */
#line 768 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4300 "pascal.tab.c"
    break;

  case 349: /* simple_expression: simple_expression PLUS term  */
#line 769 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4306 "pascal.tab.c"
    break;

  case 350: /* simple_expression: simple_expression MINUS term  */
#line 770 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4312 "pascal.tab.c"
    break;

  case 351: /* simple_expression: simple_expression OROP term  */
#line 771 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4318 "pascal.tab.c"
    break;

  case 352: /* simple_expression: simple_expression XOROP term  */
#line 772 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 353: /* term: power  */
#line 775 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4330 "pascal.tab.c"
    break;

  case 354: /* term: term MUL power  */
#line 776 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4336 "pascal.tab.c"
    break;

  case 355: /* term: term RDIV power  */
#line 777 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4342 "pascal.tab.c"
    break;

  case 356: /* term: term IDIV power  */
#line 778 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4348 "pascal.tab.c"
    break;

  case 357: /* term: term IMOD power  */
#line 779 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4354 "pascal.tab.c"
    break;

  case 358: /* term: term ANDOP power  */
#line 780 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 359: /* term: term MUL MINUS power  */
#line 781 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4366 "pascal.tab.c"
    break;

  case 360: /* term: term RDIV MINUS power  */
#line 782 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4372 "pascal.tab.c"
    break;

  case 361: /* term: term IDIV MINUS power  */
#line 783 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4378 "pascal.tab.c"
    break;

  case 362: /* term: term IMOD MINUS power  */
#line 784 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4384 "pascal.tab.c"
    break;

  case 363: /* term: term ANDOP MINUS power  */
#line 785 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4390 "pascal.tab.c"
    break;

  case 364: /* term: term SHIFTOP power  */
#line 786 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4396 "pascal.tab.c"
    break;

  case 365: /* term: term ASOP power  */
#line 787 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4402 "pascal.tab.c"
    break;

  case 366: /* power: factor  */
#line 790 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4408 "pascal.tab.c"
    break;

  case 367: /* power: power POWOP factor  */
#line 791 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4414 "pascal.tab.c"
    break;

  case 368: /* factor: selector  */
#line 794 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4420 "pascal.tab.c"
    break;

  case 369: /* factor: call_with_args  */
#line 795 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4426 "pascal.tab.c"
    break;

  case 370: /* factor: INTCONST  */
#line 796 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4432 "pascal.tab.c"
    break;

  case 371: /* factor: REALCONST  */
#line 797 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4438 "pascal.tab.c"
    break;

  case 372: /* factor: STRINGCONST  */
#line 798 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4444 "pascal.tab.c"
    break;

  case 373: /* factor: CHARCODE  */
#line 799 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4450 "pascal.tab.c"
    break;

  case 374: /* factor: LPARENT expression RPARENT  */
#line 800 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4456 "pascal.tab.c"
    break;

  case 375: /* factor: inherited_call  */
#line 801 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4462 "pascal.tab.c"
    break;

  case 376: /* factor: NOTSY factor  */
#line 802 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4468 "pascal.tab.c"
    break;

  case 377: /* factor: ATSIGN factor  */
#line 803 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4474 "pascal.tab.c"
    break;

  case 378: /* factor: LBRACK RBRACK  */
#line 804 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4480 "pascal.tab.c"
    break;

  case 379: /* factor: LBRACK set_member_list RBRACK  */
#line 805 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4486 "pascal.tab.c"
    break;

  case 380: /* set_member_list: set_member  */
#line 808 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4492 "pascal.tab.c"
    break;

  case 381: /* set_member_list: set_member_list COMMA set_member  */
#line 809 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4498 "pascal.tab.c"
    break;

  case 382: /* set_member: expression  */
#line 812 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4504 "pascal.tab.c"
    break;

  case 383: /* set_member: expression DOTDOT expression  */
#line 813 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4510 "pascal.tab.c"
    break;


#line 4514 "pascal.tab.c"

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

#line 819 "pascal.y"

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
