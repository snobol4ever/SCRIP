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
  YYSYMBOL_OBJECTSY = 83,                  /* OBJECTSY  */
  YYSYMBOL_TRYSY = 84,                     /* TRYSY  */
  YYSYMBOL_EXCEPTSY = 85,                  /* EXCEPTSY  */
  YYSYMBOL_FINALLYSY = 86,                 /* FINALLYSY  */
  YYSYMBOL_RAISESY = 87,                   /* RAISESY  */
  YYSYMBOL_INHERITEDSY = 88,               /* INHERITEDSY  */
  YYSYMBOL_CONSTRUCTORSY = 89,             /* CONSTRUCTORSY  */
  YYSYMBOL_DESTRUCTORSY = 90,              /* DESTRUCTORSY  */
  YYSYMBOL_PROPERTYSY = 91,                /* PROPERTYSY  */
  YYSYMBOL_ONSY = 92,                      /* ONSY  */
  YYSYMBOL_GENERICSY = 93,                 /* GENERICSY  */
  YYSYMBOL_GENERICTSY = 94,                /* GENERICTSY  */
  YYSYMBOL_SPECIALIZESY = 95,              /* SPECIALIZESY  */
  YYSYMBOL_GENLT = 96,                     /* GENLT  */
  YYSYMBOL_GENGT = 97,                     /* GENGT  */
  YYSYMBOL_YYACCEPT = 98,                  /* $accept  */
  YYSYMBOL_compilation_unit = 99,          /* compilation_unit  */
  YYSYMBOL_program = 100,                  /* program  */
  YYSYMBOL_program_name = 101,             /* program_name  */
  YYSYMBOL_file_id_list_opt = 102,         /* file_id_list_opt  */
  YYSYMBOL_block = 103,                    /* block  */
  YYSYMBOL_decl_part_list = 104,           /* decl_part_list  */
  YYSYMBOL_decl_part = 105,                /* decl_part  */
  YYSYMBOL_unit = 106,                     /* unit  */
  YYSYMBOL_intf_part_list = 107,           /* intf_part_list  */
  YYSYMBOL_intf_part = 108,                /* intf_part  */
  YYSYMBOL_init_opt = 109,                 /* init_opt  */
  YYSYMBOL_fini_opt = 110,                 /* fini_opt  */
  YYSYMBOL_label_list = 111,               /* label_list  */
  YYSYMBOL_const_decl_list = 112,          /* const_decl_list  */
  YYSYMBOL_const_decl = 113,               /* const_decl  */
  YYSYMBOL_typed_value = 114,              /* typed_value  */
  YYSYMBOL_typed_value_list = 115,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 116,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 117,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 118,           /* type_decl_list  */
  YYSYMBOL_type_decl = 119,                /* type_decl  */
  YYSYMBOL_uses_list = 120,                /* uses_list  */
  YYSYMBOL_uses_name = 121,                /* uses_name  */
  YYSYMBOL_spec_gen = 122,                 /* spec_gen  */
  YYSYMBOL_spec_expr = 123,                /* spec_expr  */
  YYSYMBOL_class_parents = 124,            /* class_parents  */
  YYSYMBOL_parent_item = 125,              /* parent_item  */
  YYSYMBOL_spec_kw = 126,                  /* spec_kw  */
  YYSYMBOL_spec_ref = 127,                 /* spec_ref  */
  YYSYMBOL_type_arg_list = 128,            /* type_arg_list  */
  YYSYMBOL_type = 129,                     /* type  */
  YYSYMBOL_130_1 = 130,                    /* $@1  */
  YYSYMBOL_131_2 = 131,                    /* $@2  */
  YYSYMBOL_132_3 = 132,                    /* $@3  */
  YYSYMBOL_result_type = 133,              /* result_type  */
  YYSYMBOL_guid_opt = 134,                 /* guid_opt  */
  YYSYMBOL_class_members = 135,            /* class_members  */
  YYSYMBOL_class_member = 136,             /* class_member  */
  YYSYMBOL_prop_index_opt = 137,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 138,               /* prop_specs  */
  YYSYMBOL_packed_opt = 139,               /* packed_opt  */
  YYSYMBOL_simple_type = 140,              /* simple_type  */
  YYSYMBOL_lo_bound = 141,                 /* lo_bound  */
  YYSYMBOL_lo_term = 142,                  /* lo_term  */
  YYSYMBOL_b_expr = 143,                   /* b_expr  */
  YYSYMBOL_b_term = 144,                   /* b_term  */
  YYSYMBOL_b_factor = 145,                 /* b_factor  */
  YYSYMBOL_record_body = 146,              /* record_body  */
  YYSYMBOL_record_field_list = 147,        /* record_field_list  */
  YYSYMBOL_record_field = 148,             /* record_field  */
  YYSYMBOL_149_4 = 149,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 150,            /* case_arm_mark  */
  YYSYMBOL_case_open = 151,                /* case_open  */
  YYSYMBOL_record_case_opt = 152,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 153,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 154,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 155,            /* var_decl_list  */
  YYSYMBOL_var_decl = 156,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 157,           /* procedure_decl  */
  YYSYMBOL_158_5 = 158,                    /* $@5  */
  YYSYMBOL_159_6 = 159,                    /* $@6  */
  YYSYMBOL_160_7 = 160,                    /* $@7  */
  YYSYMBOL_161_8 = 161,                    /* $@8  */
  YYSYMBOL_162_9 = 162,                    /* $@9  */
  YYSYMBOL_163_10 = 163,                   /* $@10  */
  YYSYMBOL_164_11 = 164,                   /* $@11  */
  YYSYMBOL_165_12 = 165,                   /* $@12  */
  YYSYMBOL_166_13 = 166,                   /* $@13  */
  YYSYMBOL_167_14 = 167,                   /* $@14  */
  YYSYMBOL_168_15 = 168,                   /* $@15  */
  YYSYMBOL_169_16 = 169,                   /* $@16  */
  YYSYMBOL_170_17 = 170,                   /* $@17  */
  YYSYMBOL_171_18 = 171,                   /* $@18  */
  YYSYMBOL_172_19 = 172,                   /* $@19  */
  YYSYMBOL_173_20 = 173,                   /* $@20  */
  YYSYMBOL_pv_mark = 174,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 175,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 176,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 177,           /* parameter_decl  */
  YYSYMBOL_param_type = 178,               /* param_type  */
  YYSYMBOL_routine_directives = 179,       /* routine_directives  */
  YYSYMBOL_pf_params = 180,                /* pf_params  */
  YYSYMBOL_pf_sections = 181,              /* pf_sections  */
  YYSYMBOL_pf_section = 182,               /* pf_section  */
  YYSYMBOL_enum_list = 183,                /* enum_list  */
  YYSYMBOL_enum_member = 184,              /* enum_member  */
  YYSYMBOL_id_list = 185,                  /* id_list  */
  YYSYMBOL_body = 186,                     /* body  */
  YYSYMBOL_statement_list = 187,           /* statement_list  */
  YYSYMBOL_188_21 = 188,                   /* $@21  */
  YYSYMBOL_189_22 = 189,                   /* $@22  */
  YYSYMBOL_body_stmt = 190,                /* body_stmt  */
  YYSYMBOL_191_23 = 191,                   /* $@23  */
  YYSYMBOL_statement = 192,                /* statement  */
  YYSYMBOL_statement_no_label = 193,       /* statement_no_label  */
  YYSYMBOL_try_statement = 194,            /* try_statement  */
  YYSYMBOL_handler_list = 195,             /* handler_list  */
  YYSYMBOL_handler = 196,                  /* handler  */
  YYSYMBOL_raise_statement = 197,          /* raise_statement  */
  YYSYMBOL_inherited_call = 198,           /* inherited_call  */
  YYSYMBOL_call = 199,                     /* call  */
  YYSYMBOL_recv_primary = 200,             /* recv_primary  */
  YYSYMBOL_call_with_args = 201,           /* call_with_args  */
  YYSYMBOL_argument_list = 202,            /* argument_list  */
  YYSYMBOL_argument = 203,                 /* argument  */
  YYSYMBOL_assignment = 204,               /* assignment  */
  YYSYMBOL_selector = 205,                 /* selector  */
  YYSYMBOL_expression_list = 206,          /* expression_list  */
  YYSYMBOL_compound_statement = 207,       /* compound_statement  */
  YYSYMBOL_goto_statement = 208,           /* goto_statement  */
  YYSYMBOL_if_statement = 209,             /* if_statement  */
  YYSYMBOL_case_statement = 210,           /* case_statement  */
  YYSYMBOL_211_24 = 211,                   /* $@24  */
  YYSYMBOL_case_tail = 212,                /* case_tail  */
  YYSYMBOL_case_list = 213,                /* case_list  */
  YYSYMBOL_case_elem = 214,                /* case_elem  */
  YYSYMBOL_constant_list = 215,            /* constant_list  */
  YYSYMBOL_case_label = 216,               /* case_label  */
  YYSYMBOL_while_statement = 217,          /* while_statement  */
  YYSYMBOL_repeat_statement = 218,         /* repeat_statement  */
  YYSYMBOL_for_statement = 219,            /* for_statement  */
  YYSYMBOL_with_statement = 220,           /* with_statement  */
  YYSYMBOL_with_open = 221,                /* with_open  */
  YYSYMBOL_expression = 222,               /* expression  */
  YYSYMBOL_simple_expression = 223,        /* simple_expression  */
  YYSYMBOL_term = 224,                     /* term  */
  YYSYMBOL_power = 225,                    /* power  */
  YYSYMBOL_factor = 226,                   /* factor  */
  YYSYMBOL_set_member_list = 227,          /* set_member_list  */
  YYSYMBOL_set_member = 228                /* set_member  */
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
#define YYLAST   1869

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  410
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  986

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   352


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
      95,    96,    97
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
     322,   323,   324,   327,   328,   331,   332,   335,   338,   339,
     342,   343,   346,   347,   350,   353,   354,   355,   358,   359,
     362,   363,   364,   365,   366,   366,   366,   367,   368,   369,
     370,   371,   372,   373,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   385,   386,   387,   390,   391,   394,
     395,   398,   399,   400,   401,   402,   403,   404,   405,   406,
     407,   410,   411,   414,   415,   416,   419,   420,   423,   424,
     425,   426,   427,   428,   431,   432,   433,   434,   435,   438,
     439,   440,   441,   442,   443,   446,   447,   448,   449,   450,
     453,   454,   455,   456,   457,   460,   461,   462,   465,   468,
     469,   472,   472,   473,   474,   475,   476,   477,   478,   479,
     480,   481,   484,   487,   490,   491,   492,   495,   496,   499,
     500,   503,   504,   507,   508,   509,   512,   513,   514,   514,
     515,   515,   516,   516,   517,   517,   518,   518,   519,   519,
     520,   520,   521,   521,   522,   522,   525,   525,   528,   528,
     531,   531,   534,   534,   538,   538,   542,   542,   546,   546,
     551,   554,   555,   556,   559,   560,   563,   564,   565,   566,
     567,   568,   569,   570,   573,   574,   575,   576,   577,   580,
     581,   591,   592,   593,   596,   597,   600,   601,   602,   603,
     604,   605,   608,   609,   612,   615,   616,   619,   622,   622,
     623,   623,   626,   626,   629,   630,   631,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   651,   652,   653,   654,   657,   658,   659,   662,
     663,   666,   667,   670,   671,   672,   675,   676,   677,   678,
     681,   682,   683,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   700,   701,   704,   705,   706,
     707,   710,   711,   712,   713,   714,   715,   718,   719,   720,
     721,   722,   723,   724,   725,   726,   729,   730,   733,   736,
     737,   740,   741,   744,   744,   747,   748,   749,   752,   753,
     756,   757,   760,   761,   764,   765,   768,   771,   774,   775,
     776,   779,   782,   783,   790,   791,   792,   793,   794,   795,
     796,   797,   798,   801,   802,   803,   804,   805,   806,   807,
     810,   811,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   825,   826,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   843,   844,   847,
     848
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
  "ASOP", "OBJECTSY", "TRYSY", "EXCEPTSY", "FINALLYSY", "RAISESY",
  "INHERITEDSY", "CONSTRUCTORSY", "DESTRUCTORSY", "PROPERTYSY", "ONSY",
  "GENERICSY", "GENERICTSY", "SPECIALIZESY", "GENLT", "GENGT", "$accept",
  "compilation_unit", "program", "program_name", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "unit", "intf_part_list",
  "intf_part", "init_opt", "fini_opt", "label_list", "const_decl_list",
  "const_decl", "typed_value", "typed_value_list", "field_value_list",
  "scalar_constant", "type_decl_list", "type_decl", "uses_list",
  "uses_name", "spec_gen", "spec_expr", "class_parents", "parent_item",
  "spec_kw", "spec_ref", "type_arg_list", "type", "$@1", "$@2", "$@3",
  "result_type", "guid_opt", "class_members", "class_member",
  "prop_index_opt", "prop_specs", "packed_opt", "simple_type", "lo_bound",
  "lo_term", "b_expr", "b_term", "b_factor", "record_body",
  "record_field_list", "record_field", "$@4", "case_arm_mark", "case_open",
  "record_case_opt", "record_case_list", "record_case_arm",
  "var_decl_list", "var_decl", "procedure_decl", "$@5", "$@6", "$@7",
  "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15", "$@16",
  "$@17", "$@18", "$@19", "$@20", "pv_mark", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "param_type",
  "routine_directives", "pf_params", "pf_sections", "pf_section",
  "enum_list", "enum_member", "id_list", "body", "statement_list", "$@21",
  "$@22", "body_stmt", "$@23", "statement", "statement_no_label",
  "try_statement", "handler_list", "handler", "raise_statement",
  "inherited_call", "call", "recv_primary", "call_with_args",
  "argument_list", "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@24", "case_tail", "case_list", "case_elem", "constant_list",
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

#define YYPACT_NINF (-553)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-336)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     386,    62,    89,   198,  -553,   168,   236,  -553,  -553,   409,
    -553,    68,  -553,  -553,   350,   169,    89,   219,   243,   243,
     265,  -553,   145,   176,   278,   296,   250,  -553,  -553,  -553,
     305,   243,   262,   384,   333,  -553,  -553,    44,   371,   169,
    -553,    94,   366,   339,  -553,   243,  -553,   675,   101,   346,
      57,  1017,   545,   362,   145,  -553,   377,   407,   416,   439,
     448,   488,   513,  -553,   379,  -553,  -553,  -553,  -553,   465,
    1429,  1084,  -553,  -553,    89,   243,   535,   342,  -553,   548,
    1084,  -553,   243,   556,    46,  -553,  -553,   478,   575,  -553,
    1429,   313,  1429,  -553,  1429,  1429,   561,  -553,  1135,  -553,
    1429,   592,   595,  -553,   172,  -553,  -553,  -553,  -553,  -553,
    -553,  -553,   638,   598,  -553,  1635,  -553,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,   243,  1164,   555,  -553,   619,   625,
     628,   614,   631,   593,   670,  -553,   679,   772,  -553,  -553,
    1612,  1612,  1612,  1202,  1429,  1612,  -553,  -553,  -553,   678,
     118,   592,   680,  -553,   639,   649,   351,   415,   609,   677,
    -553,   689,   683,  -553,   683,   757,   757,   212,   667,  -553,
    -553,  -553,  -553,  1514,    28,   223,  -553,  -553,  -553,  -553,
     709,   328,  -553,   325,   261,   366,   289,  -553,    64,   773,
    -553,    41,   318,  -553,  -553,   721,  1017,  -553,  -553,   524,
     286,   365,   639,   649,   159,  1201,    59,  1660,  1672,  1031,
    1031,  1237,   695,    35,  1788,   724,   743,  1279,   716,  -553,
    1429,   720,  -553,  1429,  1304,  1429,   723,  -553,  1429,  1429,
    1429,  1429,  1429,   413,   191,   125,   802,   243,   741,   745,
      25,  -553,  -553,   243,   243,  -553,   169,    89,  -553,   750,
     243,   759,   145,  -553,  -553,   609,   609,  -553,  1694,   564,
    -553,  1706,  -553,   760,  -553,  1429,  1429,  1429,  1429,  1429,
    1429,  1429,  1429,  1612,  1612,  1612,  1612,   540,  1520,  1545,
    1570,  1595,  1612,  1612,  1612,   774,   247,  -553,    96,   830,
     799,   800,   261,   261,   127,  -553,  -553,   421,  -553,   695,
     787,  1778,  1778,   792,   247,   119,   798,   810,   818,   603,
     822,   834,   841,   845,   299,  -553,   697,  1454,   192,   850,
     891,  -553,   496,   496,  1778,   496,   496,   496,   496,  -553,
     342,   243,   848,   243,   852,  -553,   243,    60,  -553,   714,
     912,  -553,  1454,   856,  -553,   342,  -553,   279,  -553,  1429,
    1429,  1429,  -553,  -553,   313,  -553,  -553,  -553,  -553,   905,
    -553,  -553,  -553,   461,  -553,   668,  1728,     1,  -553,   835,
    -553,  1329,   695,  -553,   475,   862,  1788,   867,   580,  1788,
    -553,   477,  1788,  1225,   622,  1788,  1788,  1788,  1788,   882,
    -553,  -553,   247,  -553,   453,  -553,  -553,  -553,  -553,   342,
     342,   454,   455,   169,   208,   869,  -553,   243,  -553,   145,
    1429,  1429,  -553,   718,   880,   415,   415,   415,   415,   415,
     415,   415,   415,   609,   609,   609,   609,  1612,   677,  1612,
     677,  1612,   677,  1612,   677,  1612,   677,   677,   677,  -553,
     883,   127,   521,  -553,  -553,   189,   243,   874,   243,   877,
    -553,   243,    79,  -553,   742,   878,   279,   667,  -553,   466,
    -553,   496,   496,  1778,   881,  -553,   426,   287,  -553,   402,
    -553,   527,   879,  -553,   884,    76,   301,   887,   894,   895,
    -553,   754,  -553,  -553,   892,  -553,   124,  -553,  -553,  -553,
     171,   910,  -553,  -553,  1479,   951,  1788,  1084,   695,  1084,
     695,   171,   287,   287,   468,  -553,  -553,  -553,  -553,   342,
     982,   755,   683,   781,   683,   782,   107,  -553,    29,   983,
     913,   985,   990,   342,   936,   998,   110,  -553,  1000,   930,
     664,  1788,  -553,  1017,  -553,   649,  1778,   965,  1429,  -553,
    1429,  1429,   695,  -553,   931,    61,   259,  -553,   330,  -553,
     543,   470,  -553,  1354,  1379,  1429,  -553,  -553,  1404,  -553,
    1084,   565,   956,   342,   342,   342,   342,  1005,  1008,  -553,
    -553,  -553,  -553,  -553,  1007,   342,   342,  1788,  -553,   677,
     677,   677,   677,   677,  -553,   247,   953,  -553,   785,   683,
     811,   683,   824,   115,  -553,    29,  -553,   995,  -553,  -553,
     287,   287,   411,  1778,   496,   496,  -553,   496,   496,   496,
     496,  -553,  -553,  -553,  -553,  -553,   949,   952,  -553,  -553,
     969,  1084,   342,   342,   171,  -553,  -553,   342,   342,  -553,
     230,  1084,   178,   974,    81,  1706,  -553,  -553,   661,  -553,
    -553,  1003,  1030,  -553,    29,  -553,    29,   979,    29,  -553,
    1012,   234,  -553,   991,  -553,  1038,  -553,  -553,  -553,   986,
     279,  -553,   971,  1036,  1429,  1429,  -553,  -553,   329,   280,
    -553,   851,  -553,  -553,  -553,  1741,  1753,  -553,   177,  -553,
     835,  -553,  -553,  -553,  -553,  -553,  -553,   568,  -553,   582,
    1788,  -553,   591,  1043,  1044,  1084,  1048,   999,  1001,    52,
    -553,  -553,   342,   342,  1053,  1053,  -553,  1033,  1055,  1004,
    -553,  -553,    29,  -553,    29,  1006,    29,  -553,  -553,   989,
    -553,   420,   287,   287,  -553,  -553,  -553,  -553,   419,   342,
     342,  -553,  -553,   342,   342,   107,  1010,  1062,  -553,  1013,
    1042,   342,   342,  -553,  -553,  -553,  1009,  -553,  1454,  1454,
     347,  -553,   695,  1046,  -553,  -553,   913,  1023,  -553,   279,
    -553,    42,  1011,  1429,  -553,  1072,   279,  1075,  -553,  1081,
     913,   839,  1132,  -553,  1778,  1778,  -553,  -553,  1058,  1778,
    -553,  -553,  1429,  1429,  -553,  1021,  -553,   340,  -553,  -553,
    -553,  -553,  -553,  1093,  -553,   279,   279,  -553,   279,   913,
     913,  1094,  1040,  1053,  1054,  -553,   279,   529,  -553,  -553,
     279,  -553,  -553,  -553,  -553,  1097,  1056,   342,   342,  1098,
    1109,   130,   247,  -553,   279,  -553,  1057,  1061,  1778,  1099,
    -553,  -553,   612,  -553,  1065,  1059,  -553,   913,  -553,  1429,
    -553,  -553,  -553,  -553,  1788,  -553,  1123,  -553,  -553,  -553,
    -553,  -553,   468,  -553,  1053,  1053,  -553,  -553,  -553,  1766,
    1788,  -553,  1112,  -553,  -553,   913,  1125,  1126,   913,  1129,
    -553,  -553,  -553,   279,  -553,   913,  1131,  -553,  -553,  -553,
     279,  1136,  1077,  -553,  -553,  -553,  -553,  -553,   279,   279,
    1137,  -553,   855,  -553,  1454,  -553,  1454,  1115,  -553,  -553,
    1139,  1788,  -553,   913,  1141,  -553,  -553,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,  1142,  1145,   913,  1146,  -553,  1147,
    -553,   279,    43,  -553,  -553,  1778,  1089,  1778,  -553,  -553,
    1084,  1084,  1149,  -553,   913,  -553,  -553,  -553,  1150,   913,
     913,  1156,   913,  -553,  -553,  -553,  -553,   913,  -553,  1157,
    -553,   505,  -553,  -553,  1137,  -553,  -553,  -553,  -553,  1158,
    -553,  -553,  -553,  -553,  -553,  1165,   913,  -553,  -553,  -553,
     171,  1166,  -553,  1167,  1170,  1172,  -553,  -553,  1104,  -553,
    -553,  -553,  -553,  1173,  -553,  -553
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   260,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   220,   256,    16,   182,     0,     0,   220,
       0,   281,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   127,    38,    18,     0,     0,     0,   223,   181,     0,
     127,    19,     0,     0,   223,   258,   257,     0,     0,   260,
       0,     0,     0,   260,     0,     0,     0,   300,   296,   260,
     291,   280,     0,    69,   297,    68,   261,   264,   277,   278,
     279,   268,     0,   299,   267,     0,   269,   270,   271,   272,
     273,   274,   275,   276,     0,   127,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   397,   400,   398,   399,
     335,     0,   302,   402,   396,   395,     0,   364,   373,   380,
     393,   107,   243,   126,   243,     0,     0,     0,     0,    51,
      54,    52,    53,   129,   109,   171,   139,    76,    75,   132,
       0,     0,    80,     0,   134,    64,     0,   220,     0,     0,
     255,     0,     0,   220,   192,     0,   281,   339,   340,     0,
       0,     0,     0,   363,     0,     0,     0,     0,     0,   281,
     281,     0,     0,     0,   292,   293,     0,     0,     0,   334,
       0,     0,   332,     0,     0,     0,     0,   329,     0,     0,
       0,     0,     0,     0,   107,   109,     0,     0,     0,     0,
       0,   220,   220,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   403,   374,   375,   405,   409,     0,
     407,     0,   404,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    88,
       0,    50,   135,   136,    81,    83,   254,     0,   253,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,   176,   160,     0,     0,     0,    96,
       0,    93,     0,     0,     0,     0,     0,     0,     0,   220,
     223,     0,     0,     0,     0,   222,   256,     0,   225,     0,
     239,   183,     0,     0,   220,   223,    12,     0,   259,     0,
       0,     0,   262,   262,     0,   343,   338,   262,   301,   296,
     265,   266,   306,     0,   316,   395,   317,     0,    78,   260,
     260,     0,     0,   305,     0,   333,   326,   330,     0,   337,
     308,     0,   321,   298,     0,   322,   323,   324,   325,     0,
      61,    60,     0,    57,     0,   220,   220,   220,   220,   223,
     223,     0,     0,    24,     0,    31,   220,    26,   220,    25,
       0,     0,   406,   401,   328,   365,   366,   367,   368,   369,
     370,   371,   372,   378,   376,   377,   379,     0,   383,     0,
     384,     0,   385,     0,   381,     0,   382,   391,   392,   394,
       0,    72,     0,    70,    73,     0,     0,     0,     0,     0,
     242,   256,     0,   245,     0,     0,     0,     0,   128,     0,
     131,     0,     0,     0,    50,   155,     0,   145,   150,     0,
      82,     0,     0,   100,     0,   111,     0,     0,     0,     0,
     110,     0,   220,   220,     0,   168,     0,   220,   220,    98,
     171,     0,   158,   161,     0,     0,    42,   127,     0,   127,
       0,   171,   137,   138,   133,   142,   143,   141,   144,   223,
       0,     0,   243,     0,   243,     0,     0,   221,     0,     0,
     188,     0,     0,   223,     0,     0,   104,   106,     0,     0,
       0,   357,   356,   281,   361,   362,   351,   341,     0,   303,
       0,     0,     0,    67,     0,     0,     0,   286,     0,   294,
       0,     0,   304,     0,     0,     0,   331,   307,     0,   327,
     127,     0,     0,   223,   223,   223,   223,     0,     0,   220,
     220,    23,   260,   260,    33,   223,   223,   410,   408,   388,
     389,   390,   386,   387,   108,     0,   107,   102,     0,   243,
       0,   243,     0,     0,   241,     0,    89,    90,   252,    77,
     146,   147,     0,     0,     0,     0,   130,     0,     0,     0,
       0,   140,   109,   220,   220,   112,     0,     0,   220,   220,
     121,   127,   223,   223,   171,   220,   220,   223,   223,   159,
       0,   127,   335,     0,     0,    42,    41,    87,     0,    97,
      95,     0,     0,   239,     0,   226,     0,     0,     0,   224,
       0,   234,   238,   229,   186,     0,    12,   184,   185,     0,
       0,   193,     0,   239,     0,     0,   262,   263,   354,   345,
     349,     0,   353,   262,   315,   337,   318,    79,     0,   282,
     288,   283,   260,   285,   295,    74,   314,     0,   312,     0,
     336,   310,     0,     0,   109,   127,     0,     0,     0,     0,
     239,   239,   223,   223,    29,    30,   260,     0,     0,     0,
      71,   109,     0,   250,     0,     0,     0,   244,   246,     0,
     157,     0,   148,   149,   152,   153,   151,   154,     0,   223,
     223,   220,   220,   223,   223,     0,     0,     0,   163,     0,
       0,   223,   223,   165,   166,   173,     0,   162,     0,     0,
       0,    44,     0,     0,    94,   239,   202,   230,   228,     0,
     233,     0,     0,     0,   240,     0,     0,     0,   105,     0,
     190,     0,     0,   360,     0,   351,   260,   260,     0,     0,
     262,   342,     0,     0,   262,     0,   287,     0,   313,   311,
     309,    59,    62,     0,   239,     0,     0,   239,     0,   206,
     208,     0,     0,    32,     0,   239,     0,     0,   248,   247,
       0,   249,    91,   156,   101,     0,     0,   223,   223,     0,
       0,     0,     0,   113,     0,    99,     0,   170,   180,     0,
      48,    46,     0,    45,     0,     0,    84,   198,    12,     0,
     227,   237,   236,   235,   231,   189,     0,   239,   187,    12,
     262,   262,   355,   348,   347,   346,   344,   352,   350,     0,
     319,   290,     0,   284,    58,   210,     0,     0,   216,     0,
      12,    12,   239,     0,    20,    27,     0,   103,   251,   114,
       0,     0,     0,   116,   117,   122,   123,   164,     0,     0,
     175,   178,     0,   173,     0,    43,     0,     0,    85,    12,
       0,   232,   239,   204,     0,   359,   358,   320,   262,    12,
     239,   239,    12,   239,     0,     0,   194,     0,   239,     0,
     118,     0,     0,   169,   167,   180,     0,   180,    47,    49,
     127,   127,     0,   203,   200,    12,   191,   289,     0,   218,
     214,     0,   212,   207,   209,    12,   239,    28,   115,     0,
     120,     0,   177,   172,   174,    92,    86,   199,    12,     0,
     211,    12,    12,   217,    12,     0,   196,   119,   125,   124,
     171,     0,   205,     0,     0,     0,   195,    12,     0,   201,
     219,   215,   213,     0,   179,   197
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -553,  -553,  -553,  -553,  -553,   -65,   938,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,   941,   -25,  -338,  -553,  -553,   -58,
     937,   -51,   954,    40,   -70,   -46,  -266,  -552,   -64,   202,
    -256,   -78,  -553,  -553,  -553,   141,   602,  -251,  -553,  -553,
    -553,  -553,  -206,  -553,   -83,  -283,  -294,  -257,  -493,  -553,
    -298,  -553,  -553,   309,  -553,   271,   283,   961,   -22,  -553,
    -553,  -553,  -553,  -553,  -553,  -553,  -553,  -553,  -553,  -553,
    -553,  -553,  -553,  -553,  -553,  -553,   596,   200,   481,   696,
    -535,   222,  -139,  -553,   624,  -553,   763,    13,  -553,   -72,
    -553,  -553,  -333,  -553,  -170,   512,  -553,  -553,   544,  -553,
     -36,  -553,  -553,   -39,  -187,   685,  -553,    -6,  1018,  -553,
    -553,  -553,  -553,  -553,  -553,  -553,   457,  -527,   451,  -553,
    -553,  -553,  -553,  -553,   444,  1049,   -88,  -203,  -106,  -553,
     823
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   574,   707,    37,    39,    40,   495,   832,   634,   465,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   898,   931,   501,   528,   287,   305,   480,   736,
     922,   181,   182,   183,   184,   668,   467,   468,   313,   314,
     315,   631,   970,   828,   492,   890,   891,    45,    46,    28,
     656,   849,   346,   945,   977,   899,   958,   838,   935,   870,
     871,   909,   964,   962,   912,   961,    77,   189,   337,   338,
     653,   520,   289,   452,   453,   297,   298,   316,    29,    50,
     196,    51,   532,   533,   106,   107,   108,   546,   547,   109,
     153,   111,   112,   154,   363,   364,   114,   155,   384,   116,
     117,   118,   119,   536,   778,   669,   670,   892,   672,   120,
     121,   122,   123,   204,   366,   157,   158,   159,   160,   259,
     260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,   127,   521,   104,   368,   178,   641,   671,
     177,   485,   113,   176,    72,   110,   178,   200,   466,   469,
     534,   206,   176,    78,   537,   290,   348,   213,   502,   503,
     374,    47,    48,   710,   254,   650,   445,   381,   471,   262,
      85,   504,    11,   459,    64,   115,   341,   236,   950,    68,
     841,   194,   202,   255,   256,   177,   303,   797,    47,   542,
     718,   178,    85,   397,    85,   516,    85,   176,   505,   506,
     507,   508,   331,    33,   428,   430,   432,   434,   436,   437,
     438,   615,   292,   293,   593,   203,   750,    86,   186,   356,
     332,   679,   304,   368,   342,   192,   333,   177,   543,    73,
     334,   398,    69,   178,   446,   651,    81,   176,   176,   757,
     188,   758,   798,   760,   185,   331,   551,   343,   842,   951,
     369,   370,   447,   446,   102,   517,   561,    34,   448,   335,
     391,   740,   449,   332,  -256,   516,  -256,   233,     8,   333,
     336,   447,   177,   334,   594,   472,   751,   448,   178,   473,
     104,   449,    74,   303,   176,   474,   633,   113,   299,    79,
     110,   450,   625,   104,   104,    10,   368,   600,   601,   662,
     113,   113,   451,   110,   110,   299,   353,   808,   439,   809,
     602,   811,   211,   336,   550,   423,   424,   425,   426,   392,
     115,   451,   629,   885,   784,   475,   390,   306,    12,   476,
     626,   339,    56,   115,   115,   365,   212,   307,   477,   478,
     479,   365,    57,   571,   212,   472,   177,   354,   365,   587,
     497,    52,   178,   212,   579,   474,   580,    13,   581,   177,
     582,  -302,   583,  -302,   177,   178,   217,   785,   748,    53,
     178,   176,   211,    14,    15,    38,    16,   309,   671,   306,
     394,   310,    58,   285,   498,   286,   401,   402,   745,   307,
     311,   312,    17,    47,   680,   475,    74,    65,    18,   476,
     886,    19,    20,    21,   212,    22,    61,   177,   477,   478,
     479,   525,   299,   178,   195,   775,    62,   308,   294,   681,
     746,    85,   638,   762,   640,    43,   776,   545,   548,   309,
     682,   454,   177,   310,   490,   325,   326,   102,   178,   351,
     722,   723,   311,   312,   176,   202,    23,   327,   481,    44,
     721,   777,   177,   441,   491,    24,    25,   616,   178,    26,
     212,   607,   608,   773,   318,    85,   677,   617,   328,   329,
     781,    49,   102,   609,   511,    85,   513,    79,   535,   515,
     724,   725,   726,   727,    59,   526,   264,   319,   127,   320,
     683,   728,   321,   667,   610,   365,   687,   689,   344,   295,
     863,   692,    60,   645,   102,   647,    79,    95,    72,   322,
     323,    63,   352,   604,   605,    78,   177,    75,    97,   150,
       1,   324,   178,   265,    82,   774,    66,     2,    76,   266,
     267,   268,   269,   270,   271,    83,   188,   265,   102,    67,
     830,   831,   833,   266,   267,   268,   269,   270,   271,   637,
      47,   639,    35,   834,    70,    34,    36,   177,   177,   177,
     177,    71,   272,   178,   178,   178,   178,    79,   126,   176,
     176,   176,   176,   728,   135,   472,   272,   858,   177,   814,
     713,   861,   715,   128,   178,   474,   604,   605,   481,   588,
     807,   590,   273,   389,   592,   604,   605,   611,    30,   274,
     275,    79,   177,    31,   604,   605,   720,   978,   178,   457,
     604,   605,   693,   129,   176,   813,   458,   104,   444,   606,
     177,   852,   130,   276,   113,   475,   178,   110,   131,   476,
     704,   705,   176,   562,   569,   570,   444,   132,   477,   478,
     479,    79,    79,    79,   156,   177,   599,   905,   906,   538,
     685,   178,   604,   605,   542,   177,   539,   115,   542,   339,
     510,   178,   365,   538,   201,   538,   205,   138,   207,   208,
     552,   139,   557,   737,   214,   524,   835,   365,   365,   527,
     197,   177,   365,   747,   198,   472,   928,   178,   929,   877,
     463,   177,   349,   176,   133,   474,   350,   178,   169,   170,
     171,   172,   464,   176,   177,   937,   177,   968,   177,   585,
     178,   969,   178,   140,   178,   585,   586,   258,   261,   134,
     177,   765,   612,   124,   444,   427,   178,   597,   125,   567,
     568,   538,   143,   237,   144,   475,   454,   145,   684,   476,
     787,   187,   146,   147,   148,   149,   150,   793,   477,   478,
     479,   209,   411,   585,   190,   177,   538,   412,   151,   306,
     694,   178,   193,   788,   803,   102,   220,   176,   555,   307,
     538,   243,   177,   556,   177,    84,   177,   789,   178,   538,
     178,   199,   178,   277,   278,   279,   790,   221,   527,   222,
     223,  -256,   224,  -256,   376,   280,   281,   379,   215,   382,
     894,   216,   379,   385,   386,   387,   388,   895,   238,   309,
     555,   666,   177,   310,   239,   559,   282,   240,   178,   177,
     241,   283,   311,   312,   176,   178,   177,   218,   221,   219,
     222,   223,   178,   224,   854,   855,   265,   242,   263,   642,
     227,   228,   266,   267,   268,   269,   270,   271,   244,   752,
     652,   360,   361,   659,   753,   177,   177,   263,   177,   227,
     540,   178,   178,    79,   178,    80,   177,  -300,   245,  -300,
     177,   481,   178,   296,   217,   272,   178,   288,   339,   165,
     166,   285,   177,   286,   177,    79,   284,   493,   178,   168,
     178,   496,   317,   696,   697,   698,   699,   169,   170,   171,
     172,   173,    79,   900,   518,   708,   709,  -301,   340,  -301,
     246,   347,   247,   330,   904,   248,   496,   444,   371,   345,
     102,   372,   375,   529,   530,   531,   377,   652,   249,   383,
      79,   767,   595,   177,   250,   914,   915,   393,   251,   178,
     177,   252,    79,    79,   621,   644,   178,   395,   177,   177,
     481,   396,   738,   739,   178,   178,   406,   743,   744,   169,
     170,   171,   172,   291,   932,   408,   414,   399,   400,    79,
      79,   646,   648,    79,   938,   712,   652,   941,   652,   440,
     652,   177,   955,   956,   577,   258,   850,   178,   455,   456,
     177,   177,   527,   460,   302,   756,   178,   178,   470,    79,
     959,   714,   176,   176,   482,   489,    14,    15,   499,    16,
     965,   265,    79,   572,   716,   770,   483,   266,   267,   268,
     269,   270,   271,   971,   484,    17,   973,   974,   486,   975,
     840,    18,   801,   802,    19,    20,   573,   846,    22,   779,
     487,   780,   983,   779,   652,   926,   652,   488,   652,   500,
     272,   519,   799,   800,   512,   509,   553,   544,   514,   815,
     816,   554,   522,   819,   820,   560,   866,   867,   635,   869,
     523,   826,   827,  -335,   558,   603,   584,   876,   664,    23,
     589,   878,   665,   591,   596,   613,   636,   624,    24,    25,
     614,   527,    26,   618,  -335,   887,  -335,  -335,   527,   211,
     619,   620,   265,  -335,  -335,  -335,  -335,   837,   266,   267,
     268,   269,   270,   271,   675,   676,   630,   643,   654,   655,
     657,   563,   564,   565,   566,   658,   660,   527,   527,   690,
     527,   212,   575,   661,   576,   663,   673,   678,   527,   695,
     700,   272,   527,   701,   917,   285,   865,   881,   882,   868,
      87,   919,   706,   719,   444,   731,   527,   875,   732,   923,
     924,   735,   749,   754,    87,   755,    88,    89,    90,   759,
     761,    91,    92,   764,   763,   769,   766,   768,   791,   792,
      88,    89,    90,   794,    93,    91,    92,    94,    85,   795,
     805,   796,   949,   804,   806,   812,   810,   823,    93,   903,
     822,    94,   825,   824,   836,   527,   839,   845,   622,   623,
     847,    95,   527,   627,   628,   829,   848,   843,   856,    96,
     527,   527,    97,    98,   916,    95,   161,   862,   864,   872,
     873,    99,   879,   883,   100,   101,    97,   359,   771,   772,
     162,   163,   102,   874,   884,    99,   880,   888,   100,   101,
     164,   889,   897,   527,   934,   896,   102,   893,   902,   908,
     910,   911,   939,   940,   913,   942,   918,   921,   165,   166,
     947,   920,   925,   930,   933,   167,   936,   943,   168,   851,
     944,   946,   948,   953,   957,   960,   169,   170,   171,   172,
     173,   963,   967,   972,   174,   702,   703,   175,   966,   984,
     976,   979,   980,  -335,   265,   981,   234,   982,   985,   102,
     266,   267,   268,   269,   270,   271,   405,   403,   711,   409,
     162,   163,   496,   496,  -335,   210,  -335,  -335,   954,   211,
     164,   404,   927,  -335,  -335,  -335,  -335,   844,   952,   729,
     730,   407,   649,   272,   733,   734,   821,   717,   165,   166,
     598,   741,   742,   674,   786,   167,   859,   860,   168,   355,
     857,   212,   853,     0,   578,     0,   169,   170,   171,   172,
     173,   378,     0,   265,   235,   140,     0,   175,     0,   266,
     267,   268,   269,   270,   271,     0,   141,   142,     0,   102,
       0,     0,     0,  -328,   143,   257,   144,     0,     0,   145,
       0,     0,     0,     0,   146,   147,   148,   149,   150,     0,
     140,     0,   272,   901,  -328,     0,  -328,  -328,     0,   558,
     151,   141,   142,  -328,  -328,  -328,  -328,   102,     0,   143,
       0,   144,   362,     0,   145,     0,     0,     0,     0,   146,
     147,   148,   149,   150,   415,   416,   417,   418,   419,   420,
     421,   422,   140,     0,     0,   151,     0,   817,   818,     0,
       0,     0,   102,   141,   142,     0,     0,     0,   496,     0,
     496,   143,     0,   144,   373,     0,   145,   140,     0,     0,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,     0,     0,   143,   151,   144,   380,
       0,   145,   140,     0,   102,     0,   146,   147,   148,   149,
     150,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,   143,   151,   144,   549,     0,   145,   140,     0,   102,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,     0,     0,   143,   151,   144,   686,
       0,   145,   140,     0,   102,     0,   146,   147,   148,   149,
     150,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,   143,   151,   144,   688,     0,   145,   140,     0,   102,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,     0,     0,   143,   151,   144,   691,
       0,   145,   140,     0,   102,     0,   146,   147,   148,   149,
     150,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,   143,   151,   144,     0,     0,   145,   140,     0,   102,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,     0,     0,   143,   151,   494,     0,
       0,   145,   140,     0,   102,     0,   146,   147,   148,   149,
     150,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,   143,   151,   494,     0,     0,   145,     0,     0,   102,
       0,   146,   147,   148,   149,   632,     0,     0,   -50,   -50,
       0,     0,   299,   140,     0,     0,     0,   151,   -50,   -50,
     -50,     0,     0,   300,   102,   429,   301,     0,   302,     0,
     -50,     0,   143,     0,   144,     0,     0,   145,   140,     0,
       0,   -50,   146,   147,   148,   149,   150,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     212,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,   143,   151,   144,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,     0,
     435,     0,     0,     0,     0,   140,     0,   143,   151,   144,
       0,     0,   145,     0,     0,   102,     0,   146,   147,   148,
     149,   150,     0,   225,   143,     0,   144,     0,     0,   145,
       0,     0,     0,   151,   146,   147,   148,   149,   150,     0,
     102,     0,     0,   357,   226,     0,   227,   228,     0,     0,
     151,     0,   265,   229,   230,   231,   232,   102,   266,   267,
     268,   269,   270,   271,   265,     0,     0,     0,     0,     0,
     266,   267,   268,   269,   270,   271,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   265,   358,     0,     0,
       0,   272,   266,   267,   268,   269,   270,   271,   265,     0,
       0,     0,     0,   272,   266,   267,   268,   269,   270,   271,
     410,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     265,   413,     0,     0,     0,   272,   266,   267,   268,   269,
     270,   271,     0,   265,     0,     0,     0,   272,   541,   266,
     267,   268,   269,   270,   271,   265,     0,     0,     0,     0,
       0,   266,   267,   268,   269,   270,   271,   782,   265,   272,
       0,     0,     0,   783,   266,   267,   268,   269,   270,   271,
       0,     0,   272,     0,     0,     0,     0,     0,     0,   907,
     265,     0,   461,   462,   272,     0,   266,   267,   268,   269,
     270,   271,   463,     0,     0,     0,     0,   272,     0,     0,
     169,   170,   171,   172,   464,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   272
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,    54,   342,    51,   212,    71,   501,   536,
      80,   309,    51,    71,    39,    51,    80,    89,   301,   302,
     353,    93,    80,    45,   357,   164,   196,    99,   322,   323,
     217,    18,    19,   585,   140,     6,   287,   224,   304,   145,
       5,   324,     2,   299,    31,    51,     5,   125,     5,     5,
       8,     5,    91,   141,   142,   125,    28,     5,    45,    58,
     595,   125,     5,    38,     5,     5,     5,   125,   325,   326,
     327,   328,     8,     5,   277,   278,   279,   280,   281,   282,
     283,     5,   165,   166,     5,    91,     5,    30,    75,    30,
      26,    30,    64,   299,    53,    82,    32,   167,    97,     5,
      36,    76,    58,   167,     8,    76,     5,   165,   166,   644,
      64,   646,    60,   648,    74,     8,   372,    76,    76,    76,
      85,    86,    26,     8,    95,    65,   392,    59,    32,    65,
       5,   624,    36,    26,    58,     5,    60,   124,    76,    32,
      76,    26,   212,    36,    65,    26,    65,    32,   212,    30,
     196,    36,    58,    28,   212,    36,   494,   196,    48,    58,
     196,    65,    38,   209,   210,    76,   372,   461,   462,    59,
     209,   210,    76,   209,   210,    48,    17,   712,   284,   714,
     463,   716,    64,    76,   371,   273,   274,   275,   276,    64,
     196,    76,   490,    63,    17,    76,     5,    26,     0,    80,
      76,   188,    26,   209,   210,   211,    96,    36,    89,    90,
      91,   217,    36,     5,    96,    26,   286,    58,   224,    30,
      28,    76,   286,    96,   427,    36,   429,    59,   431,   299,
     433,    59,   435,    61,   304,   299,    64,    60,    60,    94,
     304,   299,    64,     7,     8,    76,    10,    76,   775,    26,
     237,    80,    76,    62,    62,    64,   243,   244,    28,    36,
      89,    90,    26,   250,     5,    76,    58,     5,    32,    80,
     822,    35,    36,    37,    96,    39,    26,   347,    89,    90,
      91,   346,    48,   347,    84,     5,    36,    64,    76,    30,
      60,     5,   498,    59,   500,    76,    16,   369,   370,    76,
      41,   288,   372,    80,     5,    44,    45,    95,   372,    23,
     604,   605,    89,    90,   372,   354,    80,    56,   305,    76,
     603,    41,   392,    76,    25,    89,    90,    26,   392,    93,
      96,    44,    45,   666,     6,     5,   542,    36,    77,    50,
     673,    76,    95,    56,   331,     5,   333,    58,   354,   336,
     607,   608,   609,   610,    76,    76,     5,    29,   409,    31,
      30,   612,    34,   533,    77,   371,   553,   554,    50,   167,
      30,   558,    76,   512,    95,   514,    58,    64,   403,    54,
      55,    76,    17,    54,    55,   407,   456,    48,    75,    76,
       4,    66,   456,    42,    48,    66,    12,    11,    59,    48,
      49,    50,    51,    52,    53,    59,    64,    42,    95,    76,
     748,   749,    65,    48,    49,    50,    51,    52,    53,   497,
     407,   499,    72,    76,    53,    59,    76,   497,   498,   499,
     500,    60,    81,   497,   498,   499,   500,    58,    76,   497,
     498,   499,   500,   694,    65,    26,    81,   780,   518,    30,
     589,   784,   591,    76,   518,    36,    54,    55,   445,   446,
     711,   448,    47,    50,   451,    54,    55,    65,    59,    54,
      55,    58,   542,    64,    54,    55,    65,   970,   542,    58,
      54,    55,   560,    76,   542,    65,    65,   533,   286,    63,
     560,   774,    76,    78,   533,    76,   560,   533,    59,    80,
     572,   573,   560,    50,    50,    50,   304,    59,    89,    90,
      91,    58,    58,    58,    70,   585,    50,   850,   851,    58,
      50,   585,    54,    55,    58,   595,    65,   533,    58,   516,
     330,   595,   538,    58,    90,    58,    92,    72,    94,    95,
      65,    76,    65,   621,   100,   345,   752,   553,   554,   347,
      72,   621,   558,   631,    76,    26,   894,   621,   896,    30,
      64,   631,    38,   621,    76,    36,    42,   631,    72,    73,
      74,    75,    76,   631,   644,   908,   646,    72,   648,    58,
     644,    76,   646,    43,   648,    58,    65,   143,   144,    76,
     660,   656,    65,    48,   392,    55,   660,   456,    53,   399,
     400,    58,    62,    48,    64,    76,   593,    67,    65,    80,
     682,    76,    72,    73,    74,    75,    76,   695,    89,    90,
      91,    60,    58,    58,    76,   695,    58,    63,    88,    26,
      65,   695,    76,    65,   706,    95,    38,   695,    58,    36,
      58,    48,   712,    63,   714,    49,   716,    65,   712,    58,
     714,    76,   716,    44,    45,    46,    65,    59,   456,    61,
      62,    58,    64,    60,   220,    56,    57,   223,    76,   225,
      58,    76,   228,   229,   230,   231,   232,    65,    59,    76,
      58,    17,   752,    80,    59,    63,    77,    59,   752,   759,
      76,    82,    89,    90,   752,   759,   766,    59,    59,    61,
      61,    62,   766,    64,   776,   777,    42,    76,    59,   509,
      61,    62,    48,    49,    50,    51,    52,    53,    48,    58,
     518,   209,   210,   523,    63,   795,   796,    59,   798,    61,
      62,   795,   796,    58,   798,    60,   806,    59,    59,    61,
     810,   728,   806,    76,    64,    81,   810,    64,   735,    54,
      55,    62,   822,    64,   824,    58,    79,    60,   822,    64,
     824,   317,    53,   563,   564,   565,   566,    72,    73,    74,
      75,    76,    58,   838,    60,   575,   576,    59,     5,    61,
       8,    60,    10,   187,   849,    13,   342,   585,    64,   193,
      95,    48,    76,   349,   350,   351,    76,   595,    26,    76,
      58,   660,    60,   873,    32,   870,   871,     5,    36,   873,
     880,    39,    58,    58,    60,    60,   880,    76,   888,   889,
     807,    76,   622,   623,   888,   889,    76,   627,   628,    72,
      73,    74,    75,    76,   899,    76,    76,   241,   242,    58,
      58,    60,    60,    58,   909,    60,   644,   912,   646,    75,
     648,   921,   930,   931,   410,   411,    17,   921,    28,    60,
     930,   931,   660,    76,    64,   643,   930,   931,    76,    58,
     935,    60,   930,   931,    76,    30,     7,     8,    28,    10,
     945,    42,    58,    14,    60,   663,    76,    48,    49,    50,
      51,    52,    53,   958,    76,    26,   961,   962,    76,   964,
     759,    32,   702,   703,    35,    36,    37,   766,    39,    58,
      76,    60,   977,    58,   712,    60,   714,    76,   716,    28,
      81,     9,   700,   701,    76,   329,    64,    92,    76,   729,
     730,    64,    76,   733,   734,    53,   795,   796,   494,   798,
     344,   741,   742,    38,    64,    64,    63,   806,    18,    80,
      76,   810,    22,    76,    76,    76,     5,    65,    89,    90,
      76,   759,    93,    76,    59,   824,    61,    62,   766,    64,
      76,    76,    42,    68,    69,    70,    71,   755,    48,    49,
      50,    51,    52,    53,   540,   541,    76,     5,     5,    76,
       5,   395,   396,   397,   398,     5,    60,   795,   796,   555,
     798,    96,   406,     5,   408,     5,    41,    76,   806,    53,
       5,    81,   810,     5,   873,    62,   794,   817,   818,   797,
       3,   880,    15,    28,   822,    76,   824,   805,    76,   888,
     889,    62,    58,    30,     3,     5,    19,    20,    21,    60,
      28,    24,    25,     5,    53,     9,    60,    76,     5,     5,
      19,    20,    21,     5,    37,    24,    25,    40,     5,    60,
       5,    60,   921,    30,    60,    76,    60,     5,    37,   847,
      60,    40,    30,    60,    28,   873,    53,     5,   482,   483,
       5,    64,   880,   487,   488,    76,     5,    76,    30,    72,
     888,   889,    75,    76,   872,    64,    12,    76,     5,     5,
      60,    84,     5,     5,    87,    88,    75,    76,   664,   665,
      26,    27,    95,    59,     5,    84,    60,    60,    87,    88,
      36,    60,    63,   921,   902,    60,    95,    28,     5,    17,
       5,     5,   910,   911,     5,   913,     5,    60,    54,    55,
     918,     5,     5,    28,     5,    61,     5,     5,    64,    17,
       5,     5,     5,    64,     5,     5,    72,    73,    74,    75,
      76,     5,     5,     5,    80,   569,   570,    83,   946,    65,
       5,     5,     5,    38,    42,     5,    12,     5,     5,    95,
      48,    49,    50,    51,    52,    53,   248,   246,   586,   252,
      26,    27,   748,   749,    59,    60,    61,    62,   927,    64,
      36,   247,   893,    68,    69,    70,    71,   763,   925,   613,
     614,   250,   516,    81,   618,   619,   735,   593,    54,    55,
     457,   625,   626,   538,   680,    61,   782,   783,    64,    28,
     779,    96,   775,    -1,   411,    -1,    72,    73,    74,    75,
      76,   223,    -1,    42,    80,    43,    -1,    83,    -1,    48,
      49,    50,    51,    52,    53,    -1,    54,    55,    -1,    95,
      -1,    -1,    -1,    38,    62,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      43,    -1,    81,   839,    59,    -1,    61,    62,    -1,    64,
      88,    54,    55,    68,    69,    70,    71,    95,    -1,    62,
      -1,    64,    65,    -1,    67,    -1,    -1,    -1,    -1,    72,
      73,    74,    75,    76,   265,   266,   267,   268,   269,   270,
     271,   272,    43,    -1,    -1,    88,    -1,   731,   732,    -1,
      -1,    -1,    95,    54,    55,    -1,    -1,    -1,   894,    -1,
     896,    62,    -1,    64,    65,    -1,    67,    43,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    65,    -1,    67,    43,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    65,    -1,    67,    43,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    -1,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    -1,    -1,    67,    -1,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    44,    45,
      -1,    -1,    48,    43,    -1,    -1,    -1,    88,    54,    55,
      56,    -1,    -1,    59,    95,    55,    62,    -1,    64,    -1,
      66,    -1,    62,    -1,    64,    -1,    -1,    67,    43,    -1,
      -1,    77,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      96,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    43,    -1,    62,    88,    64,
      -1,    -1,    67,    -1,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    38,    62,    -1,    64,    -1,    -1,    67,
      -1,    -1,    -1,    88,    72,    73,    74,    75,    76,    -1,
      95,    -1,    -1,    33,    59,    -1,    61,    62,    -1,    -1,
      88,    -1,    42,    68,    69,    70,    71,    95,    48,    49,
      50,    51,    52,    53,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    65,    -1,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    42,    -1,
      -1,    -1,    -1,    81,    48,    49,    50,    51,    52,    53,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    65,    -1,    -1,    -1,    81,    48,    49,    50,    51,
      52,    53,    -1,    42,    -1,    -1,    -1,    81,    60,    48,
      49,    50,    51,    52,    53,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    66,    42,    81,
      -1,    -1,    -1,    60,    48,    49,    50,    51,    52,    53,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      42,    -1,    54,    55,    81,    -1,    48,    49,    50,    51,
      52,    53,    64,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    99,   100,   103,   104,   106,    76,   101,
      76,   121,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    89,    90,    93,   105,   157,   186,
      59,    64,   102,     5,    59,    72,    76,   111,    76,   112,
     113,   120,   121,    76,    76,   155,   156,   185,   185,    76,
     187,   189,    76,    94,   118,   119,    26,    36,    76,    76,
      76,    26,    36,    76,   185,     5,    12,    76,     5,    58,
      53,    60,   113,     5,    58,    48,    59,   174,   156,    58,
      60,     5,    48,    59,   174,     5,    30,     3,    19,    20,
      21,    24,    25,    37,    40,    64,    72,    75,    76,    84,
      87,    88,    95,   122,   123,   126,   192,   193,   194,   197,
     198,   199,   200,   201,   204,   205,   207,   208,   209,   210,
     217,   218,   219,   220,    48,    53,    76,   119,    76,    76,
      76,    59,    59,    76,    76,    65,   103,   107,    72,    76,
      43,    54,    55,    62,    64,    67,    72,    73,    74,    75,
      76,    88,   123,   198,   201,   205,   222,   223,   224,   225,
     226,    12,    26,    27,    36,    54,    55,    61,    64,    72,
      73,    74,    75,    76,    80,    83,   117,   122,   126,   127,
     129,   139,   140,   141,   142,   121,   185,    76,    64,   175,
      76,   129,   185,    76,     5,   175,   188,    72,    76,    76,
     187,   222,   201,   205,   221,   222,   187,   222,   222,    60,
      60,    64,    96,   187,   222,    76,    76,    64,    59,    61,
      38,    59,    61,    62,    64,    38,    59,    61,    62,    68,
      69,    70,    71,   185,    12,    80,   129,    48,    59,    59,
      59,    76,    76,    48,    48,    59,     8,    10,    13,    26,
      32,    36,    39,   108,   226,   224,   224,    63,   222,   227,
     228,   222,   226,    59,     5,    42,    48,    49,    50,    51,
      52,    53,    81,    47,    54,    55,    78,    44,    45,    46,
      56,    57,    77,    82,    79,    62,    64,   134,    64,   180,
     180,    76,   142,   142,    76,   127,    76,   183,   184,    48,
      59,    62,    64,    28,    64,   135,    26,    36,    64,    76,
      80,    89,    90,   146,   147,   148,   185,    53,     6,    29,
      31,    34,    54,    55,    66,    44,    45,    56,    77,    50,
     174,     8,    26,    32,    36,    65,    76,   176,   177,   185,
       5,     5,    53,    76,    50,   174,   160,    60,   192,    38,
      42,    23,    17,    17,    58,    28,    30,    33,    65,    76,
     193,   193,    65,   202,   203,   205,   222,   128,   140,    85,
      86,    64,    48,    65,   202,    76,   222,    76,   206,   222,
      65,   202,   222,    76,   206,   222,   222,   222,   222,    50,
       5,     5,    64,     5,   185,    76,    76,    38,    76,   174,
     174,   185,   185,   112,   120,   104,    76,   155,    76,   118,
      66,    58,    63,    65,    76,   223,   223,   223,   223,   223,
     223,   223,   223,   224,   224,   224,   224,    55,   225,    55,
     225,    55,   225,    55,   225,    55,   225,   225,   225,   226,
      75,    76,   124,   125,   127,   135,     8,    26,    32,    36,
      65,    76,   181,   182,   185,    28,    60,    58,    65,   128,
      76,    54,    55,    64,    76,   117,   143,   144,   145,   143,
      76,   124,    26,    30,    36,    76,    80,    89,    90,    91,
     136,   185,    76,    76,    76,   148,    76,    76,    76,    30,
       5,    25,   152,    60,    64,   114,   222,    28,    62,    28,
      28,   132,   144,   144,   143,   145,   145,   145,   145,   174,
     175,   185,    76,   185,    76,   185,     5,    65,    60,     9,
     179,   114,    76,   174,   175,   103,    76,   127,   133,   222,
     222,   222,   190,   191,   190,   205,   211,   190,    58,    65,
      62,    60,    58,    97,    92,   187,   195,   196,   187,    65,
     202,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   174,   174,   174,   174,   175,   175,    50,
      50,     5,    14,    37,   109,   174,   174,   222,   228,   225,
     225,   225,   225,   225,    63,    58,    65,    30,   185,    76,
     185,    76,   185,     5,    65,    60,    76,   133,   184,    50,
     144,   144,   143,    64,    54,    55,    63,    44,    45,    56,
      77,    65,    65,    76,    76,     5,    26,    36,    76,    76,
      76,    60,   174,   174,    65,    38,    76,   174,   174,   148,
      76,   149,    76,   114,   116,   222,     5,   129,   140,   129,
     140,   146,   175,     5,    60,   180,    60,   180,    60,   177,
       6,    76,   127,   178,     5,    76,   158,     5,     5,   175,
      60,     5,    59,     5,    18,    22,    17,   192,   143,   213,
     214,   215,   216,    41,   203,   222,   222,   140,    76,    30,
       5,    30,    41,    30,    65,    50,    65,   202,    65,   202,
     222,    65,   202,   129,    65,    53,   175,   175,   175,   175,
       5,     5,   174,   174,   187,   187,    15,   110,   175,   175,
     125,   134,    60,   180,    60,   180,    60,   182,   178,    28,
      65,   143,   144,   144,   145,   145,   145,   145,   135,   174,
     174,    76,    76,   174,   174,    62,   137,   129,   175,   175,
     146,   174,   174,   175,   175,    28,    60,   129,    60,    58,
       5,    65,    58,    63,    30,     5,   179,   178,   178,    60,
     178,    28,    59,    53,     5,   103,    60,   133,    76,     9,
     179,   222,   222,   190,    66,     5,    16,    41,   212,    58,
      60,   190,    66,    60,    17,    60,   196,   187,    65,    65,
      65,     5,     5,   129,     5,    60,    60,     5,    60,   179,
     179,   175,   175,   187,    30,     5,    60,   135,   178,   178,
      60,   178,    76,    65,    30,   175,   175,   174,   174,   175,
     175,   176,    60,     5,    60,    30,   175,   175,   151,    76,
     114,   114,   115,    65,    76,   140,    28,   179,   165,    53,
     133,     8,    76,    76,   222,     5,   133,     5,     5,   159,
      17,    17,   143,   214,   187,   187,    30,   216,   190,   222,
     222,   190,    76,    30,     5,   179,   133,   133,   179,   133,
     167,   168,     5,    60,    59,   179,   133,    30,   133,     5,
      60,   175,   175,     5,     5,    63,   125,   133,    60,    60,
     153,   154,   215,    28,    58,    65,    60,    63,   130,   163,
     103,   222,     5,   179,   103,   190,   190,    63,    17,   169,
       5,     5,   172,     5,   103,   103,   179,   133,     5,   133,
       5,    60,   138,   133,   133,     5,    60,   151,   114,   114,
      28,   131,   103,     5,   179,   166,     5,   190,   103,   179,
     179,   103,   179,     5,     5,   161,     5,   179,     5,   133,
       5,    76,   154,    64,   153,   129,   129,     5,   164,   103,
       5,   173,   171,     5,   170,   103,   179,     5,    72,    76,
     150,   103,     5,   103,   103,   103,     5,   162,   146,     5,
       5,     5,     5,   103,    65,     5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,    99,   100,   100,   101,   101,   102,   102,
     103,   104,   104,   105,   105,   105,   105,   105,   105,   105,
     106,   107,   107,   108,   108,   108,   108,   108,   108,   109,
     109,   109,   110,   110,   111,   111,   111,   111,   112,   112,
     113,   113,   114,   114,   114,   114,   115,   115,   116,   116,
     117,   117,   117,   117,   117,   118,   118,   119,   119,   119,
     119,   119,   119,   120,   120,   121,   121,   122,   123,   123,
     124,   124,   125,   125,   126,   127,   127,   127,   128,   128,
     129,   129,   129,   129,   130,   131,   129,   129,   129,   129,
     129,   129,   129,   132,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   133,   133,   133,   134,   134,   135,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   137,   137,   138,   138,   138,   139,   139,   140,   140,
     140,   140,   140,   140,   141,   141,   141,   141,   141,   142,
     142,   142,   142,   142,   142,   143,   143,   143,   143,   143,
     144,   144,   144,   144,   144,   145,   145,   145,   146,   147,
     147,   149,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   150,   151,   152,   152,   152,   153,   153,   154,
     154,   155,   155,   156,   156,   156,   157,   157,   158,   157,
     159,   157,   160,   157,   161,   157,   162,   157,   163,   157,
     164,   157,   165,   157,   166,   157,   167,   157,   168,   157,
     169,   157,   170,   157,   171,   157,   172,   157,   173,   157,
     174,   175,   175,   175,   176,   176,   177,   177,   177,   177,
     177,   177,   177,   177,   178,   178,   178,   178,   178,   179,
     179,   180,   180,   180,   181,   181,   182,   182,   182,   182,
     182,   182,   183,   183,   184,   185,   185,   186,   188,   187,
     189,   187,   191,   190,   192,   192,   192,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   194,   194,   194,   194,   195,   195,   195,   196,
     196,   197,   197,   198,   198,   198,   199,   199,   199,   199,
     200,   200,   200,   201,   201,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   202,   202,   203,   203,   203,
     203,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   206,   206,   207,   208,
     208,   209,   209,   211,   210,   212,   212,   212,   213,   213,
     214,   214,   215,   215,   216,   216,   217,   218,   219,   219,
     219,   220,   221,   221,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   223,   223,   223,   223,   223,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   225,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   227,   227,   228,
     228
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
       4,     4,     7,     1,     3,     1,     3,     4,     1,     1,
       1,     3,     1,     1,     5,     1,     1,     4,     1,     3,
       1,     2,     3,     2,     0,     0,     9,     4,     2,     4,
       4,     6,     9,     0,     5,     4,     2,     4,     3,     6,
       3,     6,     4,     7,     1,     3,     1,     0,     3,     0,
       2,     1,     2,     4,     5,     7,     5,     5,     6,     8,
       7,     0,     3,     0,     3,     3,     1,     0,     3,     1,
       4,     3,     1,     3,     1,     2,     2,     3,     3,     1,
       4,     3,     3,     3,     3,     1,     2,     2,     3,     3,
       1,     3,     3,     3,     3,     1,     4,     3,     2,     3,
       1,     0,     4,     4,     6,     4,     4,     7,     2,     7,
       5,     0,     0,     0,     7,     5,     0,     3,     1,     6,
       0,     2,     1,     4,     6,     6,     7,     9,     0,     9,
       0,    11,     0,     7,     0,    13,     0,    15,     0,    12,
       0,    14,     0,    11,     0,    13,     0,    11,     0,    11,
       0,    12,     0,    14,     0,    14,     0,    12,     0,    14,
       0,     3,     2,     0,     3,     1,     3,     5,     4,     3,
       4,     5,     6,     4,     1,     3,     3,     3,     1,     0,
       3,     3,     2,     0,     3,     1,     3,     4,     4,     4,
       3,     5,     3,     1,     1,     3,     1,     3,     0,     4,
       0,     2,     0,     2,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     5,     5,     7,     5,     1,     3,     2,     6,
       4,     1,     2,     2,     4,     5,     1,     1,     3,     1,
       1,     3,     1,     4,     4,     3,     3,     4,     3,     6,
       5,     6,     5,     6,     5,     3,     1,     1,     3,     5,
       6,     3,     3,     3,     3,     3,     3,     4,     3,     2,
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
#line 224 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2311 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2317 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2323 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2329 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2335 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2341 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2347 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2353 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2359 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2365 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2371 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2377 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2383 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2389 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2395 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2401 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2407 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2413 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2419 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2431 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2437 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2443 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2449 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2455 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2461 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2467 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2473 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2479 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2485 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2497 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2503 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2509 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2515 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2521 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2527 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2533 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2539 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2551 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2557 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2563 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2569 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2575 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2581 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2587 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2593 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2599 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2605 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2611 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2617 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2623 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2629 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2635 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2641 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2647 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2653 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2659 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2665 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2671 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2677 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2683 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2689 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2695 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2701 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2707 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2713 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2719 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2725 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2731 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2737 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2743 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2749 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2755 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2761 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2767 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2773 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2779 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2785 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2791 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2797 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2803 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2809 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2815 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2821 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2827 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2833 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2839 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2845 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2851 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2857 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2863 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2869 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2875 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2881 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2887 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2893 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2899 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2905 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2911 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2917 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2923 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2929 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2935 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2941 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2947 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2953 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2959 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2965 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2971 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2977 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2983 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2989 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2995 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3001 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3007 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3013 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3019 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3025 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3031 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3037 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3043 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3049 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3055 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3061 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3067 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3073 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3079 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3085 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3091 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3097 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3103 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3109 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3115 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3121 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3133 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 439 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3139 "pascal.tab.c"
    break;

  case 141: /* lo_term: lo_term MUL b_factor  */
#line 440 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3145 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term IDIV b_factor  */
#line 441 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3151 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IMOD b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3157 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term SHIFTOP b_factor  */
#line 443 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3163 "pascal.tab.c"
    break;

  case 145: /* b_expr: b_term  */
#line 446 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3169 "pascal.tab.c"
    break;

  case 146: /* b_expr: PLUS b_term  */
#line 447 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3175 "pascal.tab.c"
    break;

  case 147: /* b_expr: MINUS b_term  */
#line 448 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3181 "pascal.tab.c"
    break;

  case 148: /* b_expr: b_expr PLUS b_term  */
#line 449 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3187 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr MINUS b_term  */
#line 450 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3193 "pascal.tab.c"
    break;

  case 150: /* b_term: b_factor  */
#line 453 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3199 "pascal.tab.c"
    break;

  case 151: /* b_term: b_term MUL b_factor  */
#line 454 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3205 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term IDIV b_factor  */
#line 455 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3211 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IMOD b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3217 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term SHIFTOP b_factor  */
#line 457 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3223 "pascal.tab.c"
    break;

  case 155: /* b_factor: scalar_constant  */
#line 460 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3229 "pascal.tab.c"
    break;

  case 156: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 461 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3235 "pascal.tab.c"
    break;

  case 157: /* b_factor: LPARENT b_expr RPARENT  */
#line 462 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3241 "pascal.tab.c"
    break;

  case 158: /* record_body: record_field_list record_case_opt  */
#line 465 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3247 "pascal.tab.c"
    break;

  case 159: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 468 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3253 "pascal.tab.c"
    break;

  case 160: /* record_field_list: record_field  */
#line 469 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3259 "pascal.tab.c"
    break;

  case 161: /* $@4: %empty  */
#line 472 "pascal.y"
                  { }
#line 3265 "pascal.tab.c"
    break;

  case 162: /* record_field: id_list COLON $@4 type  */
#line 472 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3271 "pascal.tab.c"
    break;

  case 163: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 473 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3277 "pascal.tab.c"
    break;

  case 164: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 474 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3283 "pascal.tab.c"
    break;

  case 165: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3289 "pascal.tab.c"
    break;

  case 166: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 476 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3295 "pascal.tab.c"
    break;

  case 167: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 477 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3301 "pascal.tab.c"
    break;

  case 168: /* record_field: IDENT record_field  */
#line 478 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3307 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3313 "pascal.tab.c"
    break;

  case 170: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 480 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3319 "pascal.tab.c"
    break;

  case 171: /* record_field: %empty  */
#line 481 "pascal.y"
     { (yyval.node) = NULL; }
#line 3325 "pascal.tab.c"
    break;

  case 172: /* case_arm_mark: %empty  */
#line 484 "pascal.y"
    { }
#line 3331 "pascal.tab.c"
    break;

  case 173: /* case_open: %empty  */
#line 487 "pascal.y"
    { }
#line 3337 "pascal.tab.c"
    break;

  case 174: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 490 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3343 "pascal.tab.c"
    break;

  case 175: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 491 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3349 "pascal.tab.c"
    break;

  case 176: /* record_case_opt: %empty  */
#line 492 "pascal.y"
     { (yyval.node) = NULL; }
#line 3355 "pascal.tab.c"
    break;

  case 177: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 495 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3361 "pascal.tab.c"
    break;

  case 178: /* record_case_list: record_case_arm  */
#line 496 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3367 "pascal.tab.c"
    break;

  case 179: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 499 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3373 "pascal.tab.c"
    break;

  case 180: /* record_case_arm: %empty  */
#line 500 "pascal.y"
     { (yyval.node) = NULL; }
#line 3379 "pascal.tab.c"
    break;

  case 181: /* var_decl_list: var_decl_list var_decl  */
#line 503 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3385 "pascal.tab.c"
    break;

  case 182: /* var_decl_list: var_decl  */
#line 504 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3391 "pascal.tab.c"
    break;

  case 183: /* var_decl: id_list COLON type SEMICOLON  */
#line 507 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3397 "pascal.tab.c"
    break;

  case 184: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 508 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3403 "pascal.tab.c"
    break;

  case 185: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 509 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3409 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 512 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3415 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 513 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3421 "pascal.tab.c"
    break;

  case 188: /* $@5: %empty  */
#line 514 "pascal.y"
                                                                               { }
#line 3427 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3433 "pascal.tab.c"
    break;

  case 190: /* $@6: %empty  */
#line 515 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3439 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 515 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3445 "pascal.tab.c"
    break;

  case 192: /* $@7: %empty  */
#line 516 "pascal.y"
                                        { }
#line 3451 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 516 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3457 "pascal.tab.c"
    break;

  case 194: /* $@8: %empty  */
#line 517 "pascal.y"
                                                                                                           { }
#line 3463 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3469 "pascal.tab.c"
    break;

  case 196: /* $@9: %empty  */
#line 518 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3475 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3481 "pascal.tab.c"
    break;

  case 198: /* $@10: %empty  */
#line 519 "pascal.y"
                                                                                                 { }
#line 3487 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3493 "pascal.tab.c"
    break;

  case 200: /* $@11: %empty  */
#line 520 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3499 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3505 "pascal.tab.c"
    break;

  case 202: /* $@12: %empty  */
#line 521 "pascal.y"
                                                                                            { }
#line 3511 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3517 "pascal.tab.c"
    break;

  case 204: /* $@13: %empty  */
#line 522 "pascal.y"
                                                                                                             { }
#line 3523 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3531 "pascal.tab.c"
    break;

  case 206: /* $@14: %empty  */
#line 525 "pascal.y"
                                                                                              { }
#line 3537 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 525 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3545 "pascal.tab.c"
    break;

  case 208: /* $@15: %empty  */
#line 528 "pascal.y"
                                                                                             { }
#line 3551 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 528 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3559 "pascal.tab.c"
    break;

  case 210: /* $@16: %empty  */
#line 531 "pascal.y"
                                                                                                    { }
#line 3565 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 531 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3573 "pascal.tab.c"
    break;

  case 212: /* $@17: %empty  */
#line 534 "pascal.y"
                                                                                                                { }
#line 3579 "pascal.tab.c"
    break;

  case 213: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 534 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3588 "pascal.tab.c"
    break;

  case 214: /* $@18: %empty  */
#line 538 "pascal.y"
                                                                                                                  { }
#line 3594 "pascal.tab.c"
    break;

  case 215: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 538 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3603 "pascal.tab.c"
    break;

  case 216: /* $@19: %empty  */
#line 542 "pascal.y"
                                                                                              { }
#line 3609 "pascal.tab.c"
    break;

  case 217: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 542 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3618 "pascal.tab.c"
    break;

  case 218: /* $@20: %empty  */
#line 546 "pascal.y"
                                                                                                                     { }
#line 3624 "pascal.tab.c"
    break;

  case 219: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 546 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3632 "pascal.tab.c"
    break;

  case 220: /* pv_mark: %empty  */
#line 551 "pascal.y"
    { }
#line 3638 "pascal.tab.c"
    break;

  case 221: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 554 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3644 "pascal.tab.c"
    break;

  case 222: /* parameter_list_opt: LPARENT RPARENT  */
#line 555 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3650 "pascal.tab.c"
    break;

  case 223: /* parameter_list_opt: %empty  */
#line 556 "pascal.y"
     { (yyval.node) = NULL; }
#line 3656 "pascal.tab.c"
    break;

  case 224: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 559 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3662 "pascal.tab.c"
    break;

  case 225: /* parameter_decl_list: parameter_decl  */
#line 560 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3668 "pascal.tab.c"
    break;

  case 226: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 563 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3674 "pascal.tab.c"
    break;

  case 227: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 564 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3680 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: VARSY id_list COLON param_type  */
#line 565 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3686 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: id_list COLON param_type  */
#line 566 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3692 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 567 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3698 "pascal.tab.c"
    break;

  case 231: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 568 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3704 "pascal.tab.c"
    break;

  case 232: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 569 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3710 "pascal.tab.c"
    break;

  case 233: /* parameter_decl: IDENT id_list COLON param_type  */
#line 570 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3716 "pascal.tab.c"
    break;

  case 234: /* param_type: IDENT  */
#line 573 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3722 "pascal.tab.c"
    break;

  case 235: /* param_type: IDENT PERIOD IDENT  */
#line 574 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3728 "pascal.tab.c"
    break;

  case 236: /* param_type: ARRAYSY OFSY IDENT  */
#line 575 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3734 "pascal.tab.c"
    break;

  case 237: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 576 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3740 "pascal.tab.c"
    break;

  case 238: /* param_type: spec_ref  */
#line 577 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3746 "pascal.tab.c"
    break;

  case 239: /* routine_directives: %empty  */
#line 580 "pascal.y"
    { (yyval.node) = NULL; }
#line 3752 "pascal.tab.c"
    break;

  case 240: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 581 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3765 "pascal.tab.c"
    break;

  case 241: /* pf_params: LPARENT pf_sections RPARENT  */
#line 591 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3771 "pascal.tab.c"
    break;

  case 242: /* pf_params: LPARENT RPARENT  */
#line 592 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3777 "pascal.tab.c"
    break;

  case 243: /* pf_params: %empty  */
#line 593 "pascal.y"
     { (yyval.node) = NULL; }
#line 3783 "pascal.tab.c"
    break;

  case 244: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 596 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3789 "pascal.tab.c"
    break;

  case 245: /* pf_sections: pf_section  */
#line 597 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3795 "pascal.tab.c"
    break;

  case 246: /* pf_section: id_list COLON param_type  */
#line 600 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3801 "pascal.tab.c"
    break;

  case 247: /* pf_section: VARSY id_list COLON param_type  */
#line 601 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3807 "pascal.tab.c"
    break;

  case 248: /* pf_section: CONSTSY id_list COLON param_type  */
#line 602 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3813 "pascal.tab.c"
    break;

  case 249: /* pf_section: IDENT id_list COLON param_type  */
#line 603 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3819 "pascal.tab.c"
    break;

  case 250: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 604 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3825 "pascal.tab.c"
    break;

  case 251: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 605 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3831 "pascal.tab.c"
    break;

  case 252: /* enum_list: enum_list COMMA enum_member  */
#line 608 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3837 "pascal.tab.c"
    break;

  case 253: /* enum_list: enum_member  */
#line 609 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3843 "pascal.tab.c"
    break;

  case 254: /* enum_member: IDENT  */
#line 612 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3849 "pascal.tab.c"
    break;

  case 255: /* id_list: id_list COMMA IDENT  */
#line 615 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3855 "pascal.tab.c"
    break;

  case 256: /* id_list: IDENT  */
#line 616 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3861 "pascal.tab.c"
    break;

  case 257: /* body: BEGINSY statement_list ENDSY  */
#line 619 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3867 "pascal.tab.c"
    break;

  case 258: /* $@21: %empty  */
#line 622 "pascal.y"
                             { pas_stmt_mark(); }
#line 3873 "pascal.tab.c"
    break;

  case 259: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 622 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3879 "pascal.tab.c"
    break;

  case 260: /* $@22: %empty  */
#line 623 "pascal.y"
     { pas_stmt_mark(); }
#line 3885 "pascal.tab.c"
    break;

  case 261: /* statement_list: $@22 statement  */
#line 623 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3891 "pascal.tab.c"
    break;

  case 262: /* $@23: %empty  */
#line 626 "pascal.y"
    { pas_stmt_mark(); }
#line 3897 "pascal.tab.c"
    break;

  case 263: /* body_stmt: $@23 statement  */
#line 626 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3903 "pascal.tab.c"
    break;

  case 264: /* statement: statement_no_label  */
#line 629 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3909 "pascal.tab.c"
    break;

  case 265: /* statement: INTCONST COLON statement_no_label  */
#line 630 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3915 "pascal.tab.c"
    break;

  case 266: /* statement: IDENT COLON statement_no_label  */
#line 631 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3921 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: assignment  */
#line 634 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3927 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: call  */
#line 635 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3933 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: compound_statement  */
#line 636 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3939 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: goto_statement  */
#line 637 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3945 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: if_statement  */
#line 638 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3951 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: case_statement  */
#line 639 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3957 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: while_statement  */
#line 640 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3963 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: repeat_statement  */
#line 641 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3969 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: for_statement  */
#line 642 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3975 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: with_statement  */
#line 643 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3981 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: try_statement  */
#line 644 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3987 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: raise_statement  */
#line 645 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3993 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: inherited_call  */
#line 646 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3999 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: INHERITEDSY  */
#line 647 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4005 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: %empty  */
#line 648 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4011 "pascal.tab.c"
    break;

  case 282: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 651 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4017 "pascal.tab.c"
    break;

  case 283: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 652 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4023 "pascal.tab.c"
    break;

  case 284: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 653 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4029 "pascal.tab.c"
    break;

  case 285: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 654 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4035 "pascal.tab.c"
    break;

  case 286: /* handler_list: handler  */
#line 657 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4041 "pascal.tab.c"
    break;

  case 287: /* handler_list: handler_list SEMICOLON handler  */
#line 658 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4047 "pascal.tab.c"
    break;

  case 288: /* handler_list: handler_list SEMICOLON  */
#line 659 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4053 "pascal.tab.c"
    break;

  case 289: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 662 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4059 "pascal.tab.c"
    break;

  case 290: /* handler: ONSY IDENT DOSY body_stmt  */
#line 663 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4065 "pascal.tab.c"
    break;

  case 291: /* raise_statement: RAISESY  */
#line 666 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4071 "pascal.tab.c"
    break;

  case 292: /* raise_statement: RAISESY expression  */
#line 667 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4077 "pascal.tab.c"
    break;

  case 293: /* inherited_call: INHERITEDSY IDENT  */
#line 670 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4083 "pascal.tab.c"
    break;

  case 294: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 671 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4089 "pascal.tab.c"
    break;

  case 295: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 672 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4095 "pascal.tab.c"
    break;

  case 296: /* call: IDENT  */
#line 675 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4101 "pascal.tab.c"
    break;

  case 297: /* call: spec_expr  */
#line 676 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4107 "pascal.tab.c"
    break;

  case 298: /* call: selector PERIOD IDENT  */
#line 677 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4113 "pascal.tab.c"
    break;

  case 299: /* call: call_with_args  */
#line 678 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4119 "pascal.tab.c"
    break;

  case 300: /* recv_primary: STRINGCONST  */
#line 681 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4125 "pascal.tab.c"
    break;

  case 301: /* recv_primary: LPARENT expression RPARENT  */
#line 682 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4131 "pascal.tab.c"
    break;

  case 302: /* recv_primary: spec_expr  */
#line 683 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4137 "pascal.tab.c"
    break;

  case 303: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 686 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4143 "pascal.tab.c"
    break;

  case 304: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 687 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4149 "pascal.tab.c"
    break;

  case 305: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 688 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4155 "pascal.tab.c"
    break;

  case 306: /* call_with_args: IDENT LPARENT RPARENT  */
#line 689 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4161 "pascal.tab.c"
    break;

  case 307: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 690 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4167 "pascal.tab.c"
    break;

  case 308: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 691 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4173 "pascal.tab.c"
    break;

  case 309: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 692 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4179 "pascal.tab.c"
    break;

  case 310: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 693 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4185 "pascal.tab.c"
    break;

  case 311: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 694 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4191 "pascal.tab.c"
    break;

  case 312: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 695 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4197 "pascal.tab.c"
    break;

  case 313: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 696 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4203 "pascal.tab.c"
    break;

  case 314: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 697 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4209 "pascal.tab.c"
    break;

  case 315: /* argument_list: argument_list COMMA argument  */
#line 700 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4215 "pascal.tab.c"
    break;

  case 316: /* argument_list: argument  */
#line 701 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4221 "pascal.tab.c"
    break;

  case 317: /* argument: expression  */
#line 704 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4227 "pascal.tab.c"
    break;

  case 318: /* argument: expression COLON expression  */
#line 705 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4233 "pascal.tab.c"
    break;

  case 319: /* argument: expression COLON expression COLON expression  */
#line 706 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4239 "pascal.tab.c"
    break;

  case 320: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 707 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4245 "pascal.tab.c"
    break;

  case 321: /* assignment: selector BECOMES expression  */
#line 710 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4251 "pascal.tab.c"
    break;

  case 322: /* assignment: selector ADDBECOMES expression  */
#line 711 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4257 "pascal.tab.c"
    break;

  case 323: /* assignment: selector SUBBECOMES expression  */
#line 712 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4263 "pascal.tab.c"
    break;

  case 324: /* assignment: selector MULBECOMES expression  */
#line 713 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4269 "pascal.tab.c"
    break;

  case 325: /* assignment: selector DIVBECOMES expression  */
#line 714 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4275 "pascal.tab.c"
    break;

  case 326: /* assignment: call_with_args BECOMES expression  */
#line 715 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4281 "pascal.tab.c"
    break;

  case 327: /* selector: selector LBRACK expression_list RBRACK  */
#line 718 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4287 "pascal.tab.c"
    break;

  case 328: /* selector: selector PERIOD IDENT  */
#line 719 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4293 "pascal.tab.c"
    break;

  case 329: /* selector: selector ARROW  */
#line 720 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4299 "pascal.tab.c"
    break;

  case 330: /* selector: call_with_args PERIOD IDENT  */
#line 721 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4305 "pascal.tab.c"
    break;

  case 331: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 722 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4311 "pascal.tab.c"
    break;

  case 332: /* selector: call_with_args ARROW  */
#line 723 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4317 "pascal.tab.c"
    break;

  case 333: /* selector: recv_primary PERIOD IDENT  */
#line 724 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4323 "pascal.tab.c"
    break;

  case 334: /* selector: recv_primary ARROW  */
#line 725 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4329 "pascal.tab.c"
    break;

  case 335: /* selector: IDENT  */
#line 726 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4335 "pascal.tab.c"
    break;

  case 336: /* expression_list: expression_list COMMA expression  */
#line 729 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4341 "pascal.tab.c"
    break;

  case 337: /* expression_list: expression  */
#line 730 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4347 "pascal.tab.c"
    break;

  case 338: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 733 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4353 "pascal.tab.c"
    break;

  case 339: /* goto_statement: GOTOSY INTCONST  */
#line 736 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4359 "pascal.tab.c"
    break;

  case 340: /* goto_statement: GOTOSY IDENT  */
#line 737 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4365 "pascal.tab.c"
    break;

  case 341: /* if_statement: IFSY expression THENSY body_stmt  */
#line 740 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4371 "pascal.tab.c"
    break;

  case 342: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 741 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4377 "pascal.tab.c"
    break;

  case 343: /* $@24: %empty  */
#line 744 "pascal.y"
                           { }
#line 4383 "pascal.tab.c"
    break;

  case 344: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 744 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4389 "pascal.tab.c"
    break;

  case 345: /* case_tail: %empty  */
#line 747 "pascal.y"
    { (yyval.node) = NULL; }
#line 4395 "pascal.tab.c"
    break;

  case 346: /* case_tail: ELSESY statement_list  */
#line 748 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4401 "pascal.tab.c"
    break;

  case 347: /* case_tail: OTHERWISESY statement_list  */
#line 749 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4407 "pascal.tab.c"
    break;

  case 348: /* case_list: case_list SEMICOLON case_elem  */
#line 752 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4413 "pascal.tab.c"
    break;

  case 349: /* case_list: case_elem  */
#line 753 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4419 "pascal.tab.c"
    break;

  case 350: /* case_elem: constant_list COLON body_stmt  */
#line 756 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4425 "pascal.tab.c"
    break;

  case 351: /* case_elem: %empty  */
#line 757 "pascal.y"
     { (yyval.node) = NULL; }
#line 4431 "pascal.tab.c"
    break;

  case 352: /* constant_list: constant_list COMMA case_label  */
#line 760 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4437 "pascal.tab.c"
    break;

  case 353: /* constant_list: case_label  */
#line 761 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4443 "pascal.tab.c"
    break;

  case 354: /* case_label: b_expr  */
#line 764 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4449 "pascal.tab.c"
    break;

  case 355: /* case_label: b_expr DOTDOT b_expr  */
#line 765 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4455 "pascal.tab.c"
    break;

  case 356: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 768 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4461 "pascal.tab.c"
    break;

  case 357: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 771 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4467 "pascal.tab.c"
    break;

  case 358: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 774 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4473 "pascal.tab.c"
    break;

  case 359: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 775 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4479 "pascal.tab.c"
    break;

  case 360: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 776 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4485 "pascal.tab.c"
    break;

  case 361: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 779 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4491 "pascal.tab.c"
    break;

  case 362: /* with_open: with_open COMMA selector  */
#line 782 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4497 "pascal.tab.c"
    break;

  case 363: /* with_open: selector  */
#line 783 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4503 "pascal.tab.c"
    break;

  case 364: /* expression: simple_expression  */
#line 790 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4509 "pascal.tab.c"
    break;

  case 365: /* expression: expression INOP simple_expression  */
#line 791 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4515 "pascal.tab.c"
    break;

  case 366: /* expression: expression LTOP simple_expression  */
#line 792 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4521 "pascal.tab.c"
    break;

  case 367: /* expression: expression LEOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4527 "pascal.tab.c"
    break;

  case 368: /* expression: expression GTOP simple_expression  */
#line 794 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4533 "pascal.tab.c"
    break;

  case 369: /* expression: expression GEOP simple_expression  */
#line 795 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4539 "pascal.tab.c"
    break;

  case 370: /* expression: expression NEOP simple_expression  */
#line 796 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4545 "pascal.tab.c"
    break;

  case 371: /* expression: expression EQOP simple_expression  */
#line 797 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4551 "pascal.tab.c"
    break;

  case 372: /* expression: expression ISOP simple_expression  */
#line 798 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4557 "pascal.tab.c"
    break;

  case 373: /* simple_expression: term  */
#line 801 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4563 "pascal.tab.c"
    break;

  case 374: /* simple_expression: PLUS term  */
#line 802 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4569 "pascal.tab.c"
    break;

  case 375: /* simple_expression: MINUS term  */
#line 803 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4575 "pascal.tab.c"
    break;

  case 376: /* simple_expression: simple_expression PLUS term  */
#line 804 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4581 "pascal.tab.c"
    break;

  case 377: /* simple_expression: simple_expression MINUS term  */
#line 805 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4587 "pascal.tab.c"
    break;

  case 378: /* simple_expression: simple_expression OROP term  */
#line 806 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4593 "pascal.tab.c"
    break;

  case 379: /* simple_expression: simple_expression XOROP term  */
#line 807 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4599 "pascal.tab.c"
    break;

  case 380: /* term: power  */
#line 810 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4605 "pascal.tab.c"
    break;

  case 381: /* term: term MUL power  */
#line 811 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4611 "pascal.tab.c"
    break;

  case 382: /* term: term RDIV power  */
#line 812 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4617 "pascal.tab.c"
    break;

  case 383: /* term: term IDIV power  */
#line 813 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4623 "pascal.tab.c"
    break;

  case 384: /* term: term IMOD power  */
#line 814 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4629 "pascal.tab.c"
    break;

  case 385: /* term: term ANDOP power  */
#line 815 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4635 "pascal.tab.c"
    break;

  case 386: /* term: term MUL MINUS power  */
#line 816 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4641 "pascal.tab.c"
    break;

  case 387: /* term: term RDIV MINUS power  */
#line 817 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4647 "pascal.tab.c"
    break;

  case 388: /* term: term IDIV MINUS power  */
#line 818 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4653 "pascal.tab.c"
    break;

  case 389: /* term: term IMOD MINUS power  */
#line 819 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4659 "pascal.tab.c"
    break;

  case 390: /* term: term ANDOP MINUS power  */
#line 820 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4665 "pascal.tab.c"
    break;

  case 391: /* term: term SHIFTOP power  */
#line 821 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4671 "pascal.tab.c"
    break;

  case 392: /* term: term ASOP power  */
#line 822 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4677 "pascal.tab.c"
    break;

  case 393: /* power: factor  */
#line 825 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4683 "pascal.tab.c"
    break;

  case 394: /* power: power POWOP factor  */
#line 826 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4689 "pascal.tab.c"
    break;

  case 395: /* factor: selector  */
#line 829 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4695 "pascal.tab.c"
    break;

  case 396: /* factor: call_with_args  */
#line 830 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4701 "pascal.tab.c"
    break;

  case 397: /* factor: INTCONST  */
#line 831 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4707 "pascal.tab.c"
    break;

  case 398: /* factor: REALCONST  */
#line 832 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4713 "pascal.tab.c"
    break;

  case 399: /* factor: STRINGCONST  */
#line 833 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4719 "pascal.tab.c"
    break;

  case 400: /* factor: CHARCODE  */
#line 834 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4725 "pascal.tab.c"
    break;

  case 401: /* factor: LPARENT expression RPARENT  */
#line 835 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4731 "pascal.tab.c"
    break;

  case 402: /* factor: inherited_call  */
#line 836 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4737 "pascal.tab.c"
    break;

  case 403: /* factor: NOTSY factor  */
#line 837 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4743 "pascal.tab.c"
    break;

  case 404: /* factor: ATSIGN factor  */
#line 838 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4749 "pascal.tab.c"
    break;

  case 405: /* factor: LBRACK RBRACK  */
#line 839 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4755 "pascal.tab.c"
    break;

  case 406: /* factor: LBRACK set_member_list RBRACK  */
#line 840 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4761 "pascal.tab.c"
    break;

  case 407: /* set_member_list: set_member  */
#line 843 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4767 "pascal.tab.c"
    break;

  case 408: /* set_member_list: set_member_list COMMA set_member  */
#line 844 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4773 "pascal.tab.c"
    break;

  case 409: /* set_member: expression  */
#line 847 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4779 "pascal.tab.c"
    break;

  case 410: /* set_member: expression DOTDOT expression  */
#line 848 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4785 "pascal.tab.c"
    break;


#line 4789 "pascal.tab.c"

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

#line 854 "pascal.y"

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
