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
#define YYLAST   1854

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  412
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  990

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
     439,   440,   441,   442,   443,   444,   447,   448,   449,   450,
     451,   454,   455,   456,   457,   458,   461,   462,   463,   464,
     467,   470,   471,   474,   474,   475,   476,   477,   478,   479,
     480,   481,   482,   483,   486,   489,   492,   493,   494,   497,
     498,   501,   502,   505,   506,   509,   510,   511,   514,   515,
     516,   516,   517,   517,   518,   518,   519,   519,   520,   520,
     521,   521,   522,   522,   523,   523,   524,   524,   527,   527,
     530,   530,   533,   533,   536,   536,   540,   540,   544,   544,
     548,   548,   553,   556,   557,   558,   561,   562,   565,   566,
     567,   568,   569,   570,   571,   572,   575,   576,   577,   578,
     579,   582,   583,   593,   594,   595,   598,   599,   602,   603,
     604,   605,   606,   607,   610,   611,   614,   617,   618,   621,
     624,   624,   625,   625,   628,   628,   631,   632,   633,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   653,   654,   655,   656,   659,   660,
     661,   664,   665,   668,   669,   672,   673,   674,   677,   678,
     679,   680,   683,   684,   685,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   702,   703,   706,
     707,   708,   709,   712,   713,   714,   715,   716,   717,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   731,   732,
     735,   738,   739,   742,   743,   746,   746,   749,   750,   751,
     754,   755,   758,   759,   762,   763,   766,   767,   770,   773,
     776,   777,   778,   781,   784,   785,   792,   793,   794,   795,
     796,   797,   798,   799,   800,   803,   804,   805,   806,   807,
     808,   809,   812,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   827,   828,   831,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,   845,
     846,   849,   850
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

#define YYPACT_NINF (-568)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-338)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     134,     7,    90,   197,  -568,   162,   980,  -568,  -568,   493,
    -568,   113,  -568,  -568,   151,   163,    90,   171,   193,   193,
     274,  -568,   -19,   141,   288,   311,   323,  -568,  -568,  -568,
     378,   193,   295,   445,   419,  -568,  -568,    89,   457,   163,
    -568,   145,   468,   165,  -568,   193,  -568,     2,   153,   346,
     185,    65,   535,   480,   -19,  -568,   521,   548,   573,   640,
     643,   641,   650,  -568,   380,  -568,  -568,  -568,  -568,   331,
    1395,  1035,  -568,  -568,    90,   193,   656,   683,  -568,   675,
    1035,  -568,   193,   686,    87,  -568,  -568,   407,   698,  -568,
    1395,   454,  1395,  -568,  1395,  1395,   699,  -568,   887,  -568,
    1395,   713,   718,  -568,   509,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,   408,   564,  -568,   600,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   193,  1129,   756,  -568,   752,   761,
     767,   753,   754,   797,   805,  -568,   803,  1815,  -568,  -568,
    1603,  1603,  1603,  1172,  1395,  1603,  -568,  -568,  -568,   617,
     -16,   713,   795,  -568,   711,   773,   203,   372,   659,   784,
    -568,   581,   814,  -568,   814,   742,   742,   -24,   806,  -568,
    -568,  -568,  -568,  1480,   164,   200,  -568,  -568,  -568,  -568,
     834,   312,  -568,   343,   447,   468,    16,  -568,   234,   862,
    -568,    51,   224,  -568,  -568,   826,    65,  -568,  -568,   470,
     273,   729,   711,   773,   152,   716,   282,   791,  1644,   889,
     889,  1207,   867,    48,  1768,   830,   842,  1232,   819,  -568,
    1395,   822,  -568,  1395,  1257,  1395,   839,  -568,  1395,  1395,
    1395,  1395,  1395,   269,    82,    60,   912,   193,   843,   851,
     241,  -568,  -568,   193,   193,  -568,   163,    90,  -568,   852,
     193,   854,   -19,  -568,  -568,   659,   659,  -568,  1662,   543,
    -568,  1681,  -568,   861,  -568,  1395,  1395,  1395,  1395,  1395,
    1395,  1395,  1395,  1603,  1603,  1603,  1603,  1486,  1511,  1536,
    1561,  1586,  1603,  1603,  1603,   831,   130,  -568,   382,   890,
     863,   552,   447,   447,    -3,  -568,  -568,   496,  -568,   867,
     868,  1004,  1004,   869,   130,   129,   883,   884,   885,   577,
     893,   894,   895,   942,   207,  -568,   305,  1420,    15,   892,
     946,  -568,   860,   860,  1004,   860,   860,   860,   860,  -568,
     683,   193,   899,   193,   902,  -568,   193,    30,  -568,   692,
    -568,  -568,  1420,   903,  -568,   683,  -568,   216,  -568,  1395,
    1395,  1395,  -568,  -568,   454,  -568,  -568,  -568,  -568,   930,
    -568,  -568,  -568,   514,  -568,   788,  1699,   136,  -568,   888,
    -568,  1282,   867,  -568,   557,   917,  1768,   921,   632,  1768,
    -568,   571,  1768,  1765,   660,  1768,  1768,  1768,  1768,   933,
    -568,  -568,   130,  -568,   375,  -568,  -568,  -568,  -568,   683,
     683,   451,   464,   163,   173,   236,  -568,   193,  -568,   -19,
    1395,  1395,  -568,   695,   931,   372,   372,   372,   372,   372,
     372,   372,   372,   659,   659,   659,   659,  1603,   784,  1603,
     784,  1603,   784,  1603,   784,  1603,   784,   784,   784,  -568,
     941,    -3,   576,  -568,  -568,   541,   193,   932,   193,   937,
    -568,   193,    71,  -568,   747,   947,   216,   948,   806,  -568,
     484,  1186,   860,   860,  1004,   599,  -568,   369,   460,  -568,
     398,  -568,   582,   951,  -568,   952,   138,   365,   953,   956,
     961,  -568,   778,  -568,  -568,   944,  -568,   260,  -568,  -568,
    -568,   326,   963,  -568,  -568,  1445,  1036,  1768,  1035,   867,
    1035,   867,   326,   460,   460,   655,  -568,  -568,  -568,  -568,
     683,  1037,   794,   814,   800,   814,   808,    46,  -568,    40,
      54,  1038,  1043,   683,   989,  1045,   189,  -568,  1052,  1003,
    1097,  1768,  -568,    65,  -568,   773,  1004,   999,  1395,  -568,
    1395,  1395,   867,  -568,   987,   283,   301,  -568,   348,  -568,
     607,   516,  -568,  1320,  1345,  1395,  -568,  -568,  1370,  -568,
    1035,   619,  1011,   683,   683,   683,   683,  1060,  1062,  -568,
    -568,  -568,  -568,  -568,  1057,   683,   683,  1768,  -568,   784,
     784,   784,   784,   784,  -568,   130,  1013,  -568,   815,   814,
     823,   814,   833,   385,  -568,    40,  -568,  1053,  -568,  -568,
    -568,   460,   460,   432,  1006,  1004,   860,   860,  -568,   860,
     860,   860,   860,  -568,  -568,  -568,  -568,  -568,  1007,  1009,
    -568,  -568,  1026,  1035,   683,   683,   326,  -568,  -568,   683,
     683,  -568,   135,  1035,   237,  1033,    74,  1681,  -568,  -568,
     667,  -568,  -568,  1063,  1087,  -568,    40,  -568,    40,  1034,
      40,  -568,  1067,   190,  -568,  1044,  1093,  1095,  -568,  -568,
    -568,  1041,   216,  -568,  1028,  -568,  1395,  1395,  -568,  -568,
     434,   363,  -568,   844,  -568,  -568,  -568,  1715,  1734,  -568,
     144,  -568,   888,  -568,  -568,  -568,  -568,  -568,  -568,   623,
    -568,   624,  1768,  -568,   628,  1101,  1107,  1035,  1114,  1056,
    1064,    68,  -568,  -568,   683,   683,  1115,  1115,  -568,  1102,
    1118,  1065,  -568,  -568,    40,  -568,    40,  1073,    40,  -568,
    -568,  1058,  -568,  -568,   481,   460,   460,  -568,  -568,  -568,
    -568,   584,   683,   683,  -568,  -568,   683,   683,    46,  1075,
    1131,  -568,  1077,  1108,   683,   683,  -568,  -568,  -568,  1076,
    -568,  1420,  1420,   334,  -568,   867,  1112,  -568,  -568,  1078,
    1090,  -568,   216,  -568,    50,  1081,  1395,  -568,  -568,  1146,
     216,  1153,  -568,    55,  1111,  1168,  -568,  1004,  1004,  -568,
    -568,  1144,  1004,  -568,  -568,  1395,  1395,  -568,  1099,  -568,
     366,  -568,  -568,  -568,  -568,  -568,  1171,  -568,   216,   216,
    -568,   216,  1078,  1078,  1174,  1117,  1115,  1122,  -568,   216,
     712,  -568,  -568,   216,  -568,  -568,  -568,  -568,  1177,  1126,
     683,   683,  1183,  1184,    33,   130,  -568,   216,  -568,  1134,
    1137,  1004,  1163,  -568,  -568,   636,  -568,  1138,  1136,  -568,
    1078,  -568,  1395,  -568,  -568,  -568,  -568,  1768,  -568,  1195,
    -568,  1201,  -568,  -568,  -568,   655,  -568,  1115,  1115,  -568,
    -568,  -568,  1749,  1768,  -568,  1194,  -568,  -568,  1078,  1208,
    1209,  1078,  1217,  -568,  -568,  -568,   216,  -568,  1078,  1218,
    -568,  -568,  -568,   216,  1220,  1173,  -568,  -568,  -568,  -568,
    -568,   216,   216,  1223,  -568,   847,  -568,  1420,  -568,  1420,
    1204,  -568,  -568,  1233,  1768,  -568,  1078,  -568,  1238,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  1246,  1250,
    1078,  1251,  -568,  1252,  -568,   216,    44,  -568,  -568,  1004,
    1200,  1004,  -568,  -568,  1035,  1035,  1263,  -568,  1078,  -568,
    -568,  -568,  1265,  1078,  1078,  1268,  1078,  -568,  -568,  -568,
    -568,  1078,  -568,  1271,  -568,   513,  -568,  -568,  1223,  -568,
    -568,  -568,  -568,  1272,  -568,  -568,  -568,  -568,  -568,  1273,
    1078,  -568,  -568,  -568,   326,  1279,  -568,  1280,  1283,  1284,
    -568,  -568,  1225,  -568,  -568,  -568,  -568,  1286,  -568,  -568
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   262,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   222,   258,    16,   184,     0,     0,   222,
       0,   283,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   127,    38,    18,     0,     0,     0,   225,   183,     0,
     127,    19,     0,     0,   225,   260,   259,     0,     0,   262,
       0,     0,     0,   262,     0,     0,     0,   302,   298,   262,
     293,   282,     0,    69,   299,    68,   263,   266,   279,   280,
     281,   270,     0,   301,   269,     0,   271,   272,   273,   274,
     275,   276,   277,   278,     0,   127,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   399,   402,   400,   401,
     337,     0,   304,   404,   398,   397,     0,   366,   375,   382,
     395,   107,   245,   126,   245,     0,     0,     0,     0,    51,
      54,    52,    53,   129,   109,   173,   139,    76,    75,   132,
       0,     0,    80,     0,   134,    64,     0,   222,     0,     0,
     257,     0,     0,   222,   194,     0,   283,   341,   342,     0,
       0,     0,     0,   365,     0,     0,     0,     0,     0,   283,
     283,     0,     0,     0,   294,   295,     0,     0,     0,   336,
       0,     0,   334,     0,     0,     0,     0,   331,     0,     0,
       0,     0,     0,     0,   107,   109,     0,     0,     0,     0,
       0,   222,   222,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   405,   376,   377,   407,   411,     0,
     409,     0,   406,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    88,
       0,    50,   135,   136,    81,    83,   256,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
       0,     0,     0,     0,   178,   162,     0,     0,     0,    96,
       0,    93,     0,     0,     0,     0,     0,     0,     0,   222,
     225,     0,     0,     0,     0,   224,   258,     0,   227,     0,
     241,   185,     0,     0,   222,   225,    12,     0,   261,     0,
       0,     0,   264,   264,     0,   345,   340,   264,   303,   298,
     267,   268,   308,     0,   318,   397,   319,     0,    78,   262,
     262,     0,     0,   307,     0,   335,   328,   332,     0,   339,
     310,     0,   323,   300,     0,   324,   325,   326,   327,     0,
      61,    60,     0,    57,     0,   222,   222,   222,   222,   225,
     225,     0,     0,    24,     0,    31,   222,    26,   222,    25,
       0,     0,   408,   403,   330,   367,   368,   369,   370,   371,
     372,   373,   374,   380,   378,   379,   381,     0,   385,     0,
     386,     0,   387,     0,   383,     0,   384,   393,   394,   396,
       0,    72,     0,    70,    73,     0,     0,     0,     0,     0,
     244,   258,     0,   247,     0,     0,     0,     0,     0,   128,
       0,   131,     0,     0,     0,    50,   156,     0,   146,   151,
       0,    82,     0,     0,   100,     0,   111,     0,     0,     0,
       0,   110,     0,   222,   222,     0,   170,     0,   222,   222,
      98,   173,     0,   160,   163,     0,     0,    42,   127,     0,
     127,     0,   173,   137,   138,   133,   143,   144,   142,   145,
     225,     0,     0,   245,     0,   245,     0,     0,   223,     0,
     190,     0,     0,   225,     0,     0,   104,   106,     0,     0,
       0,   359,   358,   283,   363,   364,   353,   343,     0,   305,
       0,     0,     0,    67,     0,     0,     0,   288,     0,   296,
       0,     0,   306,     0,     0,     0,   333,   309,     0,   329,
     127,     0,     0,   225,   225,   225,   225,     0,     0,   222,
     222,    23,   262,   262,    33,   225,   225,   412,   410,   390,
     391,   392,   388,   389,   108,     0,   107,   102,     0,   245,
       0,   245,     0,     0,   243,     0,    89,    90,   140,   254,
      77,   147,   148,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,   141,   109,   222,   222,   112,     0,     0,
     222,   222,   121,   127,   225,   225,   173,   222,   222,   225,
     225,   161,     0,   127,   337,     0,     0,    42,    41,    87,
       0,    97,    95,     0,     0,   241,     0,   228,     0,     0,
       0,   226,     0,   236,   240,   231,     0,     0,    12,   186,
     187,     0,     0,   195,     0,   241,     0,     0,   264,   265,
     356,   347,   351,     0,   355,   264,   317,   339,   320,    79,
       0,   284,   290,   285,   262,   287,   297,    74,   316,     0,
     314,     0,   338,   312,     0,     0,   109,   127,     0,     0,
       0,     0,   241,   241,   225,   225,    29,    30,   262,     0,
       0,     0,    71,   109,     0,   252,     0,     0,     0,   246,
     248,     0,   159,   157,     0,   149,   150,   153,   154,   152,
     155,     0,   225,   225,   222,   222,   225,   225,     0,     0,
       0,   165,     0,     0,   225,   225,   167,   168,   175,     0,
     164,     0,     0,     0,    44,     0,     0,    94,   241,   204,
     232,   230,     0,   235,     0,     0,     0,   188,   242,     0,
       0,     0,   105,   192,     0,     0,   362,     0,   353,   262,
     262,     0,     0,   264,   344,     0,     0,   264,     0,   289,
       0,   315,   313,   311,    59,    62,     0,   241,     0,     0,
     241,     0,   208,   210,     0,     0,    32,     0,   241,     0,
       0,   250,   249,     0,   251,    91,   158,   101,     0,     0,
     225,   225,     0,     0,     0,     0,   113,     0,    99,     0,
     172,   182,     0,    48,    46,     0,    45,     0,     0,    84,
     200,    12,     0,   229,   239,   238,   237,   233,   191,     0,
     241,     0,    12,   264,   264,   357,   350,   349,   348,   346,
     354,   352,     0,   321,   292,     0,   286,    58,   212,     0,
       0,   218,     0,    12,    12,   241,     0,    20,    27,     0,
     103,   253,   114,     0,     0,     0,   116,   117,   122,   123,
     166,     0,     0,   177,   180,     0,   175,     0,    43,     0,
       0,    85,    12,     0,   234,   241,   206,   189,     0,   361,
     360,   322,   264,    12,   241,   241,    12,   241,     0,     0,
     196,     0,   241,     0,   118,     0,     0,   171,   169,   182,
       0,   182,    47,    49,   127,   127,     0,   205,   202,    12,
     193,   291,     0,   220,   216,     0,   214,   209,   211,    12,
     241,    28,   115,     0,   120,     0,   179,   174,   176,    92,
      86,   201,    12,     0,   213,    12,    12,   219,    12,     0,
     198,   119,   125,   124,   173,     0,   207,     0,     0,     0,
     197,    12,     0,   203,   221,   217,   215,     0,   181,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -568,  -568,  -568,  -568,  -568,   -65,  1050,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   991,   -30,  -339,  -568,  -568,   -58,
    1040,   -29,  1046,    32,   -70,   -39,  -273,  -567,   -64,   304,
    -255,   -78,  -568,  -568,  -568,  -432,   715,  -257,  -568,  -568,
    -568,  -568,  -208,  -568,   618,  -296,  -283,  -227,  -494,  -568,
    -289,  -568,  -568,   413,  -568,   379,   374,  1066,   -31,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   638,   -25,   575,   798,
    -525,    88,  -143,  -568,   721,  -568,   859,   276,  -568,   -66,
    -568,  -568,  -321,  -568,  -185,   589,  -568,  -568,   644,  -568,
     -34,  -568,  -568,   -22,  -198,   780,  -568,   -10,  1100,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   550,  -521,   553,  -568,
    -568,  -568,  -568,  -568,   245,   901,   -91,  -168,  -103,  -568,
     923
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   574,   709,    37,    39,    40,   496,   835,   636,   466,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   901,   935,   502,   528,   287,   305,   481,   739,
     926,   181,   182,   183,   184,   670,   468,   469,   313,   314,
     315,   633,   974,   831,   493,   893,   894,    45,    46,    28,
     658,   852,   346,   949,   981,   902,   962,   841,   939,   873,
     874,   913,   968,   966,   916,   965,    77,   189,   337,   338,
     655,   520,   289,   452,   453,   297,   298,   316,    29,    50,
     196,    51,   532,   533,   106,   107,   108,   546,   547,   109,
     153,   111,   112,   154,   363,   364,   114,   155,   384,   116,
     117,   118,   119,   536,   781,   671,   672,   895,   674,   120,
     121,   122,   123,   204,   366,   157,   158,   159,   160,   259,
     260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,   521,   368,   467,   470,   178,   643,    72,
     177,   348,   104,   176,    78,   673,   178,   110,   712,   374,
     486,   290,   176,   200,   597,   127,   381,   206,   505,   113,
     445,   472,   534,   213,    11,   517,   537,   254,   517,   503,
     504,   115,   262,   498,   460,   299,   652,   236,   211,   954,
     255,   256,   294,    85,   331,   177,   341,    52,   844,   195,
      79,   178,    80,   656,   851,   391,   329,   176,    87,   202,
     720,   102,   332,   800,    79,    53,   593,   499,   333,   753,
     212,   203,   334,     8,    88,    89,    90,   390,   303,    91,
      92,   368,   194,   212,    68,   518,   888,   177,   506,   507,
     508,   509,    93,   178,   342,    94,   185,   176,   176,   428,
     430,   432,   434,   436,   437,   438,   653,   551,    33,   561,
     955,   760,   336,   761,   392,   763,   845,   343,   801,    95,
     657,   657,   743,   369,   370,   102,   594,    96,     1,   754,
      97,    98,   177,   617,   285,     2,   286,    69,   178,    99,
      73,   188,   100,   101,   176,   473,   635,   104,    81,   474,
     102,   787,   110,   748,   368,   475,    10,    56,   603,   353,
     104,   104,    34,   550,   113,   110,   110,    57,   571,   601,
     602,   439,   423,   424,   425,   426,   115,   113,   113,   811,
      85,   812,   303,   814,   542,   749,  -258,    12,  -258,   115,
     115,   365,   631,    74,   788,   476,   441,   365,   264,   477,
     354,    79,   491,    75,   365,    86,   177,    58,   478,   479,
     480,    13,   178,    35,    76,   102,   306,    36,   304,   177,
     771,    74,   492,   543,   177,   178,   307,   299,   299,    38,
     178,   176,   331,    14,    15,   265,    16,    43,   664,   765,
     572,   266,   267,   268,   269,   270,   271,   673,   889,   579,
     332,   580,    17,   581,   308,   582,   333,   583,    18,    44,
     334,    19,    20,   573,   344,    22,   309,   177,    85,   397,
     310,   525,    79,   178,   272,   212,   212,    85,    85,   311,
     312,   640,   526,   642,    47,    48,   351,   751,   627,   335,
      65,   211,   177,   545,   548,   511,   682,    64,   178,   724,
     336,   102,   356,   681,   176,   156,    23,   398,   318,   389,
     524,    47,   177,   725,   726,    24,    25,    79,   178,    26,
     843,   683,   202,   212,   679,   201,   628,   205,   849,   207,
     208,   319,   684,   320,   535,   214,   321,   776,   669,    61,
      49,   186,   306,    85,   784,   689,   691,   731,   192,    62,
     694,   365,   307,    79,    59,   494,   869,   870,   778,   872,
     647,    85,   649,    72,   567,   568,    78,   879,   685,   779,
     127,   881,   727,   728,   729,   730,   177,    60,   258,   261,
     446,   618,   178,   446,    82,   890,   866,   322,   323,   836,
     233,   619,   309,   138,   780,    83,   310,   139,   447,   324,
     837,   447,   833,   834,   448,   311,   312,   448,   449,   273,
     639,   449,   641,   606,   607,   562,   274,   275,   177,   177,
     177,   177,   608,    79,   178,   178,   178,   178,    79,   731,
     176,   176,   176,   176,   921,   135,   715,   450,   717,   177,
     276,   923,   606,   607,    63,   178,   810,    66,   451,   927,
     928,   451,   861,   613,   339,   376,   864,   218,   379,   219,
     382,   295,   177,   379,   385,   386,   387,   388,   178,   197,
     982,   855,   695,   198,   176,   644,   606,   607,   606,   607,
     177,   325,   326,   953,   104,    67,   178,   722,   661,   110,
     777,   569,   176,   327,   609,   610,   706,   707,   349,    79,
      70,   113,   350,   394,   570,   177,   611,    71,    95,   401,
     402,   178,    79,   115,   328,   177,    47,    34,   365,    97,
     150,   178,   909,   910,   600,   606,   607,   612,   698,   699,
     700,   701,   542,   365,   365,   740,   816,   838,   365,   102,
     710,   711,    30,   177,   458,   750,   126,    31,   932,   178,
     933,   459,   497,   177,   454,   176,   687,   473,  -304,   178,
    -304,   587,   538,   217,   542,   176,   177,   475,   177,   539,
     177,   482,   178,   124,   178,   972,   178,   497,   125,   973,
     444,   941,   177,   769,   529,   530,   531,   128,   178,   741,
     742,   411,   220,   306,   746,   747,   412,   512,   444,   514,
     473,   457,   516,   307,   817,   538,   302,   476,   790,   796,
     475,   477,   552,   221,   129,   222,   223,   177,   224,   538,
     478,   479,   480,   178,   585,  -258,   557,  -258,   225,   176,
     585,   586,   806,   285,   177,   286,   177,   614,   177,   130,
     178,   527,   178,   309,   178,   577,   258,   310,   604,   226,
     476,   227,   228,   605,   477,   538,   311,   312,   229,   230,
     231,   232,   686,   478,   479,   480,  -302,   585,  -302,   804,
     805,   538,   538,    47,   696,   177,   538,    84,   791,   792,
     555,   178,   177,   793,   897,   556,   444,   176,   178,   131,
     177,   898,   132,   277,   278,   279,   178,   818,   819,   606,
     607,   822,   823,   857,   858,   280,   281,   133,   555,   829,
     830,   482,   588,   559,   590,   755,   134,   592,   177,   177,
     756,   177,   187,   759,   178,   178,   282,   178,   473,   177,
     637,   283,   880,   177,   355,   178,   352,   188,   475,   178,
      79,   190,   519,   773,  -303,   177,  -303,   177,   265,   209,
     527,   178,   193,   178,   266,   267,   268,   269,   270,   271,
     221,   265,   222,   223,   199,   224,   903,   266,   267,   268,
     269,   270,   271,   292,   293,   677,   678,   908,   476,   215,
     802,   803,   477,   339,   216,   884,   885,   272,   360,   361,
     692,   478,   479,   480,   237,    79,   177,   595,   918,   919,
     272,   238,   178,   177,   169,   170,   171,   172,   291,   178,
     239,   177,   177,   654,   357,   330,   240,   178,   178,   241,
     242,   345,   263,   265,   227,   228,    79,   936,   623,   266,
     267,   268,   269,   270,   271,   243,   840,   263,   942,   227,
     540,   945,    79,   244,   646,   177,   959,   960,    79,   217,
     648,   178,   245,   284,   177,   177,    79,   340,   650,   454,
     178,   178,   272,    79,   963,   714,   176,   176,   288,   399,
     400,    79,   296,   716,   969,   868,   347,   317,   871,   444,
     372,    79,    87,   718,   371,   375,   878,   975,   377,   654,
     977,   978,   782,   979,   783,   782,   440,   930,    88,    89,
      90,   774,   775,    91,    92,   383,   987,   393,   455,   395,
     500,   165,   166,   456,   464,  -337,    93,   396,   406,    94,
     408,   168,   169,   170,   171,   172,   465,   414,   906,   169,
     170,   171,   172,   173,   461,   471,  -337,   210,  -337,  -337,
     654,   211,   654,    95,   654,  -337,  -337,  -337,  -337,   483,
     484,   485,   102,   920,    97,   359,   527,   510,  -337,   487,
     488,   489,   490,    99,   501,   513,   100,   101,   515,   522,
     544,   553,   523,   212,   102,   554,   560,    14,    15,  -337,
      16,  -337,  -337,   938,   211,   558,   497,   497,  -337,  -337,
    -337,  -337,   943,   944,   584,   946,    17,   482,   589,   626,
     951,   847,    18,   591,   339,    19,    20,    21,   654,    22,
     654,   666,   654,   596,   598,   667,   212,   615,   616,   620,
     862,   863,   621,   563,   564,   565,   566,   622,   970,   632,
     675,   638,   645,   659,   575,   265,   576,   161,   660,   662,
     663,   266,   267,   268,   269,   270,   271,   665,   462,   463,
      23,   162,   163,   680,   697,   702,   527,   703,   464,    24,
      25,   164,   708,    26,   527,   285,   169,   170,   171,   172,
     465,   721,   723,   734,   272,   735,   482,   904,   738,   165,
     166,   752,   758,   757,   762,   764,   167,   766,   767,   168,
     768,   770,   527,   527,   772,   527,   794,   169,   170,   171,
     172,   173,   795,   527,   668,   174,   798,   527,   175,   797,
      85,   624,   625,   808,   799,   809,   629,   630,   853,   444,
     102,   527,   807,   813,   815,   825,   826,   827,   828,   265,
     839,   234,   497,   842,   497,   266,   267,   268,   269,   270,
     271,   848,   832,   265,   657,   162,   163,   846,   850,   266,
     267,   268,   269,   270,   271,   164,   415,   416,   417,   418,
     419,   420,   421,   422,   859,   865,   867,   876,   272,   875,
     527,   877,   882,   165,   166,   854,   883,   527,   886,   887,
     167,   896,   272,   168,   891,   527,   527,   892,   899,   900,
     905,   169,   170,   171,   172,   173,   907,   704,   705,   235,
     265,   912,   175,   914,   915,   140,   266,   267,   268,   269,
     270,   271,   917,   922,   102,   924,   141,   142,   929,   527,
    -140,  -140,   934,   925,   143,   257,   144,   403,   937,   145,
    -140,  -140,  -140,   940,   146,   147,   148,   149,   150,   272,
     140,   947,  -140,   732,   733,   948,   950,   952,   736,   737,
     151,   141,   142,  -140,   957,   744,   745,   102,   961,   143,
     964,   144,   362,   967,   145,   140,   971,   976,   980,   146,
     147,   148,   149,   150,   983,   984,   141,   142,   985,   986,
     988,   989,   409,   404,   143,   151,   144,   373,   405,   145,
     140,   713,   102,   956,   146,   147,   148,   149,   150,   931,
     958,   141,   142,   824,   719,   651,   407,   599,   676,   143,
     151,   144,   380,   378,   145,   140,   789,   102,   856,   146,
     147,   148,   149,   150,   578,   860,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   549,     0,   145,
       0,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
     151,     0,   820,   821,   141,   142,     0,   102,     0,     0,
       0,     0,   143,     0,   144,   688,     0,   145,   140,     0,
       0,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     690,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   693,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   495,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   495,
       0,     0,   145,     0,     0,   102,     0,   146,   147,   148,
     149,   634,     0,     0,   -50,   -50,     0,     0,   299,   140,
       0,     0,     0,   151,   -50,   -50,   -50,     0,     0,   300,
     102,   427,   301,     0,   302,     0,   -50,     0,   143,     0,
     144,     0,     0,   145,   140,     0,     0,   -50,   146,   147,
     148,   149,   150,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   212,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   435,     0,     0,     0,     0,   140,     0,   143,   151,
     144,     0,     0,   145,     0,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   143,     0,   144,     0,     0,
     145,     0,     0,     0,   151,   146,   147,   148,   149,   150,
       0,   102,     0,     0,     0,     0,   265,     0,     0,     0,
       0,   151,   266,   267,   268,   269,   270,   271,   102,     0,
       0,     0,     0,     0,   265,     0,     0,     0,     0,   358,
     266,   267,   268,   269,   270,   271,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   272,     0,     0,   410,   266,
     267,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,   265,     0,   272,     0,     0,   413,   266,   267,   268,
     269,   270,   271,     0,     0,     0,     0,   265,     0,   541,
       0,     0,   272,   266,   267,   268,   269,   270,   271,     0,
       0,     0,     0,     0,     0,     0,   265,     0,     0,     0,
     272,   785,   266,   267,   268,   269,   270,   271,     0,     0,
       0,   265,     0,     0,   786,     0,   272,   266,   267,   268,
     269,   270,   271,  -330,     0,     0,     0,     0,     0,     0,
     265,     0,   911,     0,     0,   272,   266,   267,   268,   269,
     270,   271,     0,   246,  -330,   247,  -330,  -330,   248,   558,
     272,     0,     0,  -330,  -330,  -330,  -330,     0,     0,     0,
       0,   249,     0,     0,     0,     0,     0,   250,     0,   272,
       0,   251,     0,     0,   252
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   342,   212,   301,   302,    71,   502,    39,
      80,   196,    51,    71,    45,   536,    80,    51,   585,   217,
     309,   164,    80,    89,   456,    54,   224,    93,   324,    51,
     287,   304,   353,    99,     2,     5,   357,   140,     5,   322,
     323,    51,   145,    28,   299,    48,     6,   125,    64,     5,
     141,   142,    76,     5,     8,   125,     5,    76,     8,    84,
      58,   125,    60,     9,     9,     5,    50,   125,     3,    91,
     595,    95,    26,     5,    58,    94,     5,    62,    32,     5,
      96,    91,    36,    76,    19,    20,    21,     5,    28,    24,
      25,   299,     5,    96,     5,    65,    63,   167,   325,   326,
     327,   328,    37,   167,    53,    40,    74,   165,   166,   277,
     278,   279,   280,   281,   282,   283,    76,   372,     5,   392,
      76,   646,    76,   648,    64,   650,    76,    76,    60,    64,
      76,    76,   626,    85,    86,    95,    65,    72,     4,    65,
      75,    76,   212,     5,    62,    11,    64,    58,   212,    84,
       5,    64,    87,    88,   212,    26,   495,   196,     5,    30,
      95,    17,   196,    28,   372,    36,    76,    26,   464,    17,
     209,   210,    59,   371,   196,   209,   210,    36,     5,   462,
     463,   284,   273,   274,   275,   276,   196,   209,   210,   714,
       5,   716,    28,   718,    58,    60,    58,     0,    60,   209,
     210,   211,   491,    58,    60,    76,    76,   217,     5,    80,
      58,    58,     5,    48,   224,    30,   286,    76,    89,    90,
      91,    59,   286,    72,    59,    95,    26,    76,    64,   299,
     662,    58,    25,    97,   304,   299,    36,    48,    48,    76,
     304,   299,     8,     7,     8,    42,    10,    76,    59,    59,
      14,    48,    49,    50,    51,    52,    53,   778,   825,   427,
      26,   429,    26,   431,    64,   433,    32,   435,    32,    76,
      36,    35,    36,    37,    50,    39,    76,   347,     5,    38,
      80,   346,    58,   347,    81,    96,    96,     5,     5,    89,
      90,   499,    76,   501,    18,    19,    23,    60,    38,    65,
       5,    64,   372,   369,   370,   330,     5,    31,   372,   605,
      76,    95,    30,    30,   372,    70,    80,    76,     6,    50,
     345,    45,   392,   606,   607,    89,    90,    58,   392,    93,
     762,    30,   354,    96,   542,    90,    76,    92,   770,    94,
      95,    29,    41,    31,   354,   100,    34,   668,   533,    26,
      76,    75,    26,     5,   675,   553,   554,   614,    82,    36,
     558,   371,    36,    58,    76,    60,   798,   799,     5,   801,
     513,     5,   515,   403,   399,   400,   407,   809,    30,    16,
     409,   813,   609,   610,   611,   612,   456,    76,   143,   144,
       8,    26,   456,     8,    48,   827,    30,    54,    55,    65,
     124,    36,    76,    72,    41,    59,    80,    76,    26,    66,
      76,    26,   751,   752,    32,    89,    90,    32,    36,    47,
     498,    36,   500,    54,    55,    50,    54,    55,   498,   499,
     500,   501,    63,    58,   498,   499,   500,   501,    58,   696,
     498,   499,   500,   501,   876,    65,   589,    65,   591,   519,
      78,   883,    54,    55,    76,   519,   713,    12,    76,   891,
     892,    76,   783,    65,   188,   220,   787,    59,   223,    61,
     225,   167,   542,   228,   229,   230,   231,   232,   542,    72,
     974,   777,   560,    76,   542,   510,    54,    55,    54,    55,
     560,    44,    45,   925,   533,    76,   560,    65,   523,   533,
      66,    50,   560,    56,    44,    45,   572,   573,    38,    58,
      53,   533,    42,   237,    50,   585,    56,    60,    64,   243,
     244,   585,    58,   533,    77,   595,   250,    59,   538,    75,
      76,   595,   853,   854,    50,    54,    55,    77,   563,   564,
     565,   566,    58,   553,   554,   623,    65,   755,   558,    95,
     575,   576,    59,   623,    58,   633,    76,    64,   897,   623,
     899,    65,   317,   633,   288,   623,    50,    26,    59,   633,
      61,    30,    58,    64,    58,   633,   646,    36,   648,    65,
     650,   305,   646,    48,   648,    72,   650,   342,    53,    76,
     286,   912,   662,   658,   349,   350,   351,    76,   662,   624,
     625,    58,    38,    26,   629,   630,    63,   331,   304,   333,
      26,    59,   336,    36,    30,    58,    64,    76,   684,   697,
      36,    80,    65,    59,    76,    61,    62,   697,    64,    58,
      89,    90,    91,   697,    58,    58,    65,    60,    38,   697,
      58,    65,   708,    62,   714,    64,   716,    65,   718,    76,
     714,   347,   716,    76,   718,   410,   411,    80,    59,    59,
      76,    61,    62,    64,    80,    58,    89,    90,    68,    69,
      70,    71,    65,    89,    90,    91,    59,    58,    61,   704,
     705,    58,    58,   407,    65,   755,    58,    49,    65,    65,
      58,   755,   762,    65,    58,    63,   392,   755,   762,    59,
     770,    65,    59,    44,    45,    46,   770,   732,   733,    54,
      55,   736,   737,   779,   780,    56,    57,    76,    58,   744,
     745,   445,   446,    63,   448,    58,    76,   451,   798,   799,
      63,   801,    76,   645,   798,   799,    77,   801,    26,   809,
     495,    82,    30,   813,    28,   809,    17,    64,    36,   813,
      58,    76,    60,   665,    59,   825,    61,   827,    42,    60,
     456,   825,    76,   827,    48,    49,    50,    51,    52,    53,
      59,    42,    61,    62,    76,    64,   841,    48,    49,    50,
      51,    52,    53,   165,   166,   540,   541,   852,    76,    76,
     702,   703,    80,   517,    76,   820,   821,    81,   209,   210,
     555,    89,    90,    91,    48,    58,   876,    60,   873,   874,
      81,    59,   876,   883,    72,    73,    74,    75,    76,   883,
      59,   891,   892,   519,    33,   187,    59,   891,   892,    76,
      76,   193,    59,    42,    61,    62,    58,   902,    60,    48,
      49,    50,    51,    52,    53,    48,   758,    59,   913,    61,
      62,   916,    58,    48,    60,   925,   934,   935,    58,    64,
      60,   925,    59,    79,   934,   935,    58,     5,    60,   593,
     934,   935,    81,    58,   939,    60,   934,   935,    64,   241,
     242,    58,    76,    60,   949,   797,    60,    53,   800,   585,
      48,    58,     3,    60,    64,    76,   808,   962,    76,   595,
     965,   966,    58,   968,    60,    58,    75,    60,    19,    20,
      21,   666,   667,    24,    25,    76,   981,     5,    28,    76,
      28,    54,    55,    60,    64,    38,    37,    76,    76,    40,
      76,    64,    72,    73,    74,    75,    76,    76,   850,    72,
      73,    74,    75,    76,    76,    76,    59,    60,    61,    62,
     646,    64,   648,    64,   650,    68,    69,    70,    71,    76,
      76,    76,    95,   875,    75,    76,   662,   329,    38,    76,
      76,    76,    30,    84,    28,    76,    87,    88,    76,    76,
      92,    64,   344,    96,    95,    64,    53,     7,     8,    59,
      10,    61,    62,   905,    64,    64,   751,   752,    68,    69,
      70,    71,   914,   915,    63,   917,    26,   731,    76,    65,
     922,   766,    32,    76,   738,    35,    36,    37,   714,    39,
     716,    18,   718,    76,    76,    22,    96,    76,    76,    76,
     785,   786,    76,   395,   396,   397,   398,    76,   950,    76,
      41,     5,     5,     5,   406,    42,   408,    12,     5,    60,
       5,    48,    49,    50,    51,    52,    53,     5,    54,    55,
      80,    26,    27,    76,    53,     5,   762,     5,    64,    89,
      90,    36,    15,    93,   770,    62,    72,    73,    74,    75,
      76,    28,    76,    76,    81,    76,   810,   842,    62,    54,
      55,    58,     5,    30,    60,    28,    61,    53,     5,    64,
       5,    60,   798,   799,    76,   801,     5,    72,    73,    74,
      75,    76,     5,   809,    17,    80,    60,   813,    83,     5,
       5,   483,   484,     5,    60,    60,   488,   489,    17,   825,
      95,   827,    30,    60,    76,    60,     5,    60,    30,    42,
      28,    12,   897,    53,   899,    48,    49,    50,    51,    52,
      53,     5,    76,    42,    76,    26,    27,    76,     5,    48,
      49,    50,    51,    52,    53,    36,   265,   266,   267,   268,
     269,   270,   271,   272,    30,    76,     5,    60,    81,     5,
     876,    59,     5,    54,    55,    17,    60,   883,     5,     5,
      61,    28,    81,    64,    60,   891,   892,    60,    60,    63,
       5,    72,    73,    74,    75,    76,     5,   569,   570,    80,
      42,    17,    83,     5,     5,    43,    48,    49,    50,    51,
      52,    53,     5,     5,    95,     5,    54,    55,     5,   925,
      44,    45,    28,    60,    62,    63,    64,   246,     5,    67,
      54,    55,    56,     5,    72,    73,    74,    75,    76,    81,
      43,     5,    66,   615,   616,     5,     5,     5,   620,   621,
      88,    54,    55,    77,    64,   627,   628,    95,     5,    62,
       5,    64,    65,     5,    67,    43,     5,     5,     5,    72,
      73,    74,    75,    76,     5,     5,    54,    55,     5,     5,
      65,     5,   252,   247,    62,    88,    64,    65,   248,    67,
      43,   586,    95,   929,    72,    73,    74,    75,    76,   896,
     931,    54,    55,   738,   593,   517,   250,   458,   538,    62,
      88,    64,    65,   223,    67,    43,   682,    95,   778,    72,
      73,    74,    75,    76,   411,   782,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      -1,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,   734,   735,    54,    55,    -1,    95,    -1,    -1,
      -1,    -1,    62,    -1,    64,    65,    -1,    67,    43,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      65,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    65,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      -1,    -1,    67,    -1,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    44,    45,    -1,    -1,    48,    43,
      -1,    -1,    -1,    88,    54,    55,    56,    -1,    -1,    59,
      95,    55,    62,    -1,    64,    -1,    66,    -1,    62,    -1,
      64,    -1,    -1,    67,    43,    -1,    -1,    77,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    96,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    43,    -1,    62,    88,
      64,    -1,    -1,    67,    -1,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    62,    -1,    64,    -1,    -1,
      67,    -1,    -1,    -1,    88,    72,    73,    74,    75,    76,
      -1,    95,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    88,    48,    49,    50,    51,    52,    53,    95,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    65,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    81,    -1,    -1,    66,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,
      -1,    -1,    81,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      81,    66,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    42,    -1,    -1,    60,    -1,    81,    48,    49,    50,
      51,    52,    53,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    63,    -1,    -1,    81,    48,    49,    50,    51,
      52,    53,    -1,     8,    59,    10,    61,    62,    13,    64,
      81,    -1,    -1,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    -1,    -1,    -1,    32,    -1,    81,
      -1,    36,    -1,    -1,    39
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
      65,    76,   181,   182,   185,    28,    60,    59,    58,    65,
     128,    76,    54,    55,    64,    76,   117,   143,   144,   145,
     143,    76,   124,    26,    30,    36,    76,    80,    89,    90,
      91,   136,   185,    76,    76,    76,   148,    76,    76,    76,
      30,     5,    25,   152,    60,    64,   114,   222,    28,    62,
      28,    28,   132,   144,   144,   143,   145,   145,   145,   145,
     174,   175,   185,    76,   185,    76,   185,     5,    65,    60,
     179,   114,    76,   174,   175,   103,    76,   127,   133,   222,
     222,   222,   190,   191,   190,   205,   211,   190,    58,    65,
      62,    60,    58,    97,    92,   187,   195,   196,   187,    65,
     202,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   174,   174,   174,   174,   175,   175,    50,
      50,     5,    14,    37,   109,   174,   174,   222,   228,   225,
     225,   225,   225,   225,    63,    58,    65,    30,   185,    76,
     185,    76,   185,     5,    65,    60,    76,   133,    76,   184,
      50,   144,   144,   143,    59,    64,    54,    55,    63,    44,
      45,    56,    77,    65,    65,    76,    76,     5,    26,    36,
      76,    76,    76,    60,   174,   174,    65,    38,    76,   174,
     174,   148,    76,   149,    76,   114,   116,   222,     5,   129,
     140,   129,   140,   146,   175,     5,    60,   180,    60,   180,
      60,   177,     6,    76,   127,   178,     9,    76,   158,     5,
       5,   175,    60,     5,    59,     5,    18,    22,    17,   192,
     143,   213,   214,   215,   216,    41,   203,   222,   222,   140,
      76,    30,     5,    30,    41,    30,    65,    50,    65,   202,
      65,   202,   222,    65,   202,   129,    65,    53,   175,   175,
     175,   175,     5,     5,   174,   174,   187,   187,    15,   110,
     175,   175,   125,   134,    60,   180,    60,   180,    60,   182,
     178,    28,    65,    76,   143,   144,   144,   145,   145,   145,
     145,   135,   174,   174,    76,    76,   174,   174,    62,   137,
     129,   175,   175,   146,   174,   174,   175,   175,    28,    60,
     129,    60,    58,     5,    65,    58,    63,    30,     5,   179,
     178,   178,    60,   178,    28,    59,    53,     5,     5,   103,
      60,   133,    76,   179,   222,   222,   190,    66,     5,    16,
      41,   212,    58,    60,   190,    66,    60,    17,    60,   196,
     187,    65,    65,    65,     5,     5,   129,     5,    60,    60,
       5,    60,   179,   179,   175,   175,   187,    30,     5,    60,
     135,   178,   178,    60,   178,    76,    65,    30,   175,   175,
     174,   174,   175,   175,   176,    60,     5,    60,    30,   175,
     175,   151,    76,   114,   114,   115,    65,    76,   140,    28,
     179,   165,    53,   133,     8,    76,    76,   222,     5,   133,
       5,     9,   159,    17,    17,   143,   214,   187,   187,    30,
     216,   190,   222,   222,   190,    76,    30,     5,   179,   133,
     133,   179,   133,   167,   168,     5,    60,    59,   179,   133,
      30,   133,     5,    60,   175,   175,     5,     5,    63,   125,
     133,    60,    60,   153,   154,   215,    28,    58,    65,    60,
      63,   130,   163,   103,   222,     5,   179,     5,   103,   190,
     190,    63,    17,   169,     5,     5,   172,     5,   103,   103,
     179,   133,     5,   133,     5,    60,   138,   133,   133,     5,
      60,   151,   114,   114,    28,   131,   103,     5,   179,   166,
       5,   190,   103,   179,   179,   103,   179,     5,     5,   161,
       5,   179,     5,   133,     5,    76,   154,    64,   153,   129,
     129,     5,   164,   103,     5,   173,   171,     5,   170,   103,
     179,     5,    72,    76,   150,   103,     5,   103,   103,   103,
       5,   162,   146,     5,     5,     5,     5,   103,    65,     5
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
     142,   142,   142,   142,   142,   142,   143,   143,   143,   143,
     143,   144,   144,   144,   144,   144,   145,   145,   145,   145,
     146,   147,   147,   149,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   150,   151,   152,   152,   152,   153,
     153,   154,   154,   155,   155,   156,   156,   156,   157,   157,
     158,   157,   159,   157,   160,   157,   161,   157,   162,   157,
     163,   157,   164,   157,   165,   157,   166,   157,   167,   157,
     168,   157,   169,   157,   170,   157,   171,   157,   172,   157,
     173,   157,   174,   175,   175,   175,   176,   176,   177,   177,
     177,   177,   177,   177,   177,   177,   178,   178,   178,   178,
     178,   179,   179,   180,   180,   180,   181,   181,   182,   182,
     182,   182,   182,   182,   183,   183,   184,   185,   185,   186,
     188,   187,   189,   187,   191,   190,   192,   192,   192,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   194,   194,   194,   194,   195,   195,
     195,   196,   196,   197,   197,   198,   198,   198,   199,   199,
     199,   199,   200,   200,   200,   201,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   202,   202,   203,
     203,   203,   203,   204,   204,   204,   204,   204,   204,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   206,   206,
     207,   208,   208,   209,   209,   211,   210,   212,   212,   212,
     213,   213,   214,   214,   215,   215,   216,   216,   217,   218,
     219,   219,   219,   220,   221,   221,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   225,   225,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   227,
     227,   228,   228
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
       3,     4,     3,     3,     3,     3,     1,     2,     2,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     4,     3,
       2,     3,     1,     0,     4,     4,     6,     4,     4,     7,
       2,     7,     5,     0,     0,     0,     7,     5,     0,     3,
       1,     6,     0,     2,     1,     4,     6,     6,     8,    10,
       0,     9,     0,    11,     0,     7,     0,    13,     0,    15,
       0,    12,     0,    14,     0,    11,     0,    13,     0,    11,
       0,    11,     0,    12,     0,    14,     0,    14,     0,    12,
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
#line 2309 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2315 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2321 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2327 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2333 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2339 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2345 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2351 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2357 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2363 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2369 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2375 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2381 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2387 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2393 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2399 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2417 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2423 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2429 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2435 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2441 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2447 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2453 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2477 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2483 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2489 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2495 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2501 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2507 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2525 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2531 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2543 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2549 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2555 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2561 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2567 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2579 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2585 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2591 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2597 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2603 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2609 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2615 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2621 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2639 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2645 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2651 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2657 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2663 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2669 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2681 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2687 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2705 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2711 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2729 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2735 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2741 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2747 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2753 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2777 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2783 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2795 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2801 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2807 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2813 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2837 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2843 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2849 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2855 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2861 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2885 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2891 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2897 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2903 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2909 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2915 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2921 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2927 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2933 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2939 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2945 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2951 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2957 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2963 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2969 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2975 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2981 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2987 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2993 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2999 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3005 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3011 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3017 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3023 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3029 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3035 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3047 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3053 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3059 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3065 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3071 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3077 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3089 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3095 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3101 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3119 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3125 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3131 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT PERIOD IDENT  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3137 "pascal.tab.c"
    break;

  case 141: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 440 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term MUL b_factor  */
#line 441 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IDIV b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3155 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term IMOD b_factor  */
#line 443 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 145: /* lo_term: lo_term SHIFTOP b_factor  */
#line 444 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 146: /* b_expr: b_term  */
#line 447 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3173 "pascal.tab.c"
    break;

  case 147: /* b_expr: PLUS b_term  */
#line 448 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 148: /* b_expr: MINUS b_term  */
#line 449 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr PLUS b_term  */
#line 450 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3191 "pascal.tab.c"
    break;

  case 150: /* b_expr: b_expr MINUS b_term  */
#line 451 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3197 "pascal.tab.c"
    break;

  case 151: /* b_term: b_factor  */
#line 454 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3203 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term MUL b_factor  */
#line 455 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IDIV b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3215 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term IMOD b_factor  */
#line 457 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3221 "pascal.tab.c"
    break;

  case 155: /* b_term: b_term SHIFTOP b_factor  */
#line 458 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3227 "pascal.tab.c"
    break;

  case 156: /* b_factor: scalar_constant  */
#line 461 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3233 "pascal.tab.c"
    break;

  case 157: /* b_factor: IDENT PERIOD IDENT  */
#line 462 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 158: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 463 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3245 "pascal.tab.c"
    break;

  case 159: /* b_factor: LPARENT b_expr RPARENT  */
#line 464 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3251 "pascal.tab.c"
    break;

  case 160: /* record_body: record_field_list record_case_opt  */
#line 467 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 161: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 470 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 162: /* record_field_list: record_field  */
#line 471 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3269 "pascal.tab.c"
    break;

  case 163: /* $@4: %empty  */
#line 474 "pascal.y"
                  { }
#line 3275 "pascal.tab.c"
    break;

  case 164: /* record_field: id_list COLON $@4 type  */
#line 474 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3281 "pascal.tab.c"
    break;

  case 165: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3287 "pascal.tab.c"
    break;

  case 166: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 476 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3293 "pascal.tab.c"
    break;

  case 167: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 477 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3299 "pascal.tab.c"
    break;

  case 168: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 478 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3305 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3311 "pascal.tab.c"
    break;

  case 170: /* record_field: IDENT record_field  */
#line 480 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3317 "pascal.tab.c"
    break;

  case 171: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 481 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3323 "pascal.tab.c"
    break;

  case 172: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 482 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3329 "pascal.tab.c"
    break;

  case 173: /* record_field: %empty  */
#line 483 "pascal.y"
     { (yyval.node) = NULL; }
#line 3335 "pascal.tab.c"
    break;

  case 174: /* case_arm_mark: %empty  */
#line 486 "pascal.y"
    { }
#line 3341 "pascal.tab.c"
    break;

  case 175: /* case_open: %empty  */
#line 489 "pascal.y"
    { }
#line 3347 "pascal.tab.c"
    break;

  case 176: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 492 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3353 "pascal.tab.c"
    break;

  case 177: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 493 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3359 "pascal.tab.c"
    break;

  case 178: /* record_case_opt: %empty  */
#line 494 "pascal.y"
     { (yyval.node) = NULL; }
#line 3365 "pascal.tab.c"
    break;

  case 179: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 497 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3371 "pascal.tab.c"
    break;

  case 180: /* record_case_list: record_case_arm  */
#line 498 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3377 "pascal.tab.c"
    break;

  case 181: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 501 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3383 "pascal.tab.c"
    break;

  case 182: /* record_case_arm: %empty  */
#line 502 "pascal.y"
     { (yyval.node) = NULL; }
#line 3389 "pascal.tab.c"
    break;

  case 183: /* var_decl_list: var_decl_list var_decl  */
#line 505 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3395 "pascal.tab.c"
    break;

  case 184: /* var_decl_list: var_decl  */
#line 506 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3401 "pascal.tab.c"
    break;

  case 185: /* var_decl: id_list COLON type SEMICOLON  */
#line 509 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3407 "pascal.tab.c"
    break;

  case 186: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 510 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3413 "pascal.tab.c"
    break;

  case 187: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 511 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3419 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 514 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3425 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 515 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3431 "pascal.tab.c"
    break;

  case 190: /* $@5: %empty  */
#line 516 "pascal.y"
                                                                               { }
#line 3437 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3443 "pascal.tab.c"
    break;

  case 192: /* $@6: %empty  */
#line 517 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3449 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3455 "pascal.tab.c"
    break;

  case 194: /* $@7: %empty  */
#line 518 "pascal.y"
                                        { }
#line 3461 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 518 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3467 "pascal.tab.c"
    break;

  case 196: /* $@8: %empty  */
#line 519 "pascal.y"
                                                                                                           { }
#line 3473 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3479 "pascal.tab.c"
    break;

  case 198: /* $@9: %empty  */
#line 520 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3485 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3491 "pascal.tab.c"
    break;

  case 200: /* $@10: %empty  */
#line 521 "pascal.y"
                                                                                                 { }
#line 3497 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3503 "pascal.tab.c"
    break;

  case 202: /* $@11: %empty  */
#line 522 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3509 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3515 "pascal.tab.c"
    break;

  case 204: /* $@12: %empty  */
#line 523 "pascal.y"
                                                                                            { }
#line 3521 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 523 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3527 "pascal.tab.c"
    break;

  case 206: /* $@13: %empty  */
#line 524 "pascal.y"
                                                                                                             { }
#line 3533 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 524 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3541 "pascal.tab.c"
    break;

  case 208: /* $@14: %empty  */
#line 527 "pascal.y"
                                                                                              { }
#line 3547 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 527 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3555 "pascal.tab.c"
    break;

  case 210: /* $@15: %empty  */
#line 530 "pascal.y"
                                                                                             { }
#line 3561 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 530 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3569 "pascal.tab.c"
    break;

  case 212: /* $@16: %empty  */
#line 533 "pascal.y"
                                                                                                    { }
#line 3575 "pascal.tab.c"
    break;

  case 213: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 533 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3583 "pascal.tab.c"
    break;

  case 214: /* $@17: %empty  */
#line 536 "pascal.y"
                                                                                                                { }
#line 3589 "pascal.tab.c"
    break;

  case 215: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 536 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3598 "pascal.tab.c"
    break;

  case 216: /* $@18: %empty  */
#line 540 "pascal.y"
                                                                                                                  { }
#line 3604 "pascal.tab.c"
    break;

  case 217: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 540 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3613 "pascal.tab.c"
    break;

  case 218: /* $@19: %empty  */
#line 544 "pascal.y"
                                                                                              { }
#line 3619 "pascal.tab.c"
    break;

  case 219: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 544 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3628 "pascal.tab.c"
    break;

  case 220: /* $@20: %empty  */
#line 548 "pascal.y"
                                                                                                                     { }
#line 3634 "pascal.tab.c"
    break;

  case 221: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 548 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3642 "pascal.tab.c"
    break;

  case 222: /* pv_mark: %empty  */
#line 553 "pascal.y"
    { }
#line 3648 "pascal.tab.c"
    break;

  case 223: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 556 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3654 "pascal.tab.c"
    break;

  case 224: /* parameter_list_opt: LPARENT RPARENT  */
#line 557 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3660 "pascal.tab.c"
    break;

  case 225: /* parameter_list_opt: %empty  */
#line 558 "pascal.y"
     { (yyval.node) = NULL; }
#line 3666 "pascal.tab.c"
    break;

  case 226: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 561 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3672 "pascal.tab.c"
    break;

  case 227: /* parameter_decl_list: parameter_decl  */
#line 562 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3678 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 565 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3684 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 566 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3690 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: VARSY id_list COLON param_type  */
#line 567 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3696 "pascal.tab.c"
    break;

  case 231: /* parameter_decl: id_list COLON param_type  */
#line 568 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3702 "pascal.tab.c"
    break;

  case 232: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 569 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3708 "pascal.tab.c"
    break;

  case 233: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 570 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3714 "pascal.tab.c"
    break;

  case 234: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 571 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3720 "pascal.tab.c"
    break;

  case 235: /* parameter_decl: IDENT id_list COLON param_type  */
#line 572 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3726 "pascal.tab.c"
    break;

  case 236: /* param_type: IDENT  */
#line 575 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3732 "pascal.tab.c"
    break;

  case 237: /* param_type: IDENT PERIOD IDENT  */
#line 576 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3738 "pascal.tab.c"
    break;

  case 238: /* param_type: ARRAYSY OFSY IDENT  */
#line 577 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3744 "pascal.tab.c"
    break;

  case 239: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 578 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3750 "pascal.tab.c"
    break;

  case 240: /* param_type: spec_ref  */
#line 579 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3756 "pascal.tab.c"
    break;

  case 241: /* routine_directives: %empty  */
#line 582 "pascal.y"
    { (yyval.node) = NULL; }
#line 3762 "pascal.tab.c"
    break;

  case 242: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 583 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3775 "pascal.tab.c"
    break;

  case 243: /* pf_params: LPARENT pf_sections RPARENT  */
#line 593 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3781 "pascal.tab.c"
    break;

  case 244: /* pf_params: LPARENT RPARENT  */
#line 594 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3787 "pascal.tab.c"
    break;

  case 245: /* pf_params: %empty  */
#line 595 "pascal.y"
     { (yyval.node) = NULL; }
#line 3793 "pascal.tab.c"
    break;

  case 246: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 598 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3799 "pascal.tab.c"
    break;

  case 247: /* pf_sections: pf_section  */
#line 599 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3805 "pascal.tab.c"
    break;

  case 248: /* pf_section: id_list COLON param_type  */
#line 602 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3811 "pascal.tab.c"
    break;

  case 249: /* pf_section: VARSY id_list COLON param_type  */
#line 603 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3817 "pascal.tab.c"
    break;

  case 250: /* pf_section: CONSTSY id_list COLON param_type  */
#line 604 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3823 "pascal.tab.c"
    break;

  case 251: /* pf_section: IDENT id_list COLON param_type  */
#line 605 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3829 "pascal.tab.c"
    break;

  case 252: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 606 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3835 "pascal.tab.c"
    break;

  case 253: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 607 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3841 "pascal.tab.c"
    break;

  case 254: /* enum_list: enum_list COMMA enum_member  */
#line 610 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3847 "pascal.tab.c"
    break;

  case 255: /* enum_list: enum_member  */
#line 611 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3853 "pascal.tab.c"
    break;

  case 256: /* enum_member: IDENT  */
#line 614 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3859 "pascal.tab.c"
    break;

  case 257: /* id_list: id_list COMMA IDENT  */
#line 617 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3865 "pascal.tab.c"
    break;

  case 258: /* id_list: IDENT  */
#line 618 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3871 "pascal.tab.c"
    break;

  case 259: /* body: BEGINSY statement_list ENDSY  */
#line 621 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3877 "pascal.tab.c"
    break;

  case 260: /* $@21: %empty  */
#line 624 "pascal.y"
                             { pas_stmt_mark(); }
#line 3883 "pascal.tab.c"
    break;

  case 261: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 624 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3889 "pascal.tab.c"
    break;

  case 262: /* $@22: %empty  */
#line 625 "pascal.y"
     { pas_stmt_mark(); }
#line 3895 "pascal.tab.c"
    break;

  case 263: /* statement_list: $@22 statement  */
#line 625 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3901 "pascal.tab.c"
    break;

  case 264: /* $@23: %empty  */
#line 628 "pascal.y"
    { pas_stmt_mark(); }
#line 3907 "pascal.tab.c"
    break;

  case 265: /* body_stmt: $@23 statement  */
#line 628 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3913 "pascal.tab.c"
    break;

  case 266: /* statement: statement_no_label  */
#line 631 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3919 "pascal.tab.c"
    break;

  case 267: /* statement: INTCONST COLON statement_no_label  */
#line 632 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3925 "pascal.tab.c"
    break;

  case 268: /* statement: IDENT COLON statement_no_label  */
#line 633 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3931 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: assignment  */
#line 636 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3937 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: call  */
#line 637 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3943 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: compound_statement  */
#line 638 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3949 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: goto_statement  */
#line 639 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3955 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: if_statement  */
#line 640 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3961 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: case_statement  */
#line 641 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3967 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: while_statement  */
#line 642 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3973 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: repeat_statement  */
#line 643 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3979 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: for_statement  */
#line 644 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3985 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: with_statement  */
#line 645 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3991 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: try_statement  */
#line 646 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3997 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: raise_statement  */
#line 647 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4003 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: inherited_call  */
#line 648 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4009 "pascal.tab.c"
    break;

  case 282: /* statement_no_label: INHERITEDSY  */
#line 649 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4015 "pascal.tab.c"
    break;

  case 283: /* statement_no_label: %empty  */
#line 650 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4021 "pascal.tab.c"
    break;

  case 284: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 653 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4027 "pascal.tab.c"
    break;

  case 285: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 654 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4033 "pascal.tab.c"
    break;

  case 286: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 655 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4039 "pascal.tab.c"
    break;

  case 287: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 656 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4045 "pascal.tab.c"
    break;

  case 288: /* handler_list: handler  */
#line 659 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4051 "pascal.tab.c"
    break;

  case 289: /* handler_list: handler_list SEMICOLON handler  */
#line 660 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4057 "pascal.tab.c"
    break;

  case 290: /* handler_list: handler_list SEMICOLON  */
#line 661 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4063 "pascal.tab.c"
    break;

  case 291: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 664 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4069 "pascal.tab.c"
    break;

  case 292: /* handler: ONSY IDENT DOSY body_stmt  */
#line 665 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4075 "pascal.tab.c"
    break;

  case 293: /* raise_statement: RAISESY  */
#line 668 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4081 "pascal.tab.c"
    break;

  case 294: /* raise_statement: RAISESY expression  */
#line 669 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4087 "pascal.tab.c"
    break;

  case 295: /* inherited_call: INHERITEDSY IDENT  */
#line 672 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4093 "pascal.tab.c"
    break;

  case 296: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 673 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4099 "pascal.tab.c"
    break;

  case 297: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 674 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4105 "pascal.tab.c"
    break;

  case 298: /* call: IDENT  */
#line 677 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4111 "pascal.tab.c"
    break;

  case 299: /* call: spec_expr  */
#line 678 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4117 "pascal.tab.c"
    break;

  case 300: /* call: selector PERIOD IDENT  */
#line 679 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4123 "pascal.tab.c"
    break;

  case 301: /* call: call_with_args  */
#line 680 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4129 "pascal.tab.c"
    break;

  case 302: /* recv_primary: STRINGCONST  */
#line 683 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4135 "pascal.tab.c"
    break;

  case 303: /* recv_primary: LPARENT expression RPARENT  */
#line 684 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4141 "pascal.tab.c"
    break;

  case 304: /* recv_primary: spec_expr  */
#line 685 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4147 "pascal.tab.c"
    break;

  case 305: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 688 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4153 "pascal.tab.c"
    break;

  case 306: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 689 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4159 "pascal.tab.c"
    break;

  case 307: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 690 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4165 "pascal.tab.c"
    break;

  case 308: /* call_with_args: IDENT LPARENT RPARENT  */
#line 691 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4171 "pascal.tab.c"
    break;

  case 309: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 692 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4177 "pascal.tab.c"
    break;

  case 310: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 693 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4183 "pascal.tab.c"
    break;

  case 311: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 694 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4189 "pascal.tab.c"
    break;

  case 312: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 695 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4195 "pascal.tab.c"
    break;

  case 313: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 696 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4201 "pascal.tab.c"
    break;

  case 314: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 697 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4207 "pascal.tab.c"
    break;

  case 315: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 698 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4213 "pascal.tab.c"
    break;

  case 316: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 699 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4219 "pascal.tab.c"
    break;

  case 317: /* argument_list: argument_list COMMA argument  */
#line 702 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4225 "pascal.tab.c"
    break;

  case 318: /* argument_list: argument  */
#line 703 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4231 "pascal.tab.c"
    break;

  case 319: /* argument: expression  */
#line 706 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4237 "pascal.tab.c"
    break;

  case 320: /* argument: expression COLON expression  */
#line 707 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4243 "pascal.tab.c"
    break;

  case 321: /* argument: expression COLON expression COLON expression  */
#line 708 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4249 "pascal.tab.c"
    break;

  case 322: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 709 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4255 "pascal.tab.c"
    break;

  case 323: /* assignment: selector BECOMES expression  */
#line 712 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4261 "pascal.tab.c"
    break;

  case 324: /* assignment: selector ADDBECOMES expression  */
#line 713 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4267 "pascal.tab.c"
    break;

  case 325: /* assignment: selector SUBBECOMES expression  */
#line 714 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4273 "pascal.tab.c"
    break;

  case 326: /* assignment: selector MULBECOMES expression  */
#line 715 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4279 "pascal.tab.c"
    break;

  case 327: /* assignment: selector DIVBECOMES expression  */
#line 716 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4285 "pascal.tab.c"
    break;

  case 328: /* assignment: call_with_args BECOMES expression  */
#line 717 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4291 "pascal.tab.c"
    break;

  case 329: /* selector: selector LBRACK expression_list RBRACK  */
#line 720 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4297 "pascal.tab.c"
    break;

  case 330: /* selector: selector PERIOD IDENT  */
#line 721 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4303 "pascal.tab.c"
    break;

  case 331: /* selector: selector ARROW  */
#line 722 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4309 "pascal.tab.c"
    break;

  case 332: /* selector: call_with_args PERIOD IDENT  */
#line 723 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4315 "pascal.tab.c"
    break;

  case 333: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 724 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4321 "pascal.tab.c"
    break;

  case 334: /* selector: call_with_args ARROW  */
#line 725 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4327 "pascal.tab.c"
    break;

  case 335: /* selector: recv_primary PERIOD IDENT  */
#line 726 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4333 "pascal.tab.c"
    break;

  case 336: /* selector: recv_primary ARROW  */
#line 727 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4339 "pascal.tab.c"
    break;

  case 337: /* selector: IDENT  */
#line 728 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4345 "pascal.tab.c"
    break;

  case 338: /* expression_list: expression_list COMMA expression  */
#line 731 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4351 "pascal.tab.c"
    break;

  case 339: /* expression_list: expression  */
#line 732 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4357 "pascal.tab.c"
    break;

  case 340: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 735 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4363 "pascal.tab.c"
    break;

  case 341: /* goto_statement: GOTOSY INTCONST  */
#line 738 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4369 "pascal.tab.c"
    break;

  case 342: /* goto_statement: GOTOSY IDENT  */
#line 739 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4375 "pascal.tab.c"
    break;

  case 343: /* if_statement: IFSY expression THENSY body_stmt  */
#line 742 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4381 "pascal.tab.c"
    break;

  case 344: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 743 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4387 "pascal.tab.c"
    break;

  case 345: /* $@24: %empty  */
#line 746 "pascal.y"
                           { }
#line 4393 "pascal.tab.c"
    break;

  case 346: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 746 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4399 "pascal.tab.c"
    break;

  case 347: /* case_tail: %empty  */
#line 749 "pascal.y"
    { (yyval.node) = NULL; }
#line 4405 "pascal.tab.c"
    break;

  case 348: /* case_tail: ELSESY statement_list  */
#line 750 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4411 "pascal.tab.c"
    break;

  case 349: /* case_tail: OTHERWISESY statement_list  */
#line 751 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4417 "pascal.tab.c"
    break;

  case 350: /* case_list: case_list SEMICOLON case_elem  */
#line 754 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4423 "pascal.tab.c"
    break;

  case 351: /* case_list: case_elem  */
#line 755 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4429 "pascal.tab.c"
    break;

  case 352: /* case_elem: constant_list COLON body_stmt  */
#line 758 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4435 "pascal.tab.c"
    break;

  case 353: /* case_elem: %empty  */
#line 759 "pascal.y"
     { (yyval.node) = NULL; }
#line 4441 "pascal.tab.c"
    break;

  case 354: /* constant_list: constant_list COMMA case_label  */
#line 762 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4447 "pascal.tab.c"
    break;

  case 355: /* constant_list: case_label  */
#line 763 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4453 "pascal.tab.c"
    break;

  case 356: /* case_label: b_expr  */
#line 766 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4459 "pascal.tab.c"
    break;

  case 357: /* case_label: b_expr DOTDOT b_expr  */
#line 767 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4465 "pascal.tab.c"
    break;

  case 358: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 770 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4471 "pascal.tab.c"
    break;

  case 359: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 773 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4477 "pascal.tab.c"
    break;

  case 360: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 776 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4483 "pascal.tab.c"
    break;

  case 361: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 777 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4489 "pascal.tab.c"
    break;

  case 362: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 778 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4495 "pascal.tab.c"
    break;

  case 363: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 781 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4501 "pascal.tab.c"
    break;

  case 364: /* with_open: with_open COMMA selector  */
#line 784 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4507 "pascal.tab.c"
    break;

  case 365: /* with_open: selector  */
#line 785 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4513 "pascal.tab.c"
    break;

  case 366: /* expression: simple_expression  */
#line 792 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4519 "pascal.tab.c"
    break;

  case 367: /* expression: expression INOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4525 "pascal.tab.c"
    break;

  case 368: /* expression: expression LTOP simple_expression  */
#line 794 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4531 "pascal.tab.c"
    break;

  case 369: /* expression: expression LEOP simple_expression  */
#line 795 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4537 "pascal.tab.c"
    break;

  case 370: /* expression: expression GTOP simple_expression  */
#line 796 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4543 "pascal.tab.c"
    break;

  case 371: /* expression: expression GEOP simple_expression  */
#line 797 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4549 "pascal.tab.c"
    break;

  case 372: /* expression: expression NEOP simple_expression  */
#line 798 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4555 "pascal.tab.c"
    break;

  case 373: /* expression: expression EQOP simple_expression  */
#line 799 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4561 "pascal.tab.c"
    break;

  case 374: /* expression: expression ISOP simple_expression  */
#line 800 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4567 "pascal.tab.c"
    break;

  case 375: /* simple_expression: term  */
#line 803 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4573 "pascal.tab.c"
    break;

  case 376: /* simple_expression: PLUS term  */
#line 804 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4579 "pascal.tab.c"
    break;

  case 377: /* simple_expression: MINUS term  */
#line 805 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4585 "pascal.tab.c"
    break;

  case 378: /* simple_expression: simple_expression PLUS term  */
#line 806 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4591 "pascal.tab.c"
    break;

  case 379: /* simple_expression: simple_expression MINUS term  */
#line 807 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4597 "pascal.tab.c"
    break;

  case 380: /* simple_expression: simple_expression OROP term  */
#line 808 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4603 "pascal.tab.c"
    break;

  case 381: /* simple_expression: simple_expression XOROP term  */
#line 809 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4609 "pascal.tab.c"
    break;

  case 382: /* term: power  */
#line 812 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4615 "pascal.tab.c"
    break;

  case 383: /* term: term MUL power  */
#line 813 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4621 "pascal.tab.c"
    break;

  case 384: /* term: term RDIV power  */
#line 814 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4627 "pascal.tab.c"
    break;

  case 385: /* term: term IDIV power  */
#line 815 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4633 "pascal.tab.c"
    break;

  case 386: /* term: term IMOD power  */
#line 816 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4639 "pascal.tab.c"
    break;

  case 387: /* term: term ANDOP power  */
#line 817 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4645 "pascal.tab.c"
    break;

  case 388: /* term: term MUL MINUS power  */
#line 818 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4651 "pascal.tab.c"
    break;

  case 389: /* term: term RDIV MINUS power  */
#line 819 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4657 "pascal.tab.c"
    break;

  case 390: /* term: term IDIV MINUS power  */
#line 820 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4663 "pascal.tab.c"
    break;

  case 391: /* term: term IMOD MINUS power  */
#line 821 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4669 "pascal.tab.c"
    break;

  case 392: /* term: term ANDOP MINUS power  */
#line 822 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4675 "pascal.tab.c"
    break;

  case 393: /* term: term SHIFTOP power  */
#line 823 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4681 "pascal.tab.c"
    break;

  case 394: /* term: term ASOP power  */
#line 824 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4687 "pascal.tab.c"
    break;

  case 395: /* power: factor  */
#line 827 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4693 "pascal.tab.c"
    break;

  case 396: /* power: power POWOP factor  */
#line 828 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4699 "pascal.tab.c"
    break;

  case 397: /* factor: selector  */
#line 831 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4705 "pascal.tab.c"
    break;

  case 398: /* factor: call_with_args  */
#line 832 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4711 "pascal.tab.c"
    break;

  case 399: /* factor: INTCONST  */
#line 833 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4717 "pascal.tab.c"
    break;

  case 400: /* factor: REALCONST  */
#line 834 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4723 "pascal.tab.c"
    break;

  case 401: /* factor: STRINGCONST  */
#line 835 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4729 "pascal.tab.c"
    break;

  case 402: /* factor: CHARCODE  */
#line 836 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4735 "pascal.tab.c"
    break;

  case 403: /* factor: LPARENT expression RPARENT  */
#line 837 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4741 "pascal.tab.c"
    break;

  case 404: /* factor: inherited_call  */
#line 838 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4747 "pascal.tab.c"
    break;

  case 405: /* factor: NOTSY factor  */
#line 839 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4753 "pascal.tab.c"
    break;

  case 406: /* factor: ATSIGN factor  */
#line 840 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4759 "pascal.tab.c"
    break;

  case 407: /* factor: LBRACK RBRACK  */
#line 841 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4765 "pascal.tab.c"
    break;

  case 408: /* factor: LBRACK set_member_list RBRACK  */
#line 842 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4771 "pascal.tab.c"
    break;

  case 409: /* set_member_list: set_member  */
#line 845 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4777 "pascal.tab.c"
    break;

  case 410: /* set_member_list: set_member_list COMMA set_member  */
#line 846 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4783 "pascal.tab.c"
    break;

  case 411: /* set_member: expression  */
#line 849 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4789 "pascal.tab.c"
    break;

  case 412: /* set_member: expression DOTDOT expression  */
#line 850 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4795 "pascal.tab.c"
    break;


#line 4799 "pascal.tab.c"

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

#line 856 "pascal.y"

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
