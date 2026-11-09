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
#define YYLAST   1645

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  388
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  901

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
     539,   540,   543,   544,   545,   546,   547,   550,   551,   561,
     562,   563,   566,   567,   570,   571,   572,   573,   574,   575,
     578,   579,   582,   585,   586,   589,   592,   592,   593,   593,
     596,   596,   599,   600,   601,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     621,   622,   623,   624,   627,   628,   629,   632,   633,   636,
     637,   640,   641,   642,   645,   646,   647,   648,   651,   652,
     653,   656,   657,   658,   659,   660,   661,   662,   663,   664,
     665,   666,   667,   670,   671,   674,   675,   676,   677,   680,
     681,   682,   683,   684,   685,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   699,   700,   703,   706,   707,   710,
     711,   714,   714,   717,   718,   719,   722,   723,   726,   727,
     730,   731,   734,   735,   738,   741,   744,   745,   746,   749,
     752,   753,   760,   761,   762,   763,   764,   765,   766,   767,
     768,   771,   772,   773,   774,   775,   776,   777,   780,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   795,   796,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   813,   814,   817,   818
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

#define YYPACT_NINF (-558)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-314)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     210,     2,    12,   153,  -558,   107,   343,  -558,  -558,   400,
    -558,    67,  -558,  -558,   512,   110,    12,   178,   214,   214,
     230,  -558,   133,    33,   250,   377,   273,  -558,  -558,  -558,
     393,   214,   445,   521,   474,  -558,  -558,    76,   177,   110,
    -558,    77,   444,   227,  -558,   214,  -558,   491,   104,   312,
     258,   737,   563,   483,   133,  -558,   498,   552,   577,   596,
     582,   590,  -558,   416,  -558,  -558,  -558,  -558,   600,  1187,
     905,  -558,  -558,    12,   214,   597,   629,  -558,   625,   905,
    -558,   214,   634,    80,  -558,  -558,   605,   640,  -558,  1187,
     204,  1187,  -558,  1187,  1187,   678,  -558,   553,  -558,  1187,
     666,   674,  -558,   509,  -558,  -558,  -558,  -558,  -558,  -558,
    -558,   546,   170,  -558,  1393,  -558,  -558,  -558,  -558,  -558,
    -558,  -558,  -558,   214,   946,   657,  -558,   694,   700,   690,
     693,   724,   725,  -558,   721,   935,  -558,  -558,  1410,  1410,
    1410,   996,  1187,  1410,  -558,  -558,  -558,   623,    -3,   666,
     717,  -558,   601,   473,    79,   309,   381,   712,  -558,   720,
    -558,   720,   743,   743,   147,   718,  -558,  -558,  -558,  -558,
    1276,   735,  -558,  -558,  -558,  -558,   772,   315,  -558,   407,
     296,   444,    15,  -558,    32,   816,  -558,    47,   289,  -558,
    -558,   776,   737,  -558,  -558,   645,   410,   754,   601,   473,
     203,   280,   282,  1457,  1474,   819,   819,  1012,  1439,    22,
     548,   773,   784,  1038,   765,  -558,  1187,   770,  -558,  1187,
    1075,  1187,   779,  -558,  1187,  1187,  1187,  1187,  1187,   304,
      85,   843,   214,   785,   786,  -558,  -558,   214,   214,  -558,
     110,    12,  -558,   787,   214,   790,   133,  -558,  -558,   381,
     381,  -558,  1492,   427,  -558,  1511,  -558,   791,  -558,  1187,
    1187,  1187,  1187,  1187,  1187,  1187,  1187,  1410,  1410,  1410,
    1410,  1282,  1308,  1334,  1350,  1384,  1410,  1410,  1410,   179,
     821,   808,   806,   296,   296,   155,  -558,  -558,   448,  -558,
    1439,   795,  1493,  1493,   345,    57,  1213,   196,   844,   845,
    -558,   451,   451,  1493,   451,   451,   451,   451,  -558,   629,
     214,   798,   214,   799,  -558,   214,    31,  -558,   644,   870,
    -558,  1213,   804,  -558,   629,  -558,   360,  -558,  1187,  1187,
    1187,  -558,  -558,   204,  -558,  -558,  -558,  -558,   855,  -558,
    -558,  -558,   504,  -558,   637,  1529,     5,  -558,   793,  -558,
    1091,  1439,  -558,   506,   817,   548,   824,   568,   548,  -558,
     527,   548,   925,   574,   548,   548,   548,   548,   838,  -558,
     345,  -558,   335,  -558,  -558,   629,   629,   364,   446,   110,
     159,   850,  -558,   214,  -558,   133,  1187,  1187,  -558,   647,
     828,   309,   309,   309,   309,   309,   309,   309,   309,   381,
     381,   381,   381,  1410,   712,  1410,   712,  1410,   712,  1410,
     712,  1410,   712,   712,   712,  -558,   214,   827,   214,   831,
    -558,   214,    55,  -558,   651,   832,   360,   718,  -558,   459,
    -558,   451,   451,  1493,   840,  -558,   437,   374,  -558,   413,
     155,   528,  -558,  -558,   833,  -558,   834,    97,   319,   835,
     836,   842,  -558,   668,  1229,   915,   548,   905,  1439,   905,
    1439,   214,   374,   374,   340,  -558,  -558,  -558,  -558,   629,
     916,   673,   720,   681,   720,   685,   116,  -558,     4,   922,
     852,   928,   929,   629,   875,   931,   148,  -558,   939,   359,
     848,   548,  -558,   737,  -558,   473,  1493,   899,  1187,  -558,
    1187,  1187,  1439,  -558,   873,   284,   237,  -558,   353,  -558,
     529,   481,  -558,  1117,  1133,  1187,  -558,  -558,  1159,  -558,
     905,   545,   893,   629,   629,   947,   948,  -558,  -558,  -558,
    -558,  -558,   936,   629,   629,   548,  -558,   712,   712,   712,
     712,   712,   691,   720,   705,   720,   727,   162,  -558,     4,
    -558,   930,  -558,  -558,   374,   374,   456,  1493,   451,   451,
    -558,   451,   451,   451,   451,  -558,   345,  -558,  -558,  -558,
    -558,   881,   888,  -558,  -558,   903,   905,   175,   910,    71,
    1511,  -558,  -558,   612,  -558,  -558,   953,    66,  -558,   740,
     971,  -558,     4,  -558,     4,   932,     4,  -558,   960,   151,
    -558,   937,  -558,   986,  -558,  -558,  -558,   942,   360,  -558,
     927,   995,  1187,  1187,  -558,  -558,   421,   266,  -558,   750,
    -558,  -558,  -558,  1545,  1564,  -558,   141,  -558,   793,  -558,
    -558,  -558,  -558,  -558,  -558,   560,  -558,   588,   548,  -558,
     592,  1000,  1001,   905,  1003,   949,  -558,  -558,   629,   629,
    1009,  1009,  -558,   985,  1011,   957,     4,  -558,     4,   964,
       4,  -558,  -558,   951,  -558,   502,   374,   374,  -558,  -558,
    -558,  -558,  -558,   176,   629,   629,  -558,  -558,   629,   629,
     116,   965,  1023,  1213,  1213,   352,  -558,  1439,  1007,  -558,
     214,   961,  -558,  -558,  -558,   852,   988,  -558,   360,  -558,
      36,   962,  1187,  -558,  1043,   360,  1044,  -558,  1047,   852,
     981,   994,  -558,  1493,  1493,  -558,  -558,  1024,  1493,  -558,
    -558,  1187,  1187,  -558,   977,  -558,   354,  -558,  -558,  -558,
    -558,  -558,  1051,  -558,   360,   852,   852,  1052,  1004,  1009,
    1002,  -558,   360,  -558,  -558,   360,  -558,  -558,  -558,  -558,
    1060,  1013,   629,   629,  1073,  1077,   100,   345,  -558,  -558,
    -558,   594,  -558,  1029,  1028,  -558,  -558,   165,   905,   852,
    -558,  1187,  -558,  -558,  -558,  -558,   548,  -558,  1089,  -558,
    -558,  -558,  -558,  -558,   340,  -558,  1009,  1009,  -558,  -558,
    -558,   495,   548,  -558,  1078,  -558,  -558,   852,  1092,  -558,
    -558,  -558,   360,  -558,   852,  1093,  -558,  -558,   360,  1096,
    1036,  -558,  -558,  -558,  -558,  1213,  -558,  1213,  1076,  -558,
    -558,  1031,  -558,  -558,  1103,   548,  -558,   852,  1104,  -558,
    -558,  -558,  -558,  -558,  -558,  1110,  1111,   852,  1112,  -558,
    1114,  -558,   360,    46,  -558,  -558,   905,   905,  1493,  1094,
    1115,  -558,   852,  -558,  -558,  -558,  1116,   852,  -558,  -558,
    -558,  -558,   852,  -558,  1118,  -558,   618,  -558,  -558,  1119,
    -558,   768,  -558,  -558,  -558,  1123,  -558,  -558,  1126,   852,
    -558,  -558,  -558,  1493,  1069,  1493,  1130,  -558,  1131,  -558,
    -558,  -558,  -558,  1119,  -558,  -558,  1138,   214,  -558,  1079,
    -558
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   238,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   198,   234,    16,   166,     0,     0,   198,
       0,   259,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     7,     0,    12,    22,    65,    13,     0,     0,
     119,    38,    18,     0,     0,     0,   201,   165,     0,   119,
      19,     0,     0,   201,   236,   235,     0,     0,   238,     0,
       0,     0,   238,     0,     0,     0,   278,   274,   238,   269,
     258,     0,    68,   275,    67,   239,   242,   255,   256,   257,
     246,     0,   277,   245,     0,   247,   248,   249,   250,   251,
     252,   253,   254,     0,   119,     0,    55,     0,     0,     0,
       0,     0,     0,     8,     0,     0,    34,    35,     0,     0,
       0,     0,     0,     0,   375,   378,   376,   377,   313,     0,
     280,   380,   374,   373,     0,   342,   351,   358,   371,   221,
     118,   221,     0,     0,     0,     0,    51,    54,    52,    53,
     121,   101,   131,    75,    74,   124,     0,     0,    79,     0,
     126,    63,     0,   198,     0,     0,   233,     0,     0,   198,
     176,     0,   259,   317,   318,     0,     0,     0,     0,   341,
       0,     0,     0,     0,     0,   259,   259,     0,     0,     0,
     270,   271,     0,     0,     0,   312,     0,     0,   310,     0,
       0,     0,     0,   307,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,   198,   198,     0,     0,     4,
       0,     0,    12,     0,     0,     0,     0,    21,   381,   352,
     353,   383,   387,     0,   385,     0,   382,     0,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,    50,   127,   128,    80,    81,   232,     0,   231,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
      91,     0,     0,     0,     0,     0,     0,     0,   198,   201,
       0,     0,     0,     0,   200,   234,     0,   203,     0,   217,
     167,     0,     0,   198,   201,    12,     0,   237,     0,     0,
       0,   240,   240,     0,   321,   316,   240,   279,   274,   243,
     244,   284,     0,   294,   373,   295,     0,    77,   238,   238,
       0,     0,   283,     0,   311,   304,   308,     0,   315,   286,
       0,   299,   276,     0,   300,   301,   302,   303,     0,    60,
       0,    57,     0,   198,   198,   201,   201,     0,     0,    24,
       0,    31,   198,    26,   198,    25,     0,     0,   384,   379,
     306,   343,   344,   345,   346,   347,   348,   349,   350,   356,
     354,   355,   357,     0,   361,     0,   362,     0,   363,     0,
     359,     0,   360,   369,   370,   372,     0,     0,     0,     0,
     220,   234,     0,   223,     0,     0,     0,     0,   120,     0,
     123,     0,     0,     0,    50,   147,     0,   137,   142,     0,
      71,     0,    69,    72,     0,    96,     0,   103,     0,     0,
       0,     0,   102,     0,     0,     0,    42,   119,     0,   119,
       0,   155,   129,   130,   125,   134,   135,   133,   136,   201,
       0,     0,   221,     0,   221,     0,     0,   199,     0,     0,
     172,     0,     0,   201,     0,     0,    98,   100,     0,     0,
       0,   335,   334,   259,   339,   340,   329,   319,     0,   281,
       0,     0,     0,    66,     0,     0,     0,   264,     0,   272,
       0,     0,   282,     0,     0,     0,   309,   285,     0,   305,
     119,     0,     0,   201,   201,     0,     0,   198,   198,    23,
     238,   238,    33,   201,   201,   388,   386,   366,   367,   368,
     364,   365,     0,   221,     0,   221,     0,     0,   219,     0,
      87,    88,   230,    76,   138,   139,     0,     0,     0,     0,
     122,     0,     0,     0,     0,   132,     0,   101,   198,   198,
     104,     0,     0,   198,   198,   113,   119,   313,     0,     0,
      42,    41,    85,     0,    95,    93,     0,   160,   152,     0,
       0,   217,     0,   204,     0,     0,     0,   202,     0,   212,
     216,   207,   170,     0,    12,   168,   169,     0,     0,   177,
       0,   217,     0,     0,   240,   241,   332,   323,   327,     0,
     331,   240,   293,   315,   296,    78,     0,   260,   266,   261,
     238,   263,   273,    73,   292,     0,   290,     0,   314,   288,
       0,     0,   101,   119,     0,     0,   217,   217,   201,   201,
      29,    30,   238,     0,     0,     0,     0,   228,     0,     0,
       0,   222,   224,     0,   149,     0,   140,   141,   144,   145,
     143,   146,    70,     0,   201,   201,   198,   198,   201,   201,
       0,     0,     0,     0,     0,     0,    44,     0,     0,    92,
     155,     0,   150,   153,   217,   186,   208,   206,     0,   211,
       0,     0,     0,   218,     0,     0,     0,    99,     0,   174,
       0,     0,   338,     0,   329,   238,   238,     0,     0,   240,
     320,     0,     0,   240,     0,   265,     0,   291,   289,   287,
      59,    61,     0,   217,     0,   190,   192,     0,     0,    32,
       0,   217,     0,   226,   225,     0,   227,    89,   148,    97,
       0,     0,   201,   201,     0,     0,     0,     0,   105,    48,
      46,     0,    45,     0,     0,    82,   151,     0,   119,   182,
      12,     0,   205,   215,   214,   213,   209,   173,     0,   217,
     171,    12,   240,   240,   333,   326,   325,   324,   322,   330,
     328,     0,   297,   268,     0,   262,    58,   194,     0,    12,
      12,   217,     0,    20,    27,     0,   229,   106,     0,     0,
       0,   108,   109,   114,   115,     0,    43,     0,     0,    83,
     157,     0,   154,    12,     0,   210,   217,   188,     0,   337,
     336,   298,   240,    12,   217,     0,     0,   178,     0,   217,
       0,   110,     0,     0,    47,    49,   119,   119,   164,     0,
       0,   187,   184,    12,   175,   267,     0,   196,   191,   193,
      12,   217,    28,   107,     0,   112,     0,    90,    84,   159,
     162,     0,   157,   183,    12,     0,   195,    12,     0,   180,
     111,   117,   116,   164,     0,   164,     0,   189,     0,   179,
      12,   161,   156,   158,   185,   197,     0,   155,   181,     0,
     163
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -558,  -558,  -558,  -558,  -558,   -63,   896,  -558,  -558,  -558,
    -558,  -558,  -558,  -558,   912,   -36,  -316,  -558,  -558,   -68,
     908,   -48,   918,    52,   -54,   -33,   800,  -557,   -13,   -78,
    -248,   -71,  -558,  -558,  -558,   -56,  -543,  -558,  -558,  -558,
    -558,  -201,  -558,   572,  -272,  -264,  -259,   260,  -558,   471,
    -558,  -558,   301,  -558,   283,   291,   933,   -41,  -558,  -558,
    -558,  -558,  -558,  -558,  -558,  -558,  -558,  -558,  -558,  -558,
    -558,  -558,    -8,   -40,   500,   699,  -517,   136,  -147,  -558,
     636,  -558,   759,   -19,  -558,   -75,  -558,  -558,  -303,  -558,
    -170,   517,  -558,  -558,   566,  -558,   -21,  -558,  -558,   -23,
    -194,   698,  -558,   -16,   980,  -558,  -558,  -558,  -558,  -558,
    -558,  -558,   487,  -481,   485,  -558,  -558,  -558,  -558,  -558,
     626,  1368,   -90,  -158,  -104,  -558,   823
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   135,
     247,   532,   653,    37,    39,    40,   455,   761,   579,   435,
      54,    55,    41,    42,   102,   150,   441,   442,   104,   175,
     346,   176,   819,   847,   461,   488,   295,   452,   681,   843,
     177,   178,   179,   180,   616,   437,   438,   586,   587,   588,
     768,   897,   848,   692,   869,   870,    45,    46,    28,   604,
     781,   325,   860,   890,   823,   874,   770,   853,   799,   800,
     833,   877,    76,   185,   316,   317,   601,   480,   280,   422,
     423,   288,   289,    47,    29,    50,   192,    51,   492,   493,
     105,   106,   107,   506,   507,   108,   151,   110,   111,   152,
     342,   343,   113,   153,   363,   115,   116,   117,   118,   496,
     717,   617,   618,   871,   620,   119,   120,   121,   122,   200,
     345,   155,   156,   157,   158,   253,   254
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      48,   134,   172,    71,    77,   481,   126,   347,   187,   672,
     598,   172,    63,   196,   281,   619,   173,   202,   103,   353,
     436,   439,   327,   209,   673,   173,   360,    84,   112,   494,
     109,   464,   662,   497,   248,   114,   476,   462,   463,   256,
     310,    83,   429,   191,   773,   465,   466,   467,   468,   249,
     250,   865,   320,   231,    11,   182,   172,   174,   311,    56,
     547,   207,   188,   502,   312,   308,   174,   198,   313,    57,
     173,   690,    33,    78,   199,   696,   685,   697,     8,   699,
     599,    67,    72,   444,   258,   190,   286,   445,    10,   347,
     369,   691,   208,   446,   172,   172,   477,   314,   101,   673,
     321,   503,   570,   511,   229,   476,   348,   349,   315,    80,
     173,   174,   774,   404,   406,   408,   410,   412,   413,   414,
     548,   259,   866,   322,   310,   181,    34,   260,   261,   262,
     263,   264,   265,   447,    68,    73,   686,   448,   578,   743,
     172,   744,   311,   746,   184,   449,   450,   451,   312,   370,
     347,   174,   313,    12,   173,  -234,   510,  -234,   723,   103,
     266,   556,    78,   813,   529,   318,    13,   554,   555,   112,
     416,   109,   103,   103,   415,   309,   114,   399,   400,   401,
     402,   324,   112,   112,   109,   109,    38,   416,   417,   114,
     114,   344,   315,   820,   418,   174,   290,   344,   419,   290,
     814,   724,   444,   290,   344,   417,   749,   610,   216,    52,
     701,   418,   446,   372,     1,   419,   443,    73,   377,   378,
     332,     2,   172,   285,   457,   821,    53,   375,   376,   217,
      69,   218,   219,   619,   220,   683,   173,    70,   421,   207,
     173,   101,   628,   208,   420,   537,   208,   538,   487,   539,
     208,   540,   447,   541,    43,   421,   448,   583,   458,   585,
     424,   333,   485,    84,   449,   450,   451,   629,    94,   470,
     208,   714,   173,   505,   508,    74,   453,   174,   630,    96,
     148,   174,   715,   172,   484,   665,    75,    84,    85,    84,
      44,   471,   443,   473,   666,   667,   475,   173,   101,    60,
     469,   625,   668,   669,   670,   671,    49,   716,   334,    61,
     198,   712,   335,   174,   627,   483,   173,   495,   720,   635,
     637,   297,   259,   615,   640,   593,    58,   595,   260,   261,
     262,   263,   264,   265,   344,   525,   526,   126,   174,   323,
     304,   305,    77,    71,   298,   571,   299,    78,   487,   300,
      14,    15,   306,    16,   368,   572,   267,   174,    84,    84,
      81,   266,    78,   268,   269,   523,   524,   759,   760,    17,
     551,    82,   173,   307,   533,    18,   534,   612,    19,    20,
      21,   613,    22,   631,   795,   522,   582,   270,   584,   172,
     172,   172,   172,    78,   558,   559,   657,   542,   659,   544,
     600,   259,   546,   173,   173,   173,   173,   260,   261,   262,
     263,   264,   265,   174,   527,    84,   790,   762,   561,   562,
     793,   440,    78,    23,   173,   271,   272,   273,   763,   590,
     563,    24,    25,   330,   172,    26,   486,   274,   275,   101,
     266,   784,   589,   607,   174,   174,   174,   174,   173,   641,
      64,   564,   172,    59,   101,   650,   651,   318,   276,    30,
     103,   301,   302,   277,    31,   174,   173,   558,   559,    62,
     112,   600,   109,   303,    78,   558,   559,   114,   565,   829,
     830,   133,   344,   644,   645,   387,   764,   713,   443,   174,
     388,   558,   559,   654,   655,   173,   528,   344,   344,   844,
     560,   845,   344,    34,    78,   682,   427,   174,   172,   553,
     558,   559,   173,   428,   600,   433,   600,   502,   600,   648,
     649,   664,   173,   166,   167,   168,   169,   434,   424,   855,
     487,   633,   257,    65,   223,   224,   174,   259,   173,   502,
     173,   704,   173,   260,   261,   262,   263,   264,   265,    78,
      66,    79,   706,   174,   173,   726,   558,   559,   831,   125,
     674,   675,   498,   174,   498,   678,   679,   748,  -280,   499,
    -280,   512,   732,   213,   127,   172,   266,   739,   600,   174,
     600,   174,   600,   174,    35,   498,   566,   498,    36,   173,
     259,  -313,   517,   567,   632,   174,   260,   261,   262,   263,
     264,   265,   173,   566,   173,   214,   173,   215,   737,   738,
     642,   123,  -313,   206,  -313,  -313,   124,   207,   498,   172,
     487,  -313,  -313,  -313,  -313,   727,   515,   487,   128,   266,
     174,   516,   515,   173,   750,   751,   129,   519,   754,   755,
     786,   787,   772,   174,   173,   174,   498,   174,   208,   778,
     498,   173,   815,   728,   453,   130,   487,   729,   131,   816,
     217,   318,   218,   219,   487,   220,   132,   487,   752,   753,
     687,   589,   136,   183,   174,   688,   137,   193,   798,   443,
     173,   194,  -278,   328,  -278,   174,   805,   329,   173,   806,
     881,   173,   174,   184,   882,   154,   257,   822,   223,   500,
     172,   186,    78,   173,   478,   232,  -279,   824,  -279,    78,
     189,   549,   809,   810,   173,   197,   195,   201,   828,   203,
     204,   174,   339,   340,   487,   210,    78,   695,   576,   174,
     487,    78,   174,   592,   283,   284,   835,   836,   205,    78,
      86,   594,   211,    78,   174,   596,   838,   709,   173,    78,
     212,   656,   840,   233,   173,   174,    87,    88,    89,   234,
     850,    90,    91,    78,   487,   658,   235,   252,   255,   236,
     856,   331,   237,   238,    92,   867,   868,    93,   172,   172,
     239,   213,   735,   736,   279,    78,   864,   660,   173,   174,
     875,   278,   173,   173,   287,   174,   259,   878,    78,   294,
     693,    94,   260,   261,   262,   263,   264,   265,   718,    95,
     719,   886,    96,    97,   888,   166,   167,   168,   169,   282,
      98,   319,    86,    99,   100,   296,   718,   896,   884,   174,
     769,   101,   351,   174,   174,   266,   326,   350,    87,    88,
      89,   354,   355,    90,    91,   358,   356,   361,   371,   425,
     358,   364,   365,   366,   367,   362,    92,    14,    15,    93,
      16,   373,   374,   382,   530,   614,   384,   390,   426,   797,
     293,   430,   459,   460,   472,   474,    17,   804,   589,   479,
     482,   513,    18,    94,   504,    19,    20,   531,   514,    22,
     259,   520,   518,  -313,    96,   338,   260,   261,   262,   263,
     264,   265,    98,   543,   557,    99,   100,   545,   550,   568,
     569,   573,   574,   101,  -313,   827,  -313,  -313,   575,   207,
     581,   591,   456,  -313,  -313,  -313,  -313,   602,   603,   266,
      23,   159,   160,   605,   606,   608,   609,   837,    24,    25,
     621,   161,    26,   240,   611,   241,   643,   456,   242,   626,
     208,   652,   646,   647,   489,   490,   491,   676,   663,   162,
     163,   243,   852,  -306,   677,   680,   164,   244,   684,   165,
     857,   245,   159,   160,   246,   862,   694,   166,   167,   168,
     169,   170,   161,   689,  -306,   171,  -306,  -306,   700,   518,
     702,   703,   698,  -306,  -306,  -306,  -306,   879,   782,   101,
     162,   163,   705,   707,   708,   730,   731,   164,   733,   734,
     165,   783,   535,   252,    84,   740,   741,   742,   166,   167,
     168,   169,   170,   259,   745,   757,   230,   747,   758,   260,
     261,   262,   263,   264,   265,   765,   259,   767,   775,   138,
     101,   771,   260,   261,   262,   263,   264,   265,   777,   779,
     139,   140,   780,   794,   788,   138,   796,   801,   141,   251,
     142,   803,   266,   143,   802,   807,   139,   140,   144,   145,
     146,   147,   148,   808,   141,   266,   142,   341,   811,   143,
     580,   138,   812,   149,   144,   145,   146,   147,   148,   817,
     101,   818,   139,   140,   826,   832,   842,   834,   839,   149,
     141,   841,   142,   352,   846,   143,   101,   849,   851,   854,
     144,   145,   146,   147,   148,   858,   859,   861,   138,   863,
     873,   876,   872,   880,   883,   149,   623,   624,   887,   139,
     140,   889,   101,   892,   138,   894,   895,   141,   381,   142,
     359,   638,   143,   898,   900,   139,   140,   144,   145,   146,
     147,   148,   379,   141,   385,   142,   509,   899,   143,   380,
     138,   766,   149,   144,   145,   146,   147,   148,   893,   101,
     521,   139,   140,   885,   891,   597,   138,   383,   149,   141,
     756,   142,   634,   661,   143,   101,   552,   139,   140,   144,
     145,   146,   147,   148,   725,   141,   622,   142,   636,   357,
     143,   785,   138,   789,   149,   144,   145,   146,   147,   148,
     536,   101,     0,   139,   140,     0,     0,     0,     0,     0,
     149,   141,     0,   142,   639,     0,   143,   101,     0,     0,
     138,   144,   145,   146,   147,   148,     0,     0,   710,   711,
       0,   139,   140,     0,     0,     0,   149,     0,     0,   141,
       0,   142,     0,   101,   143,     0,   138,     0,     0,   144,
     145,   146,   147,   148,     0,     0,     0,   139,   140,     0,
       0,     0,   138,     0,   149,   141,     0,   454,     0,     0,
     143,   101,     0,   139,   140,   144,   145,   146,   147,   148,
       0,   141,     0,   454,     0,     0,   143,     0,     0,     0,
     149,   144,   145,   146,   147,   577,     0,   101,     0,   456,
     456,     0,     0,     0,     0,     0,   149,     0,     0,     0,
     -50,   -50,     0,   101,   290,   138,     0,     0,   776,     0,
     -50,   -50,   -50,     0,     0,   291,     0,   403,   292,     0,
     293,     0,   -50,     0,   141,     0,   142,   791,   792,   143,
       0,   138,     0,   -50,   144,   145,   146,   147,   148,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,   149,
     141,   208,   142,     0,     0,   143,   101,   138,     0,     0,
     144,   145,   146,   147,   148,     0,     0,     0,     0,   407,
       0,     0,     0,   138,     0,   149,   141,   825,   142,     0,
       0,   143,   101,     0,     0,   409,   144,   145,   146,   147,
     148,     0,   141,     0,   142,     0,     0,   143,     0,     0,
       0,   149,   144,   145,   146,   147,   148,   138,   101,     0,
       0,   221,     0,     0,     0,     0,     0,   149,     0,   411,
       0,   456,     0,   456,   101,     0,   141,     0,   142,     0,
       0,   143,   222,   138,   223,   224,   144,   145,   146,   147,
     148,   225,   226,   227,   228,     0,     0,     0,     0,     0,
       0,   149,   141,     0,   142,     0,     0,   143,   101,     0,
       0,     0,   144,   145,   146,   147,   148,     0,     0,     0,
     336,     0,     0,   162,   163,     0,     0,   149,     0,   259,
       0,     0,     0,   165,   101,   260,   261,   262,   263,   264,
     265,   166,   167,   168,   169,   170,   259,     0,     0,     0,
       0,     0,   260,   261,   262,   263,   264,   265,     0,     0,
       0,     0,     0,   101,   259,     0,     0,     0,   266,   337,
     260,   261,   262,   263,   264,   265,     0,   431,   432,     0,
       0,     0,     0,   259,     0,   266,     0,   433,   386,   260,
     261,   262,   263,   264,   265,   166,   167,   168,   169,   434,
       0,   259,     0,   266,     0,     0,   389,   260,   261,   262,
     263,   264,   265,     0,     0,     0,     0,   259,     0,   501,
       0,     0,   266,   260,   261,   262,   263,   264,   265,     0,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
     266,   721,   260,   261,   262,   263,   264,   265,     0,     0,
       0,     0,     0,     0,   722,     0,   266,   391,   392,   393,
     394,   395,   396,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   266
};

static const yytype_int16 yycheck[] =
{
      19,    64,    70,    39,    45,   321,    54,   208,    79,   566,
       6,    79,    31,    88,   161,   496,    70,    92,    51,   213,
     292,   293,   192,    98,   567,    79,   220,     5,    51,   332,
      51,   303,   549,   336,   138,    51,     5,   301,   302,   143,
       8,    49,   290,    83,     8,   304,   305,   306,   307,   139,
     140,     5,     5,   124,     2,    74,   124,    70,    26,    26,
       5,    64,    81,    58,    32,    50,    79,    90,    36,    36,
     124,     5,     5,    58,    90,   592,     5,   594,    76,   596,
      76,     5,     5,    26,     5,     5,   164,    30,    76,   290,
       5,    25,    95,    36,   162,   163,    65,    65,    94,   642,
      53,    96,     5,   351,   123,     5,    84,    85,    76,     5,
     164,   124,    76,   271,   272,   273,   274,   275,   276,   277,
      65,    42,    76,    76,     8,    73,    59,    48,    49,    50,
      51,    52,    53,    76,    58,    58,    65,    80,   454,   656,
     208,   658,    26,   660,    64,    88,    89,    90,    32,    64,
     351,   164,    36,     0,   208,    58,   350,    60,    17,   192,
      81,   433,    58,    63,     5,   184,    59,   431,   432,   192,
       8,   192,   205,   206,   278,   183,   192,   267,   268,   269,
     270,   189,   205,   206,   205,   206,    76,     8,    26,   205,
     206,   207,    76,    28,    32,   208,    48,   213,    36,    48,
     757,    60,    26,    48,   220,    26,    30,    59,    38,    76,
      59,    32,    36,   232,     4,    36,   294,    58,   237,   238,
      17,    11,   290,    76,    28,    60,    93,   235,   236,    59,
      53,    61,    62,   714,    64,    60,   290,    60,    76,    64,
     294,    94,     5,    95,    65,   403,    95,   405,   326,   407,
      95,   409,    76,   411,    76,    76,    80,   458,    62,   460,
     279,    58,   325,     5,    88,    89,    90,    30,    64,   309,
      95,     5,   326,   348,   349,    48,   295,   290,    41,    75,
      76,   294,    16,   351,   324,   557,    59,     5,    30,     5,
      76,   310,   370,   312,   558,   559,   315,   351,    94,    26,
     308,   502,   561,   562,   563,   564,    76,    41,    28,    36,
     333,   614,    30,   326,    30,   323,   370,   333,   621,   513,
     514,     6,    42,   493,   518,   472,    76,   474,    48,    49,
      50,    51,    52,    53,   350,   375,   376,   385,   351,    50,
      44,    45,   383,   379,    29,    26,    31,    58,   426,    34,
       7,     8,    56,    10,    50,    36,    47,   370,     5,     5,
      48,    81,    58,    54,    55,   373,   374,   683,   684,    26,
     426,    59,   426,    77,   382,    32,   384,    18,    35,    36,
      37,    22,    39,    30,    30,    50,   457,    78,   459,   457,
     458,   459,   460,    58,    54,    55,   543,   416,   545,   418,
     478,    42,   421,   457,   458,   459,   460,    48,    49,    50,
      51,    52,    53,   426,    50,     5,   719,    65,    44,    45,
     723,    76,    58,    80,   478,    44,    45,    46,    76,   469,
      56,    88,    89,    23,   502,    92,    76,    56,    57,    94,
      81,   713,   461,   483,   457,   458,   459,   460,   502,   520,
       5,    77,   520,    76,    94,   530,   531,   476,    77,    59,
     493,    54,    55,    82,    64,   478,   520,    54,    55,    76,
     493,   549,   493,    66,    58,    54,    55,   493,    65,   782,
     783,    65,   498,   523,   524,    58,   687,    66,   566,   502,
      63,    54,    55,   533,   534,   549,    50,   513,   514,   815,
      63,   817,   518,    59,    58,   576,    58,   520,   576,    50,
      54,    55,   566,    65,   592,    64,   594,    58,   596,   527,
     528,    65,   576,    72,    73,    74,    75,    76,   547,   832,
     608,    50,    59,    12,    61,    62,   549,    42,   592,    58,
     594,   604,   596,    48,    49,    50,    51,    52,    53,    58,
      76,    60,   608,   566,   608,   630,    54,    55,    63,    76,
     568,   569,    58,   576,    58,   573,   574,    65,    59,    65,
      61,    65,   643,    64,    76,   643,    81,   652,   656,   592,
     658,   594,   660,   596,    72,    58,    58,    58,    76,   643,
      42,    38,    65,    65,    65,   608,    48,    49,    50,    51,
      52,    53,   656,    58,   658,    59,   660,    61,   648,   649,
      65,    48,    59,    60,    61,    62,    53,    64,    58,   687,
     698,    68,    69,    70,    71,    65,    58,   705,    76,    81,
     643,    63,    58,   687,   674,   675,    59,    63,   678,   679,
     715,   716,   698,   656,   698,   658,    58,   660,    95,   705,
      58,   705,    58,    65,   673,    59,   734,    65,    76,    65,
      59,   680,    61,    62,   742,    64,    76,   745,   676,   677,
      58,   690,    72,    76,   687,    63,    76,    72,   734,   757,
     734,    76,    59,    38,    61,   698,   742,    42,   742,   745,
      72,   745,   705,    64,    76,    69,    59,   768,    61,    62,
     768,    76,    58,   757,    60,    48,    59,   770,    61,    58,
      76,    60,   752,   753,   768,    89,    76,    91,   781,    93,
      94,   734,   205,   206,   802,    99,    58,   591,    60,   742,
     808,    58,   745,    60,   162,   163,   799,   800,    60,    58,
       3,    60,    76,    58,   757,    60,   802,   611,   802,    58,
      76,    60,   808,    59,   808,   768,    19,    20,    21,    59,
     823,    24,    25,    58,   842,    60,    76,   141,   142,    76,
     833,    17,    48,    48,    37,   846,   847,    40,   846,   847,
      59,    64,   646,   647,    64,    58,   842,    60,   842,   802,
     853,    79,   846,   847,    76,   808,    42,   860,    58,    64,
      60,    64,    48,    49,    50,    51,    52,    53,    58,    72,
      60,   874,    75,    76,   877,    72,    73,    74,    75,    76,
      83,     5,     3,    86,    87,    53,    58,   890,    60,   842,
     694,    94,    48,   846,   847,    81,    60,    64,    19,    20,
      21,    76,   216,    24,    25,   219,    76,   221,     5,    28,
     224,   225,   226,   227,   228,    76,    37,     7,     8,    40,
      10,    76,    76,    76,    14,    17,    76,    76,    60,   733,
      64,    76,    28,    28,    76,    76,    26,   741,   897,     9,
      76,    64,    32,    64,    91,    35,    36,    37,    64,    39,
      42,    53,    64,    38,    75,    76,    48,    49,    50,    51,
      52,    53,    83,    76,    64,    86,    87,    76,    76,    76,
      76,    76,    76,    94,    59,   779,    61,    62,    76,    64,
       5,     5,   296,    68,    69,    70,    71,     5,    76,    81,
      80,    26,    27,     5,     5,    60,     5,   801,    88,    89,
      41,    36,    92,     8,     5,    10,    53,   321,    13,    76,
      95,    15,     5,     5,   328,   329,   330,    76,    28,    54,
      55,    26,   826,    38,    76,    62,    61,    32,    58,    64,
     834,    36,    26,    27,    39,   839,     5,    72,    73,    74,
      75,    76,    36,    30,    59,    80,    61,    62,    28,    64,
      53,     5,    60,    68,    69,    70,    71,   861,    17,    94,
      54,    55,    60,    76,     9,     5,     5,    61,     5,    60,
      64,    17,   386,   387,     5,    30,     5,    60,    72,    73,
      74,    75,    76,    42,    60,    60,    80,    76,     5,    48,
      49,    50,    51,    52,    53,    28,    42,    76,    76,    43,
      94,    53,    48,    49,    50,    51,    52,    53,     5,     5,
      54,    55,     5,    76,    30,    43,     5,     5,    62,    63,
      64,    59,    81,    67,    60,     5,    54,    55,    72,    73,
      74,    75,    76,    60,    62,    81,    64,    65,     5,    67,
     454,    43,     5,    87,    72,    73,    74,    75,    76,    60,
      94,    63,    54,    55,     5,    17,    60,     5,     5,    87,
      62,     5,    64,    65,    28,    67,    94,    76,     5,     5,
      72,    73,    74,    75,    76,     5,     5,     5,    43,     5,
       5,     5,    28,     5,     5,    87,   500,   501,     5,    54,
      55,     5,    94,    64,    43,     5,     5,    62,   242,    64,
      65,   515,    67,     5,    65,    54,    55,    72,    73,    74,
      75,    76,   240,    62,   246,    64,    65,   897,    67,   241,
      43,   690,    87,    72,    73,    74,    75,    76,   885,    94,
     370,    54,    55,   872,   883,   476,    43,   244,    87,    62,
     680,    64,    65,   547,    67,    94,   427,    54,    55,    72,
      73,    74,    75,    76,   628,    62,   498,    64,    65,   219,
      67,   714,    43,   718,    87,    72,    73,    74,    75,    76,
     387,    94,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      87,    62,    -1,    64,    65,    -1,    67,    94,    -1,    -1,
      43,    72,    73,    74,    75,    76,    -1,    -1,   612,   613,
      -1,    54,    55,    -1,    -1,    -1,    87,    -1,    -1,    62,
      -1,    64,    -1,    94,    67,    -1,    43,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    54,    55,    -1,
      -1,    -1,    43,    -1,    87,    62,    -1,    64,    -1,    -1,
      67,    94,    -1,    54,    55,    72,    73,    74,    75,    76,
      -1,    62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,
      87,    72,    73,    74,    75,    76,    -1,    94,    -1,   683,
     684,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      44,    45,    -1,    94,    48,    43,    -1,    -1,   702,    -1,
      54,    55,    56,    -1,    -1,    59,    -1,    55,    62,    -1,
      64,    -1,    66,    -1,    62,    -1,    64,   721,   722,    67,
      -1,    43,    -1,    77,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    87,
      62,    95,    64,    -1,    -1,    67,    94,    43,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    43,    -1,    87,    62,   771,    64,    -1,
      -1,    67,    94,    -1,    -1,    55,    72,    73,    74,    75,
      76,    -1,    62,    -1,    64,    -1,    -1,    67,    -1,    -1,
      -1,    87,    72,    73,    74,    75,    76,    43,    94,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    87,    -1,    55,
      -1,   815,    -1,   817,    94,    -1,    62,    -1,    64,    -1,
      -1,    67,    59,    43,    61,    62,    72,    73,    74,    75,
      76,    68,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    62,    -1,    64,    -1,    -1,    67,    94,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      33,    -1,    -1,    54,    55,    -1,    -1,    87,    -1,    42,
      -1,    -1,    -1,    64,    94,    48,    49,    50,    51,    52,
      53,    72,    73,    74,    75,    76,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    94,    42,    -1,    -1,    -1,    81,    65,
      48,    49,    50,    51,    52,    53,    -1,    54,    55,    -1,
      -1,    -1,    -1,    42,    -1,    81,    -1,    64,    66,    48,
      49,    50,    51,    52,    53,    72,    73,    74,    75,    76,
      -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,
      -1,    -1,    81,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      81,    66,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    -1,    81,   259,   260,   261,
     262,   263,   264,   265,   266,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81
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
     217,     5,   128,   138,   128,   138,   144,   145,   146,   180,
     170,     5,    60,   175,    60,   175,    60,   172,     6,    76,
     126,   173,     5,    76,   156,     5,     5,   170,    60,     5,
      59,     5,    18,    22,    17,   187,   141,   208,   209,   210,
     211,    41,   198,   217,   217,   138,    76,    30,     5,    30,
      41,    30,    65,    50,    65,   197,    65,   197,   217,    65,
     197,   128,    65,    53,   170,   170,     5,     5,   169,   169,
     182,   182,    15,   109,   170,   170,    60,   175,    60,   175,
      60,   177,   173,    28,    65,   141,   142,   142,   143,   143,
     143,   143,   124,   133,   169,   169,    76,    76,   169,   169,
      62,   135,   128,    60,    58,     5,    65,    58,    63,    30,
       5,    25,   150,    60,     5,   174,   173,   173,    60,   173,
      28,    59,    53,     5,   102,    60,   132,    76,     9,   174,
     217,   217,   185,    66,     5,    16,    41,   207,    58,    60,
     185,    66,    60,    17,    60,   191,   182,    65,    65,    65,
       5,     5,   128,     5,    60,   174,   174,   170,   170,   182,
      30,     5,    60,   173,   173,    60,   173,    76,    65,    30,
     170,   170,   169,   169,   170,   170,   171,    60,     5,   113,
     113,   114,    65,    76,   138,    28,   146,    76,   147,   174,
     163,    53,   132,     8,    76,    76,   217,     5,   132,     5,
       5,   157,    17,    17,   141,   209,   182,   182,    30,   211,
     185,   217,   217,   185,    76,    30,     5,   174,   132,   165,
     166,     5,    60,    59,   174,   132,   132,     5,    60,   170,
     170,     5,     5,    63,   124,    58,    65,    60,    63,   129,
      28,    60,   128,   161,   102,   217,     5,   174,   102,   185,
     185,    63,    17,   167,     5,   102,   102,   174,   132,     5,
     132,     5,    60,   136,   113,   113,    28,   130,   149,    76,
     102,     5,   174,   164,     5,   185,   102,   174,     5,     5,
     159,     5,   174,     5,   132,     5,    76,   128,   128,   151,
     152,   210,    28,     5,   162,   102,     5,   168,   102,   174,
       5,    72,    76,     5,    60,   149,   102,     5,   102,     5,
     160,   152,    64,   151,     5,     5,   102,   148,     5,   144,
      65
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
     172,   172,   173,   173,   173,   173,   173,   174,   174,   175,
     175,   175,   176,   176,   177,   177,   177,   177,   177,   177,
     178,   178,   179,   180,   180,   181,   183,   182,   184,   182,
     186,   185,   187,   187,   187,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     189,   189,   189,   189,   190,   190,   190,   191,   191,   192,
     192,   193,   193,   193,   194,   194,   194,   194,   195,   195,
     195,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   197,   197,   198,   198,   198,   198,   199,
     199,   199,   199,   199,   199,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   201,   201,   202,   203,   203,   204,
     204,   206,   205,   207,   207,   207,   208,   208,   209,   209,
     210,   210,   211,   211,   212,   213,   214,   214,   214,   215,
     216,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   218,   218,   218,   218,   218,   218,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   220,   220,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   222,   222,   223,   223
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
       2,     3,     1,     0,     4,     0,     0,     0,     7,     5,
       0,     3,     1,     6,     0,     2,     1,     4,     6,     6,
       7,     9,     0,     9,     0,    11,     0,     7,     0,    13,
       0,    15,     0,    12,     0,    14,     0,    11,     0,    13,
       0,    11,     0,    11,     0,    12,     0,    14,     0,     3,
       2,     0,     3,     1,     3,     5,     4,     3,     4,     5,
       6,     4,     1,     3,     3,     3,     1,     0,     3,     3,
       2,     0,     3,     1,     3,     4,     4,     4,     3,     5,
       3,     1,     1,     3,     1,     3,     0,     4,     0,     2,
       0,     2,     1,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       5,     5,     7,     5,     1,     3,     2,     6,     4,     1,
       2,     2,     4,     5,     1,     1,     3,     1,     1,     3,
       1,     4,     4,     3,     3,     4,     3,     6,     5,     6,
       5,     6,     5,     3,     1,     1,     3,     5,     6,     3,
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
#line 224 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2227 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2233 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2239 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2245 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2251 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2257 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2263 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2269 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2275 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2281 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2287 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2293 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2299 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2305 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2311 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2317 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2323 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2329 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2335 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2341 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2347 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2353 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2359 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2365 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2371 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2377 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2383 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2389 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2395 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2401 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2407 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2413 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2419 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2431 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2437 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2443 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2449 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2455 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2461 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2467 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2473 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2479 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2485 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2497 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2503 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2509 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2515 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2521 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2527 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2533 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2539 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2551 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2557 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2563 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2569 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2575 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2581 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2587 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2593 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2599 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2605 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2611 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2617 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2623 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2629 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2635 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2641 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2647 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2653 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2659 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2665 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2671 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2677 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2683 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2689 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2695 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2701 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2707 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2713 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2719 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2725 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2731 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2737 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2743 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2749 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2755 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2761 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2767 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2773 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2779 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2785 "pascal.tab.c"
    break;

  case 96: /* type: CLASSSY class_members ENDSY  */
#line 375 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2791 "pascal.tab.c"
    break;

  case 97: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 376 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2797 "pascal.tab.c"
    break;

  case 98: /* result_type: IDENT  */
#line 379 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2803 "pascal.tab.c"
    break;

  case 99: /* result_type: IDENT PERIOD IDENT  */
#line 380 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2809 "pascal.tab.c"
    break;

  case 100: /* result_type: spec_ref  */
#line 381 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2815 "pascal.tab.c"
    break;

  case 101: /* class_members: %empty  */
#line 384 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2821 "pascal.tab.c"
    break;

  case 102: /* class_members: class_members class_member  */
#line 385 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2827 "pascal.tab.c"
    break;

  case 103: /* class_member: IDENT  */
#line 388 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2833 "pascal.tab.c"
    break;

  case 104: /* class_member: IDENT SEMICOLON  */
#line 389 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2839 "pascal.tab.c"
    break;

  case 105: /* class_member: id_list COLON type SEMICOLON  */
#line 390 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2845 "pascal.tab.c"
    break;

  case 106: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 391 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2851 "pascal.tab.c"
    break;

  case 107: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 392 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2857 "pascal.tab.c"
    break;

  case 108: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2863 "pascal.tab.c"
    break;

  case 109: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 394 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2869 "pascal.tab.c"
    break;

  case 110: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2875 "pascal.tab.c"
    break;

  case 111: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 396 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2881 "pascal.tab.c"
    break;

  case 112: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 397 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2887 "pascal.tab.c"
    break;

  case 113: /* prop_index_opt: %empty  */
#line 400 "pascal.y"
    { (yyval.node) = NULL; }
#line 2893 "pascal.tab.c"
    break;

  case 114: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 401 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2899 "pascal.tab.c"
    break;

  case 115: /* prop_specs: %empty  */
#line 404 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2905 "pascal.tab.c"
    break;

  case 116: /* prop_specs: prop_specs IDENT IDENT  */
#line 405 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2911 "pascal.tab.c"
    break;

  case 117: /* prop_specs: prop_specs IDENT INTCONST  */
#line 406 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2917 "pascal.tab.c"
    break;

  case 118: /* packed_opt: PACKEDSY  */
#line 409 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2923 "pascal.tab.c"
    break;

  case 119: /* packed_opt: %empty  */
#line 410 "pascal.y"
     { (yyval.node) = NULL; }
#line 2929 "pascal.tab.c"
    break;

  case 120: /* simple_type: LPARENT enum_list RPARENT  */
#line 413 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2935 "pascal.tab.c"
    break;

  case 121: /* simple_type: IDENT  */
#line 414 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2941 "pascal.tab.c"
    break;

  case 122: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 415 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2947 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT PERIOD IDENT  */
#line 416 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2953 "pascal.tab.c"
    break;

  case 124: /* simple_type: spec_ref  */
#line 417 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2959 "pascal.tab.c"
    break;

  case 125: /* simple_type: lo_bound DOTDOT b_expr  */
#line 418 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2965 "pascal.tab.c"
    break;

  case 126: /* lo_bound: lo_term  */
#line 421 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2971 "pascal.tab.c"
    break;

  case 127: /* lo_bound: PLUS lo_term  */
#line 422 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2977 "pascal.tab.c"
    break;

  case 128: /* lo_bound: MINUS lo_term  */
#line 423 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2983 "pascal.tab.c"
    break;

  case 129: /* lo_bound: lo_bound PLUS b_term  */
#line 424 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2989 "pascal.tab.c"
    break;

  case 130: /* lo_bound: lo_bound MINUS b_term  */
#line 425 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2995 "pascal.tab.c"
    break;

  case 131: /* lo_term: scalar_constant  */
#line 428 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3001 "pascal.tab.c"
    break;

  case 132: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 429 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3007 "pascal.tab.c"
    break;

  case 133: /* lo_term: lo_term MUL b_factor  */
#line 430 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3013 "pascal.tab.c"
    break;

  case 134: /* lo_term: lo_term IDIV b_factor  */
#line 431 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3019 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term IMOD b_factor  */
#line 432 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3025 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term SHIFTOP b_factor  */
#line 433 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3031 "pascal.tab.c"
    break;

  case 137: /* b_expr: b_term  */
#line 436 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3037 "pascal.tab.c"
    break;

  case 138: /* b_expr: PLUS b_term  */
#line 437 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3043 "pascal.tab.c"
    break;

  case 139: /* b_expr: MINUS b_term  */
#line 438 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3049 "pascal.tab.c"
    break;

  case 140: /* b_expr: b_expr PLUS b_term  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3055 "pascal.tab.c"
    break;

  case 141: /* b_expr: b_expr MINUS b_term  */
#line 440 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3061 "pascal.tab.c"
    break;

  case 142: /* b_term: b_factor  */
#line 443 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3067 "pascal.tab.c"
    break;

  case 143: /* b_term: b_term MUL b_factor  */
#line 444 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3073 "pascal.tab.c"
    break;

  case 144: /* b_term: b_term IDIV b_factor  */
#line 445 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3079 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term IMOD b_factor  */
#line 446 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3085 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term SHIFTOP b_factor  */
#line 447 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3091 "pascal.tab.c"
    break;

  case 147: /* b_factor: scalar_constant  */
#line 450 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3097 "pascal.tab.c"
    break;

  case 148: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 451 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3103 "pascal.tab.c"
    break;

  case 149: /* b_factor: LPARENT b_expr RPARENT  */
#line 452 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3109 "pascal.tab.c"
    break;

  case 150: /* record_body: record_field_list record_case_opt  */
#line 455 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3115 "pascal.tab.c"
    break;

  case 151: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 458 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3121 "pascal.tab.c"
    break;

  case 152: /* record_field_list: record_field  */
#line 459 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 153: /* $@4: %empty  */
#line 462 "pascal.y"
                  { }
#line 3133 "pascal.tab.c"
    break;

  case 154: /* record_field: id_list COLON $@4 type  */
#line 462 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3139 "pascal.tab.c"
    break;

  case 155: /* record_field: %empty  */
#line 463 "pascal.y"
     { (yyval.node) = NULL; }
#line 3145 "pascal.tab.c"
    break;

  case 156: /* case_arm_mark: %empty  */
#line 466 "pascal.y"
    { }
#line 3151 "pascal.tab.c"
    break;

  case 157: /* case_open: %empty  */
#line 469 "pascal.y"
    { }
#line 3157 "pascal.tab.c"
    break;

  case 158: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 472 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3163 "pascal.tab.c"
    break;

  case 159: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 473 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3169 "pascal.tab.c"
    break;

  case 160: /* record_case_opt: %empty  */
#line 474 "pascal.y"
     { (yyval.node) = NULL; }
#line 3175 "pascal.tab.c"
    break;

  case 161: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 477 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3181 "pascal.tab.c"
    break;

  case 162: /* record_case_list: record_case_arm  */
#line 478 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3187 "pascal.tab.c"
    break;

  case 163: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 481 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3193 "pascal.tab.c"
    break;

  case 164: /* record_case_arm: %empty  */
#line 482 "pascal.y"
     { (yyval.node) = NULL; }
#line 3199 "pascal.tab.c"
    break;

  case 165: /* var_decl_list: var_decl_list var_decl  */
#line 485 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3205 "pascal.tab.c"
    break;

  case 166: /* var_decl_list: var_decl  */
#line 486 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3211 "pascal.tab.c"
    break;

  case 167: /* var_decl: id_list COLON type SEMICOLON  */
#line 489 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3217 "pascal.tab.c"
    break;

  case 168: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 490 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3223 "pascal.tab.c"
    break;

  case 169: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 491 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3229 "pascal.tab.c"
    break;

  case 170: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 494 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3235 "pascal.tab.c"
    break;

  case 171: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 495 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3241 "pascal.tab.c"
    break;

  case 172: /* $@5: %empty  */
#line 496 "pascal.y"
                                                                               { }
#line 3247 "pascal.tab.c"
    break;

  case 173: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 496 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3253 "pascal.tab.c"
    break;

  case 174: /* $@6: %empty  */
#line 497 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3259 "pascal.tab.c"
    break;

  case 175: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 497 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3265 "pascal.tab.c"
    break;

  case 176: /* $@7: %empty  */
#line 498 "pascal.y"
                                        { }
#line 3271 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 498 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3277 "pascal.tab.c"
    break;

  case 178: /* $@8: %empty  */
#line 499 "pascal.y"
                                                                                                           { }
#line 3283 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 499 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3289 "pascal.tab.c"
    break;

  case 180: /* $@9: %empty  */
#line 500 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3295 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 500 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3301 "pascal.tab.c"
    break;

  case 182: /* $@10: %empty  */
#line 501 "pascal.y"
                                                                                                 { }
#line 3307 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 501 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3313 "pascal.tab.c"
    break;

  case 184: /* $@11: %empty  */
#line 502 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3319 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 502 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3325 "pascal.tab.c"
    break;

  case 186: /* $@12: %empty  */
#line 503 "pascal.y"
                                                                                            { }
#line 3331 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 503 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3337 "pascal.tab.c"
    break;

  case 188: /* $@13: %empty  */
#line 504 "pascal.y"
                                                                                                             { }
#line 3343 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 504 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3351 "pascal.tab.c"
    break;

  case 190: /* $@14: %empty  */
#line 507 "pascal.y"
                                                                                              { }
#line 3357 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3365 "pascal.tab.c"
    break;

  case 192: /* $@15: %empty  */
#line 510 "pascal.y"
                                                                                             { }
#line 3371 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3379 "pascal.tab.c"
    break;

  case 194: /* $@16: %empty  */
#line 513 "pascal.y"
                                                                                                    { }
#line 3385 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3393 "pascal.tab.c"
    break;

  case 196: /* $@17: %empty  */
#line 516 "pascal.y"
                                                                                                                     { }
#line 3399 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3407 "pascal.tab.c"
    break;

  case 198: /* pv_mark: %empty  */
#line 521 "pascal.y"
    { }
#line 3413 "pascal.tab.c"
    break;

  case 199: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 524 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3419 "pascal.tab.c"
    break;

  case 200: /* parameter_list_opt: LPARENT RPARENT  */
#line 525 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3425 "pascal.tab.c"
    break;

  case 201: /* parameter_list_opt: %empty  */
#line 526 "pascal.y"
     { (yyval.node) = NULL; }
#line 3431 "pascal.tab.c"
    break;

  case 202: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 529 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3437 "pascal.tab.c"
    break;

  case 203: /* parameter_decl_list: parameter_decl  */
#line 530 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3443 "pascal.tab.c"
    break;

  case 204: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 533 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3449 "pascal.tab.c"
    break;

  case 205: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 534 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3455 "pascal.tab.c"
    break;

  case 206: /* parameter_decl: VARSY id_list COLON param_type  */
#line 535 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3461 "pascal.tab.c"
    break;

  case 207: /* parameter_decl: id_list COLON param_type  */
#line 536 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3467 "pascal.tab.c"
    break;

  case 208: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 537 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3473 "pascal.tab.c"
    break;

  case 209: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 538 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3479 "pascal.tab.c"
    break;

  case 210: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 539 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3485 "pascal.tab.c"
    break;

  case 211: /* parameter_decl: IDENT id_list COLON param_type  */
#line 540 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3491 "pascal.tab.c"
    break;

  case 212: /* param_type: IDENT  */
#line 543 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3497 "pascal.tab.c"
    break;

  case 213: /* param_type: IDENT PERIOD IDENT  */
#line 544 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3503 "pascal.tab.c"
    break;

  case 214: /* param_type: ARRAYSY OFSY IDENT  */
#line 545 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3509 "pascal.tab.c"
    break;

  case 215: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 546 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3515 "pascal.tab.c"
    break;

  case 216: /* param_type: spec_ref  */
#line 547 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3521 "pascal.tab.c"
    break;

  case 217: /* routine_directives: %empty  */
#line 550 "pascal.y"
    { (yyval.node) = NULL; }
#line 3527 "pascal.tab.c"
    break;

  case 218: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 551 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3540 "pascal.tab.c"
    break;

  case 219: /* pf_params: LPARENT pf_sections RPARENT  */
#line 561 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3546 "pascal.tab.c"
    break;

  case 220: /* pf_params: LPARENT RPARENT  */
#line 562 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3552 "pascal.tab.c"
    break;

  case 221: /* pf_params: %empty  */
#line 563 "pascal.y"
     { (yyval.node) = NULL; }
#line 3558 "pascal.tab.c"
    break;

  case 222: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 566 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3564 "pascal.tab.c"
    break;

  case 223: /* pf_sections: pf_section  */
#line 567 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3570 "pascal.tab.c"
    break;

  case 224: /* pf_section: id_list COLON param_type  */
#line 570 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3576 "pascal.tab.c"
    break;

  case 225: /* pf_section: VARSY id_list COLON param_type  */
#line 571 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3582 "pascal.tab.c"
    break;

  case 226: /* pf_section: CONSTSY id_list COLON param_type  */
#line 572 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3588 "pascal.tab.c"
    break;

  case 227: /* pf_section: IDENT id_list COLON param_type  */
#line 573 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3594 "pascal.tab.c"
    break;

  case 228: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 574 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3600 "pascal.tab.c"
    break;

  case 229: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 575 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3606 "pascal.tab.c"
    break;

  case 230: /* enum_list: enum_list COMMA enum_member  */
#line 578 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3612 "pascal.tab.c"
    break;

  case 231: /* enum_list: enum_member  */
#line 579 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3618 "pascal.tab.c"
    break;

  case 232: /* enum_member: IDENT  */
#line 582 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3624 "pascal.tab.c"
    break;

  case 233: /* id_list: id_list COMMA IDENT  */
#line 585 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3630 "pascal.tab.c"
    break;

  case 234: /* id_list: IDENT  */
#line 586 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3636 "pascal.tab.c"
    break;

  case 235: /* body: BEGINSY statement_list ENDSY  */
#line 589 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3642 "pascal.tab.c"
    break;

  case 236: /* $@18: %empty  */
#line 592 "pascal.y"
                             { pas_stmt_mark(); }
#line 3648 "pascal.tab.c"
    break;

  case 237: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 592 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3654 "pascal.tab.c"
    break;

  case 238: /* $@19: %empty  */
#line 593 "pascal.y"
     { pas_stmt_mark(); }
#line 3660 "pascal.tab.c"
    break;

  case 239: /* statement_list: $@19 statement  */
#line 593 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3666 "pascal.tab.c"
    break;

  case 240: /* $@20: %empty  */
#line 596 "pascal.y"
    { pas_stmt_mark(); }
#line 3672 "pascal.tab.c"
    break;

  case 241: /* body_stmt: $@20 statement  */
#line 596 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3678 "pascal.tab.c"
    break;

  case 242: /* statement: statement_no_label  */
#line 599 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3684 "pascal.tab.c"
    break;

  case 243: /* statement: INTCONST COLON statement_no_label  */
#line 600 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3690 "pascal.tab.c"
    break;

  case 244: /* statement: IDENT COLON statement_no_label  */
#line 601 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3696 "pascal.tab.c"
    break;

  case 245: /* statement_no_label: assignment  */
#line 604 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3702 "pascal.tab.c"
    break;

  case 246: /* statement_no_label: call  */
#line 605 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3708 "pascal.tab.c"
    break;

  case 247: /* statement_no_label: compound_statement  */
#line 606 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3714 "pascal.tab.c"
    break;

  case 248: /* statement_no_label: goto_statement  */
#line 607 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3720 "pascal.tab.c"
    break;

  case 249: /* statement_no_label: if_statement  */
#line 608 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3726 "pascal.tab.c"
    break;

  case 250: /* statement_no_label: case_statement  */
#line 609 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3732 "pascal.tab.c"
    break;

  case 251: /* statement_no_label: while_statement  */
#line 610 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3738 "pascal.tab.c"
    break;

  case 252: /* statement_no_label: repeat_statement  */
#line 611 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3744 "pascal.tab.c"
    break;

  case 253: /* statement_no_label: for_statement  */
#line 612 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3750 "pascal.tab.c"
    break;

  case 254: /* statement_no_label: with_statement  */
#line 613 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3756 "pascal.tab.c"
    break;

  case 255: /* statement_no_label: try_statement  */
#line 614 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3762 "pascal.tab.c"
    break;

  case 256: /* statement_no_label: raise_statement  */
#line 615 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3768 "pascal.tab.c"
    break;

  case 257: /* statement_no_label: inherited_call  */
#line 616 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3774 "pascal.tab.c"
    break;

  case 258: /* statement_no_label: INHERITEDSY  */
#line 617 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3780 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: %empty  */
#line 618 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3786 "pascal.tab.c"
    break;

  case 260: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 621 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3792 "pascal.tab.c"
    break;

  case 261: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 622 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3798 "pascal.tab.c"
    break;

  case 262: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 623 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3804 "pascal.tab.c"
    break;

  case 263: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 624 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3810 "pascal.tab.c"
    break;

  case 264: /* handler_list: handler  */
#line 627 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3816 "pascal.tab.c"
    break;

  case 265: /* handler_list: handler_list SEMICOLON handler  */
#line 628 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3822 "pascal.tab.c"
    break;

  case 266: /* handler_list: handler_list SEMICOLON  */
#line 629 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3828 "pascal.tab.c"
    break;

  case 267: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 632 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3834 "pascal.tab.c"
    break;

  case 268: /* handler: ONSY IDENT DOSY body_stmt  */
#line 633 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3840 "pascal.tab.c"
    break;

  case 269: /* raise_statement: RAISESY  */
#line 636 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3846 "pascal.tab.c"
    break;

  case 270: /* raise_statement: RAISESY expression  */
#line 637 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3852 "pascal.tab.c"
    break;

  case 271: /* inherited_call: INHERITEDSY IDENT  */
#line 640 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3858 "pascal.tab.c"
    break;

  case 272: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 641 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3864 "pascal.tab.c"
    break;

  case 273: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 642 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3870 "pascal.tab.c"
    break;

  case 274: /* call: IDENT  */
#line 645 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3876 "pascal.tab.c"
    break;

  case 275: /* call: spec_expr  */
#line 646 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3882 "pascal.tab.c"
    break;

  case 276: /* call: selector PERIOD IDENT  */
#line 647 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3888 "pascal.tab.c"
    break;

  case 277: /* call: call_with_args  */
#line 648 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3894 "pascal.tab.c"
    break;

  case 278: /* recv_primary: STRINGCONST  */
#line 651 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3900 "pascal.tab.c"
    break;

  case 279: /* recv_primary: LPARENT expression RPARENT  */
#line 652 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3906 "pascal.tab.c"
    break;

  case 280: /* recv_primary: spec_expr  */
#line 653 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3912 "pascal.tab.c"
    break;

  case 281: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 656 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3918 "pascal.tab.c"
    break;

  case 282: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 657 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3924 "pascal.tab.c"
    break;

  case 283: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 658 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3930 "pascal.tab.c"
    break;

  case 284: /* call_with_args: IDENT LPARENT RPARENT  */
#line 659 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3936 "pascal.tab.c"
    break;

  case 285: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 660 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3942 "pascal.tab.c"
    break;

  case 286: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 661 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3948 "pascal.tab.c"
    break;

  case 287: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 662 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3954 "pascal.tab.c"
    break;

  case 288: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 663 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3960 "pascal.tab.c"
    break;

  case 289: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 664 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3966 "pascal.tab.c"
    break;

  case 290: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 665 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3972 "pascal.tab.c"
    break;

  case 291: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 666 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3978 "pascal.tab.c"
    break;

  case 292: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 667 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3984 "pascal.tab.c"
    break;

  case 293: /* argument_list: argument_list COMMA argument  */
#line 670 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3990 "pascal.tab.c"
    break;

  case 294: /* argument_list: argument  */
#line 671 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3996 "pascal.tab.c"
    break;

  case 295: /* argument: expression  */
#line 674 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4002 "pascal.tab.c"
    break;

  case 296: /* argument: expression COLON expression  */
#line 675 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4008 "pascal.tab.c"
    break;

  case 297: /* argument: expression COLON expression COLON expression  */
#line 676 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4014 "pascal.tab.c"
    break;

  case 298: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 677 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4020 "pascal.tab.c"
    break;

  case 299: /* assignment: selector BECOMES expression  */
#line 680 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4026 "pascal.tab.c"
    break;

  case 300: /* assignment: selector ADDBECOMES expression  */
#line 681 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4032 "pascal.tab.c"
    break;

  case 301: /* assignment: selector SUBBECOMES expression  */
#line 682 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4038 "pascal.tab.c"
    break;

  case 302: /* assignment: selector MULBECOMES expression  */
#line 683 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4044 "pascal.tab.c"
    break;

  case 303: /* assignment: selector DIVBECOMES expression  */
#line 684 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4050 "pascal.tab.c"
    break;

  case 304: /* assignment: call_with_args BECOMES expression  */
#line 685 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4056 "pascal.tab.c"
    break;

  case 305: /* selector: selector LBRACK expression_list RBRACK  */
#line 688 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4062 "pascal.tab.c"
    break;

  case 306: /* selector: selector PERIOD IDENT  */
#line 689 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4068 "pascal.tab.c"
    break;

  case 307: /* selector: selector ARROW  */
#line 690 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4074 "pascal.tab.c"
    break;

  case 308: /* selector: call_with_args PERIOD IDENT  */
#line 691 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4080 "pascal.tab.c"
    break;

  case 309: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 692 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4086 "pascal.tab.c"
    break;

  case 310: /* selector: call_with_args ARROW  */
#line 693 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4092 "pascal.tab.c"
    break;

  case 311: /* selector: recv_primary PERIOD IDENT  */
#line 694 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4098 "pascal.tab.c"
    break;

  case 312: /* selector: recv_primary ARROW  */
#line 695 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4104 "pascal.tab.c"
    break;

  case 313: /* selector: IDENT  */
#line 696 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4110 "pascal.tab.c"
    break;

  case 314: /* expression_list: expression_list COMMA expression  */
#line 699 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4116 "pascal.tab.c"
    break;

  case 315: /* expression_list: expression  */
#line 700 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4122 "pascal.tab.c"
    break;

  case 316: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 703 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4128 "pascal.tab.c"
    break;

  case 317: /* goto_statement: GOTOSY INTCONST  */
#line 706 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4134 "pascal.tab.c"
    break;

  case 318: /* goto_statement: GOTOSY IDENT  */
#line 707 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4140 "pascal.tab.c"
    break;

  case 319: /* if_statement: IFSY expression THENSY body_stmt  */
#line 710 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4146 "pascal.tab.c"
    break;

  case 320: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 711 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4152 "pascal.tab.c"
    break;

  case 321: /* $@21: %empty  */
#line 714 "pascal.y"
                           { }
#line 4158 "pascal.tab.c"
    break;

  case 322: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 714 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4164 "pascal.tab.c"
    break;

  case 323: /* case_tail: %empty  */
#line 717 "pascal.y"
    { (yyval.node) = NULL; }
#line 4170 "pascal.tab.c"
    break;

  case 324: /* case_tail: ELSESY statement_list  */
#line 718 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4176 "pascal.tab.c"
    break;

  case 325: /* case_tail: OTHERWISESY statement_list  */
#line 719 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4182 "pascal.tab.c"
    break;

  case 326: /* case_list: case_list SEMICOLON case_elem  */
#line 722 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4188 "pascal.tab.c"
    break;

  case 327: /* case_list: case_elem  */
#line 723 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4194 "pascal.tab.c"
    break;

  case 328: /* case_elem: constant_list COLON body_stmt  */
#line 726 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4200 "pascal.tab.c"
    break;

  case 329: /* case_elem: %empty  */
#line 727 "pascal.y"
     { (yyval.node) = NULL; }
#line 4206 "pascal.tab.c"
    break;

  case 330: /* constant_list: constant_list COMMA case_label  */
#line 730 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4212 "pascal.tab.c"
    break;

  case 331: /* constant_list: case_label  */
#line 731 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4218 "pascal.tab.c"
    break;

  case 332: /* case_label: b_expr  */
#line 734 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4224 "pascal.tab.c"
    break;

  case 333: /* case_label: b_expr DOTDOT b_expr  */
#line 735 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4230 "pascal.tab.c"
    break;

  case 334: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 738 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4236 "pascal.tab.c"
    break;

  case 335: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 741 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4242 "pascal.tab.c"
    break;

  case 336: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 744 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4248 "pascal.tab.c"
    break;

  case 337: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 745 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4254 "pascal.tab.c"
    break;

  case 338: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 746 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4260 "pascal.tab.c"
    break;

  case 339: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 749 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4266 "pascal.tab.c"
    break;

  case 340: /* with_open: with_open COMMA selector  */
#line 752 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4272 "pascal.tab.c"
    break;

  case 341: /* with_open: selector  */
#line 753 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4278 "pascal.tab.c"
    break;

  case 342: /* expression: simple_expression  */
#line 760 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4284 "pascal.tab.c"
    break;

  case 343: /* expression: expression INOP simple_expression  */
#line 761 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4290 "pascal.tab.c"
    break;

  case 344: /* expression: expression LTOP simple_expression  */
#line 762 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4296 "pascal.tab.c"
    break;

  case 345: /* expression: expression LEOP simple_expression  */
#line 763 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4302 "pascal.tab.c"
    break;

  case 346: /* expression: expression GTOP simple_expression  */
#line 764 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4308 "pascal.tab.c"
    break;

  case 347: /* expression: expression GEOP simple_expression  */
#line 765 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4314 "pascal.tab.c"
    break;

  case 348: /* expression: expression NEOP simple_expression  */
#line 766 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4320 "pascal.tab.c"
    break;

  case 349: /* expression: expression EQOP simple_expression  */
#line 767 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4326 "pascal.tab.c"
    break;

  case 350: /* expression: expression ISOP simple_expression  */
#line 768 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4332 "pascal.tab.c"
    break;

  case 351: /* simple_expression: term  */
#line 771 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4338 "pascal.tab.c"
    break;

  case 352: /* simple_expression: PLUS term  */
#line 772 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4344 "pascal.tab.c"
    break;

  case 353: /* simple_expression: MINUS term  */
#line 773 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4350 "pascal.tab.c"
    break;

  case 354: /* simple_expression: simple_expression PLUS term  */
#line 774 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4356 "pascal.tab.c"
    break;

  case 355: /* simple_expression: simple_expression MINUS term  */
#line 775 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4362 "pascal.tab.c"
    break;

  case 356: /* simple_expression: simple_expression OROP term  */
#line 776 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4368 "pascal.tab.c"
    break;

  case 357: /* simple_expression: simple_expression XOROP term  */
#line 777 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4374 "pascal.tab.c"
    break;

  case 358: /* term: power  */
#line 780 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4380 "pascal.tab.c"
    break;

  case 359: /* term: term MUL power  */
#line 781 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4386 "pascal.tab.c"
    break;

  case 360: /* term: term RDIV power  */
#line 782 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4392 "pascal.tab.c"
    break;

  case 361: /* term: term IDIV power  */
#line 783 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4398 "pascal.tab.c"
    break;

  case 362: /* term: term IMOD power  */
#line 784 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4404 "pascal.tab.c"
    break;

  case 363: /* term: term ANDOP power  */
#line 785 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4410 "pascal.tab.c"
    break;

  case 364: /* term: term MUL MINUS power  */
#line 786 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4416 "pascal.tab.c"
    break;

  case 365: /* term: term RDIV MINUS power  */
#line 787 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4422 "pascal.tab.c"
    break;

  case 366: /* term: term IDIV MINUS power  */
#line 788 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4428 "pascal.tab.c"
    break;

  case 367: /* term: term IMOD MINUS power  */
#line 789 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4434 "pascal.tab.c"
    break;

  case 368: /* term: term ANDOP MINUS power  */
#line 790 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4440 "pascal.tab.c"
    break;

  case 369: /* term: term SHIFTOP power  */
#line 791 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4446 "pascal.tab.c"
    break;

  case 370: /* term: term ASOP power  */
#line 792 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4452 "pascal.tab.c"
    break;

  case 371: /* power: factor  */
#line 795 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4458 "pascal.tab.c"
    break;

  case 372: /* power: power POWOP factor  */
#line 796 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4464 "pascal.tab.c"
    break;

  case 373: /* factor: selector  */
#line 799 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4470 "pascal.tab.c"
    break;

  case 374: /* factor: call_with_args  */
#line 800 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4476 "pascal.tab.c"
    break;

  case 375: /* factor: INTCONST  */
#line 801 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4482 "pascal.tab.c"
    break;

  case 376: /* factor: REALCONST  */
#line 802 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4488 "pascal.tab.c"
    break;

  case 377: /* factor: STRINGCONST  */
#line 803 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4494 "pascal.tab.c"
    break;

  case 378: /* factor: CHARCODE  */
#line 804 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4500 "pascal.tab.c"
    break;

  case 379: /* factor: LPARENT expression RPARENT  */
#line 805 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4506 "pascal.tab.c"
    break;

  case 380: /* factor: inherited_call  */
#line 806 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4512 "pascal.tab.c"
    break;

  case 381: /* factor: NOTSY factor  */
#line 807 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4518 "pascal.tab.c"
    break;

  case 382: /* factor: ATSIGN factor  */
#line 808 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4524 "pascal.tab.c"
    break;

  case 383: /* factor: LBRACK RBRACK  */
#line 809 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4530 "pascal.tab.c"
    break;

  case 384: /* factor: LBRACK set_member_list RBRACK  */
#line 810 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4536 "pascal.tab.c"
    break;

  case 385: /* set_member_list: set_member  */
#line 813 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4542 "pascal.tab.c"
    break;

  case 386: /* set_member_list: set_member_list COMMA set_member  */
#line 814 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4548 "pascal.tab.c"
    break;

  case 387: /* set_member: expression  */
#line 817 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4554 "pascal.tab.c"
    break;

  case 388: /* set_member: expression DOTDOT expression  */
#line 818 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4560 "pascal.tab.c"
    break;


#line 4564 "pascal.tab.c"

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

#line 824 "pascal.y"

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
