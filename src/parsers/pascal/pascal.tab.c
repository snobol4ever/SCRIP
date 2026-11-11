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
  YYSYMBOL_class_members = 134,            /* class_members  */
  YYSYMBOL_class_member = 135,             /* class_member  */
  YYSYMBOL_prop_index_opt = 136,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 137,               /* prop_specs  */
  YYSYMBOL_packed_opt = 138,               /* packed_opt  */
  YYSYMBOL_simple_type = 139,              /* simple_type  */
  YYSYMBOL_lo_bound = 140,                 /* lo_bound  */
  YYSYMBOL_lo_term = 141,                  /* lo_term  */
  YYSYMBOL_b_expr = 142,                   /* b_expr  */
  YYSYMBOL_b_term = 143,                   /* b_term  */
  YYSYMBOL_b_factor = 144,                 /* b_factor  */
  YYSYMBOL_record_body = 145,              /* record_body  */
  YYSYMBOL_record_field_list = 146,        /* record_field_list  */
  YYSYMBOL_record_field = 147,             /* record_field  */
  YYSYMBOL_148_4 = 148,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 149,            /* case_arm_mark  */
  YYSYMBOL_case_open = 150,                /* case_open  */
  YYSYMBOL_record_case_opt = 151,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 152,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 153,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 154,            /* var_decl_list  */
  YYSYMBOL_var_decl = 155,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 156,           /* procedure_decl  */
  YYSYMBOL_157_5 = 157,                    /* $@5  */
  YYSYMBOL_158_6 = 158,                    /* $@6  */
  YYSYMBOL_159_7 = 159,                    /* $@7  */
  YYSYMBOL_160_8 = 160,                    /* $@8  */
  YYSYMBOL_161_9 = 161,                    /* $@9  */
  YYSYMBOL_162_10 = 162,                   /* $@10  */
  YYSYMBOL_163_11 = 163,                   /* $@11  */
  YYSYMBOL_164_12 = 164,                   /* $@12  */
  YYSYMBOL_165_13 = 165,                   /* $@13  */
  YYSYMBOL_166_14 = 166,                   /* $@14  */
  YYSYMBOL_167_15 = 167,                   /* $@15  */
  YYSYMBOL_168_16 = 168,                   /* $@16  */
  YYSYMBOL_169_17 = 169,                   /* $@17  */
  YYSYMBOL_170_18 = 170,                   /* $@18  */
  YYSYMBOL_171_19 = 171,                   /* $@19  */
  YYSYMBOL_pv_mark = 172,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 173,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 174,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 175,           /* parameter_decl  */
  YYSYMBOL_param_type = 176,               /* param_type  */
  YYSYMBOL_routine_directives = 177,       /* routine_directives  */
  YYSYMBOL_pf_params = 178,                /* pf_params  */
  YYSYMBOL_pf_sections = 179,              /* pf_sections  */
  YYSYMBOL_pf_section = 180,               /* pf_section  */
  YYSYMBOL_enum_list = 181,                /* enum_list  */
  YYSYMBOL_enum_member = 182,              /* enum_member  */
  YYSYMBOL_id_list = 183,                  /* id_list  */
  YYSYMBOL_body = 184,                     /* body  */
  YYSYMBOL_statement_list = 185,           /* statement_list  */
  YYSYMBOL_186_20 = 186,                   /* $@20  */
  YYSYMBOL_187_21 = 187,                   /* $@21  */
  YYSYMBOL_body_stmt = 188,                /* body_stmt  */
  YYSYMBOL_189_22 = 189,                   /* $@22  */
  YYSYMBOL_statement = 190,                /* statement  */
  YYSYMBOL_statement_no_label = 191,       /* statement_no_label  */
  YYSYMBOL_try_statement = 192,            /* try_statement  */
  YYSYMBOL_handler_list = 193,             /* handler_list  */
  YYSYMBOL_handler = 194,                  /* handler  */
  YYSYMBOL_raise_statement = 195,          /* raise_statement  */
  YYSYMBOL_inherited_call = 196,           /* inherited_call  */
  YYSYMBOL_call = 197,                     /* call  */
  YYSYMBOL_recv_primary = 198,             /* recv_primary  */
  YYSYMBOL_call_with_args = 199,           /* call_with_args  */
  YYSYMBOL_argument_list = 200,            /* argument_list  */
  YYSYMBOL_argument = 201,                 /* argument  */
  YYSYMBOL_assignment = 202,               /* assignment  */
  YYSYMBOL_selector = 203,                 /* selector  */
  YYSYMBOL_expression_list = 204,          /* expression_list  */
  YYSYMBOL_compound_statement = 205,       /* compound_statement  */
  YYSYMBOL_goto_statement = 206,           /* goto_statement  */
  YYSYMBOL_if_statement = 207,             /* if_statement  */
  YYSYMBOL_case_statement = 208,           /* case_statement  */
  YYSYMBOL_209_23 = 209,                   /* $@23  */
  YYSYMBOL_case_tail = 210,                /* case_tail  */
  YYSYMBOL_case_list = 211,                /* case_list  */
  YYSYMBOL_case_elem = 212,                /* case_elem  */
  YYSYMBOL_constant_list = 213,            /* constant_list  */
  YYSYMBOL_case_label = 214,               /* case_label  */
  YYSYMBOL_while_statement = 215,          /* while_statement  */
  YYSYMBOL_repeat_statement = 216,         /* repeat_statement  */
  YYSYMBOL_for_statement = 217,            /* for_statement  */
  YYSYMBOL_with_statement = 218,           /* with_statement  */
  YYSYMBOL_with_open = 219,                /* with_open  */
  YYSYMBOL_expression = 220,               /* expression  */
  YYSYMBOL_simple_expression = 221,        /* simple_expression  */
  YYSYMBOL_term = 222,                     /* term  */
  YYSYMBOL_power = 223,                    /* power  */
  YYSYMBOL_factor = 224,                   /* factor  */
  YYSYMBOL_set_member_list = 225,          /* set_member_list  */
  YYSYMBOL_set_member = 226                /* set_member  */
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
#define YYLAST   1887

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  129
/* YYNRULES -- Number of rules.  */
#define YYNRULES  401
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  964

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
     322,   323,   326,   327,   330,   331,   334,   337,   338,   341,
     342,   345,   346,   349,   352,   353,   354,   357,   358,   361,
     362,   363,   364,   364,   364,   365,   366,   367,   368,   369,
     370,   371,   371,   372,   373,   374,   375,   376,   377,   378,
     381,   382,   383,   386,   387,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   402,   403,   406,   407,   408,
     411,   412,   415,   416,   417,   418,   419,   420,   423,   424,
     425,   426,   427,   430,   431,   432,   433,   434,   435,   438,
     439,   440,   441,   442,   445,   446,   447,   448,   449,   452,
     453,   454,   457,   460,   461,   464,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   475,   478,   481,   482,   483,
     486,   487,   490,   491,   494,   495,   498,   499,   500,   503,
     504,   505,   505,   506,   506,   507,   507,   508,   508,   509,
     509,   510,   510,   511,   511,   512,   512,   513,   513,   516,
     516,   519,   519,   522,   522,   525,   525,   529,   529,   533,
     533,   538,   541,   542,   543,   546,   547,   550,   551,   552,
     553,   554,   555,   556,   557,   560,   561,   562,   563,   564,
     567,   568,   578,   579,   580,   583,   584,   587,   588,   589,
     590,   591,   592,   595,   596,   599,   602,   603,   606,   609,
     609,   610,   610,   613,   613,   616,   617,   618,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   638,   639,   640,   641,   644,   645,   646,
     649,   650,   653,   654,   657,   658,   659,   662,   663,   664,
     665,   668,   669,   670,   673,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   687,   688,   691,   692,
     693,   694,   697,   698,   699,   700,   701,   702,   705,   706,
     707,   708,   709,   710,   711,   712,   713,   716,   717,   720,
     723,   724,   727,   728,   731,   731,   734,   735,   736,   739,
     740,   743,   744,   747,   748,   751,   752,   755,   758,   761,
     762,   763,   766,   769,   770,   777,   778,   779,   780,   781,
     782,   783,   784,   785,   788,   789,   790,   791,   792,   793,
     794,   797,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   812,   813,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   830,   831,
     834,   835
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
  "result_type", "class_members", "class_member", "prop_index_opt",
  "prop_specs", "packed_opt", "simple_type", "lo_bound", "lo_term",
  "b_expr", "b_term", "b_factor", "record_body", "record_field_list",
  "record_field", "$@4", "case_arm_mark", "case_open", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "$@19",
  "pv_mark", "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "param_type", "routine_directives", "pf_params", "pf_sections",
  "pf_section", "enum_list", "enum_member", "id_list", "body",
  "statement_list", "$@20", "$@21", "body_stmt", "$@22", "statement",
  "statement_no_label", "try_statement", "handler_list", "handler",
  "raise_statement", "inherited_call", "call", "recv_primary",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@23", "case_tail", "case_list",
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

#define YYPACT_NINF (-569)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-327)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     290,    79,   162,   273,  -569,   240,   658,  -569,  -569,   236,
    -569,    68,  -569,  -569,   175,   251,   162,   262,   265,   265,
     286,  -569,   173,    45,   318,   327,   329,  -569,  -569,  -569,
     339,   265,   467,   497,   448,  -569,  -569,    51,   338,   251,
    -569,    98,   475,    26,  -569,   265,  -569,   109,   101,    30,
     317,   894,   472,   482,   173,  -569,   503,   515,   537,   587,
     594,   549,   593,  -569,   465,  -569,  -569,  -569,  -569,   485,
    1455,  1130,  -569,  -569,   162,   265,   603,   590,  -569,   604,
    1130,  -569,   265,   609,    64,  -569,  -569,   495,   612,  -569,
    1455,   314,  1455,  -569,  1455,  1455,   631,  -569,  1159,  -569,
    1455,   616,   622,  -569,   581,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,   374,   267,  -569,   758,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   265,  1190,   652,  -569,   646,   662,
     666,   650,   660,   691,   696,  -569,   684,   542,  -569,  -569,
     794,   794,   794,  1217,  1455,   794,  -569,  -569,  -569,   389,
     159,   616,   685,  -569,   640,   573,   191,   214,   459,   675,
    -569,   701,  -569,   701,   588,   588,   212,   688,  -569,  -569,
    -569,  -569,  1042,   707,   280,  -569,  -569,  -569,  -569,   719,
     422,  -569,    29,   221,   475,   140,  -569,    78,   777,  -569,
      40,   376,  -569,  -569,   733,   894,  -569,  -569,   633,   344,
     333,   640,   573,    81,  1082,   363,  1678,  1690,  1052,  1052,
    1244,   583,    37,  1806,   730,   749,  1283,   722,  -569,  1455,
     734,  -569,  1455,  1318,  1455,   736,  -569,  1455,  1455,  1455,
    1455,  1455,   402,    65,   797,   265,   738,   740,   151,  -569,
    -569,   265,   265,  -569,   251,   162,  -569,   742,   265,   747,
     173,  -569,  -569,   459,   459,  -569,  1712,   517,  -569,  1724,
    -569,   759,  -569,  1455,  1455,  1455,  1455,  1455,  1455,  1455,
    1455,   794,   794,   794,   794,  1530,  1555,  1580,  1605,  1630,
     794,   794,   794,   126,   781,   764,   770,   221,   221,     9,
    -569,  -569,   468,  -569,   583,   762,  1796,  1796,   228,   244,
     763,   766,   775,   432,   776,   779,   784,   827,   207,  -569,
     488,  1480,   164,   836,   837,  -569,   702,   702,  1796,   702,
     702,   702,   702,  -569,   590,   265,   795,   265,   798,  -569,
     265,    54,  -569,   556,   864,  -569,  1480,   803,  -569,   590,
    -569,   269,  -569,  1455,  1455,  1455,  -569,  -569,   314,  -569,
    -569,  -569,  -569,   891,  -569,  -569,  -569,   470,  -569,   647,
    1746,   124,  -569,   788,  -569,  1343,   583,  -569,   480,   817,
    1806,   820,   530,  1806,  -569,   498,  1806,  1653,   547,  1806,
    1806,  1806,  1806,   832,  -569,   228,  -569,   407,  -569,  -569,
    -569,  -569,   590,   590,   428,   431,   251,   128,   250,  -569,
     265,  -569,   173,  1455,  1455,  -569,   611,   822,   214,   214,
     214,   214,   214,   214,   214,   214,   459,   459,   459,   459,
     794,   675,   794,   675,   794,   675,   794,   675,   794,   675,
     675,   675,  -569,   265,   811,   265,   812,  -569,   265,    60,
    -569,   692,   818,   269,   688,  -569,   444,  -569,   702,   702,
    1796,   835,  -569,   358,   343,  -569,   180,     9,   507,  -569,
    -569,   824,  -569,   825,    82,   335,   829,   830,   831,  -569,
     695,  -569,  -569,   843,  -569,   172,  -569,  -569,  -569,    56,
     841,  -569,  -569,  1505,   906,  1806,  1130,   583,  1130,   583,
      56,   343,   343,   408,  -569,  -569,  -569,  -569,   590,   907,
     700,   701,   703,   701,   710,   112,  -569,    35,   915,   845,
     917,   918,   590,   866,   919,   163,  -569,   922,   953,   394,
    1806,  -569,   894,  -569,   573,  1796,   855,  1455,  -569,  1455,
    1455,   583,  -569,   857,   367,    67,  -569,   372,  -569,   508,
     461,  -569,  1380,  1405,  1455,  -569,  -569,  1430,  -569,  1130,
     539,   882,   590,   590,   590,   590,   931,   932,  -569,  -569,
    -569,  -569,  -569,   926,   590,   590,  1806,  -569,   675,   675,
     675,   675,   675,   723,   701,   726,   701,   732,   177,  -569,
      35,  -569,   914,  -569,  -569,   343,   343,   415,  1796,   702,
     702,  -569,   702,   702,   702,   702,  -569,   228,  -569,  -569,
    -569,  -569,   867,   869,  -569,  -569,   884,  1130,   590,   590,
      56,  -569,  -569,   590,   590,  -569,   203,  1130,   156,   889,
      61,  1724,  -569,  -569,   578,  -569,  -569,   927,   944,  -569,
      35,  -569,    35,   896,    35,  -569,   939,   166,  -569,   920,
    -569,   967,  -569,  -569,  -569,   916,   269,  -569,   898,   968,
    1455,  1455,  -569,  -569,   400,   213,  -569,   743,  -569,  -569,
    -569,  1759,  1771,  -569,    47,  -569,   788,  -569,  -569,  -569,
    -569,  -569,  -569,   541,  -569,   544,  1806,  -569,   550,   974,
     975,  1130,   979,   925,   928,   930,  -569,  -569,   590,   590,
     981,   981,  -569,   961,   987,   933,    35,  -569,    35,   937,
      35,  -569,  -569,   923,  -569,   452,   343,   343,  -569,  -569,
    -569,  -569,  -569,   316,   590,   590,  -569,  -569,   590,   590,
     112,   940,  1002,  -569,   952,   984,   590,   590,  -569,  -569,
    -569,   941,  -569,  1480,  1480,    92,  -569,   583,   988,  -569,
    -569,   845,   962,  -569,   269,  -569,    36,   942,  1455,  -569,
    1017,   269,  1018,  -569,  1019,   845,   570,   665,  -569,  1796,
    1796,  -569,  -569,   995,  1796,  -569,  -569,  1455,  1455,  -569,
     950,  -569,   399,  -569,  -569,  -569,  -569,  -569,  1026,  -569,
     269,   269,   269,   845,   845,  1027,   973,   981,   976,  -569,
     269,  -569,  -569,   269,  -569,  -569,  -569,  -569,  1031,   977,
     590,   590,  1033,  1034,    74,   228,  -569,   269,  -569,   980,
     983,  1796,  1013,  -569,  -569,   566,  -569,   985,   989,  -569,
     845,  -569,  1455,  -569,  -569,  -569,  -569,  1806,  -569,  1041,
    -569,  -569,  -569,  -569,  -569,   408,  -569,   981,   981,  -569,
    -569,  -569,  1784,  1806,  -569,  1036,  -569,  -569,   845,  1043,
    1045,  1046,  -569,  -569,  -569,   269,  -569,   845,  1049,  -569,
    -569,   269,  1051,   997,  -569,  -569,  -569,  -569,  -569,   269,
     269,  1053,  -569,   748,  -569,  1480,  -569,  1480,  1032,  -569,
    -569,  1056,  1806,  -569,   845,  1058,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,  1061,  1062,   845,  1063,  -569,  1069,
    -569,   269,    41,  -569,  -569,  1796,  1011,  1796,  -569,  -569,
    1130,  1130,  1076,  -569,   845,  -569,  -569,  -569,  1077,   845,
     845,   845,  -569,  -569,  -569,  -569,   845,  -569,  1078,  -569,
     513,  -569,  -569,  1053,  -569,  -569,  -569,  -569,  1079,  -569,
    -569,  -569,  -569,  1080,   845,  -569,  -569,  -569,    56,  1088,
    -569,  1089,  1090,  1094,  -569,  -569,  1035,  -569,  -569,  -569,
    -569,  1097,  -569,  -569
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   211,   247,    16,   175,     0,     0,   211,
       0,   272,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    65,    13,     0,
       0,   121,    38,    18,     0,     0,     0,   214,   174,     0,
     121,    19,     0,     0,   214,   249,   248,     0,     0,   251,
       0,     0,     0,   251,     0,     0,     0,   291,   287,   251,
     282,   271,     0,    68,   288,    67,   252,   255,   268,   269,
     270,   259,     0,   290,   258,     0,   260,   261,   262,   263,
     264,   265,   266,   267,     0,   121,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   388,   391,   389,   390,
     326,     0,   293,   393,   387,   386,     0,   355,   364,   371,
     384,   234,   120,   234,     0,     0,     0,     0,    51,    54,
      52,    53,   123,   103,   164,   133,    75,    74,   126,     0,
       0,    79,     0,   128,    63,     0,   211,     0,     0,   246,
       0,     0,   211,   185,     0,   272,   330,   331,     0,     0,
       0,     0,   354,     0,     0,     0,     0,     0,   272,   272,
       0,     0,     0,   283,   284,     0,     0,     0,   325,     0,
       0,   323,     0,     0,     0,     0,   320,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,     0,   211,
     211,     0,     0,     4,     0,     0,    12,     0,     0,     0,
       0,    21,   394,   365,   366,   396,   400,     0,   398,     0,
     395,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,     0,    50,   129,   130,    80,
      81,   245,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   164,     0,     0,     0,     0,   169,   154,
       0,     0,     0,    94,     0,    91,     0,     0,     0,     0,
       0,     0,     0,   211,   214,     0,     0,     0,     0,   213,
     247,     0,   216,     0,   230,   176,     0,     0,   211,   214,
      12,     0,   250,     0,     0,     0,   253,   253,     0,   334,
     329,   253,   292,   287,   256,   257,   297,     0,   307,   386,
     308,     0,    77,   251,   251,     0,     0,   296,     0,   324,
     317,   321,     0,   328,   299,     0,   312,   289,     0,   313,
     314,   315,   316,     0,    60,     0,    57,     0,   211,   211,
     211,   211,   214,   214,     0,     0,    24,     0,    31,   211,
      26,   211,    25,     0,     0,   397,   392,   319,   356,   357,
     358,   359,   360,   361,   362,   363,   369,   367,   368,   370,
       0,   374,     0,   375,     0,   376,     0,   372,     0,   373,
     382,   383,   385,     0,     0,     0,     0,   233,   247,     0,
     236,     0,     0,     0,     0,   122,     0,   125,     0,     0,
       0,    50,   149,     0,   139,   144,     0,    71,     0,    69,
      72,     0,    98,     0,   105,     0,     0,     0,     0,   104,
       0,   211,   211,     0,   162,     0,   211,   211,    96,   164,
       0,   152,   155,     0,     0,    42,   121,     0,   121,     0,
     164,   131,   132,   127,   136,   137,   135,   138,   214,     0,
       0,   234,     0,   234,     0,     0,   212,     0,     0,   181,
       0,     0,   214,     0,     0,   100,   102,     0,     0,     0,
     348,   347,   272,   352,   353,   342,   332,     0,   294,     0,
       0,     0,    66,     0,     0,     0,   277,     0,   285,     0,
       0,   295,     0,     0,     0,   322,   298,     0,   318,   121,
       0,     0,   214,   214,   214,   214,     0,     0,   211,   211,
      23,   251,   251,    33,   214,   214,   401,   399,   379,   380,
     381,   377,   378,     0,   234,     0,   234,     0,     0,   232,
       0,    87,    88,   243,    76,   140,   141,     0,     0,     0,
       0,   124,     0,     0,     0,     0,   134,     0,   103,   211,
     211,   106,     0,     0,   211,   211,   115,   121,   214,   214,
     164,   211,   211,   214,   214,   153,     0,   121,   326,     0,
       0,    42,    41,    85,     0,    95,    93,     0,     0,   230,
       0,   217,     0,     0,     0,   215,     0,   225,   229,   220,
     179,     0,    12,   177,   178,     0,     0,   186,     0,   230,
       0,     0,   253,   254,   345,   336,   340,     0,   344,   253,
     306,   328,   309,    78,     0,   273,   279,   274,   251,   276,
     286,    73,   305,     0,   303,     0,   327,   301,     0,     0,
     103,   121,     0,     0,     0,     0,   230,   230,   214,   214,
      29,    30,   251,     0,     0,     0,     0,   241,     0,     0,
       0,   235,   237,     0,   151,     0,   142,   143,   146,   147,
     145,   148,    70,     0,   214,   214,   211,   211,   214,   214,
       0,     0,     0,   157,     0,     0,   214,   214,   159,   160,
     166,     0,   156,     0,     0,     0,    44,     0,     0,    92,
     230,   195,   221,   219,     0,   224,     0,     0,     0,   231,
       0,     0,     0,   101,     0,   183,     0,     0,   351,     0,
     342,   251,   251,     0,     0,   253,   333,     0,     0,   253,
       0,   278,     0,   304,   302,   300,    59,    61,     0,   230,
       0,     0,     0,   199,   201,     0,     0,    32,     0,   230,
       0,   239,   238,     0,   240,    89,   150,    99,     0,     0,
     214,   214,     0,     0,     0,     0,   107,     0,    97,     0,
       0,   173,     0,    48,    46,     0,    45,     0,     0,    82,
     191,    12,     0,   218,   228,   227,   226,   222,   182,     0,
     230,   180,    12,   253,   253,   346,   339,   338,   337,   335,
     343,   341,     0,   310,   281,     0,   275,    58,   203,     0,
       0,     0,    12,    12,   230,     0,    20,    27,     0,   242,
     108,     0,     0,     0,   110,   111,   116,   117,   158,     0,
       0,   168,   171,     0,   166,     0,    43,     0,     0,    83,
      12,     0,   223,   230,   197,     0,   350,   349,   311,   253,
      12,   230,   230,   230,     0,     0,   187,     0,   230,     0,
     112,     0,     0,   163,   161,   173,     0,   173,    47,    49,
     121,   121,     0,   196,   193,    12,   184,   280,     0,   209,
     207,   205,   200,   202,    12,   230,    28,   109,     0,   114,
       0,   170,   165,   167,    90,    84,   192,    12,     0,   204,
      12,    12,    12,     0,   189,   113,   119,   118,   164,     0,
     198,     0,     0,     0,   188,    12,     0,   194,   210,   208,
     206,     0,   172,   190
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -569,  -569,  -569,  -569,  -569,   -65,   861,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   865,   -33,  -333,  -569,  -569,   -64,
     868,   -36,   872,    28,   -70,   -34,   735,  -568,   -48,   298,
    -251,   -78,  -569,  -569,  -569,   158,  -567,  -569,  -569,  -569,
    -569,  -206,  -569,   -74,  -285,  -277,  -284,  -475,  -569,  -275,
    -569,  -569,   237,  -569,   215,   216,   878,   -20,  -569,  -569,
    -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   724,   456,   409,   632,  -481,     0,
    -144,  -569,   563,  -569,   698,    49,  -569,   -85,  -569,  -569,
    -338,  -569,  -171,   291,  -569,  -569,   477,  -569,   -25,  -569,
    -569,   -31,  -189,   619,  -569,   -29,   936,  -569,  -569,  -569,
    -569,  -569,  -569,  -569,   388,  -504,   385,  -569,  -569,  -569,
    -569,  -569,   500,   972,   -66,  -227,   -82,  -569,   750
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     251,   563,   693,    37,    39,    40,   484,   815,   620,   452,
      54,    55,    41,    42,   103,   152,   458,   459,   105,   178,
     361,   179,   879,   911,   490,   517,   299,   469,   721,   902,
     180,   181,   182,   183,   654,   454,   455,   307,   308,   309,
     617,   948,   811,   481,   871,   872,    45,    46,    28,   642,
     832,   340,   924,   955,   880,   937,   821,   915,   852,   853,
     890,   942,   941,   940,    77,   188,   331,   332,   639,   509,
     284,   439,   440,   292,   293,   310,    29,    50,   195,    51,
     521,   522,   106,   107,   108,   535,   536,   109,   153,   111,
     112,   154,   357,   358,   114,   155,   378,   116,   117,   118,
     119,   525,   763,   655,   656,   873,   658,   120,   121,   122,
     123,   203,   360,   157,   158,   159,   160,   257,   258
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   176,   190,   510,   199,   362,    72,   175,   205,   523,
     176,   453,   456,   526,   212,   627,   175,   104,   127,   285,
     113,   657,   115,   177,   342,    78,   110,   368,   474,   712,
      11,   713,   177,   493,   375,   494,   495,   496,   497,   491,
     492,   636,    85,   446,   824,   335,   929,   234,   421,   423,
     425,   427,   429,   430,   431,   176,    68,   294,   252,   505,
     201,   175,   202,   260,   769,   578,   735,    47,    48,   193,
     384,    56,   666,    33,    75,   253,   254,   177,    82,   505,
      64,    57,   300,   316,   317,    76,   325,   601,   362,    83,
     287,   288,   301,   336,    47,   318,   176,   667,   347,   702,
     175,   175,   184,    73,   326,   211,    81,   770,   668,    69,
     327,   637,   825,   713,   328,   540,   337,   930,   177,   506,
     325,    58,   363,   364,   185,   579,   736,    34,   187,   385,
     102,   191,   303,   560,   433,   725,   304,   866,   326,   348,
    -247,   176,  -247,   329,   327,   305,   306,   175,   328,   742,
     619,   743,   434,   745,   330,     8,    74,   816,   435,    79,
     362,   104,   436,   177,   113,   587,   115,    79,   817,    80,
     110,   585,   586,   232,   104,   104,   539,   113,   113,   115,
     115,   359,   531,   110,   110,   433,    74,   359,   330,   390,
     323,   437,   486,   568,   359,   569,   262,   570,    79,   571,
     432,   572,   438,   434,   615,   416,   417,   418,   419,   435,
     611,   294,   479,   436,   294,   791,   733,   792,   760,   794,
     210,   532,   648,   210,   176,   747,   487,   391,   176,   761,
     175,   730,   480,   263,   589,   590,   333,   867,    10,   264,
     265,   266,   267,   268,   269,   596,   177,    35,   612,    52,
     177,    36,   211,   438,   762,   211,   657,    14,    15,   211,
      16,   271,   211,   731,   561,   319,   320,    53,   272,   273,
     461,   176,   270,    12,   462,   514,    17,   321,   534,   537,
     463,   624,    18,   626,   387,    19,    20,   562,   289,    22,
     394,   395,   274,   177,     1,    30,   176,    47,   322,    13,
      31,     2,   175,   705,   457,   219,   300,   102,   708,   709,
     710,   711,   706,   707,   758,   176,   301,   201,   177,   524,
     464,   766,    85,   102,   465,   663,   220,    38,   221,   222,
      23,   223,   441,   466,   467,   468,   359,   177,    43,    24,
      25,    44,   461,    26,   302,   515,   797,    86,   470,    85,
     346,   653,   463,   673,   675,    61,   303,   631,   678,   633,
     304,   602,    49,    72,   102,    62,   127,   345,    85,   305,
     306,   603,    85,   176,   500,   263,   502,    85,    95,   504,
      78,   264,   265,   266,   267,   268,   269,   592,   593,    97,
     150,    70,   464,   350,    59,   177,   465,   665,    71,   594,
     813,   814,   669,    60,    85,   466,   467,   468,   623,   102,
     625,   652,   589,   590,   270,    63,   176,   176,   176,   176,
     595,   591,   175,   175,   175,   175,   338,   841,   312,   846,
     697,   844,   699,   217,    79,   218,   263,   176,   177,   177,
     177,   177,   264,   265,   266,   267,   268,   269,  -291,    47,
    -291,   313,   383,   314,   589,   590,   315,   551,   300,   177,
      79,   176,   589,   590,   290,    79,   759,   175,   301,   589,
     590,   679,    65,   956,   835,   270,   690,   691,   558,   176,
     704,   559,   573,   177,   575,   175,    79,   577,   104,    79,
    -247,   113,  -247,   115,   584,   886,   887,   110,   359,   354,
     355,   177,   531,   275,   276,   277,   589,   590,   303,    66,
     176,   671,   304,   359,   359,   278,   279,   796,   359,   531,
     124,   305,   306,    79,    67,   125,   444,   176,   527,   722,
     135,   818,   177,   445,    34,   528,   280,   176,   527,   732,
     194,   281,   908,   175,   909,   541,    79,   176,   482,   177,
     244,   917,   245,   175,   333,   246,   527,   138,   126,   177,
     176,   139,   176,   546,   176,   597,   527,   196,   247,   177,
     156,   197,   598,   670,   248,   404,   176,   750,   249,   128,
     405,   250,   177,   772,   177,   946,   177,   833,   544,   947,
     200,   129,   204,   545,   206,   207,   460,   597,   177,   527,
     213,   582,   527,   778,   680,   544,   773,   787,   527,   774,
     548,   176,   263,   130,    79,   775,   507,   175,   264,   265,
     266,   267,   268,   269,   875,   133,   176,   441,   176,   741,
     176,   876,   261,   177,   226,   227,   737,   164,   165,   516,
    -293,   738,  -293,   256,   259,   216,   131,   167,   177,   755,
     177,   270,   177,   132,   187,   168,   169,   170,   171,   172,
     168,   169,   170,   171,   286,    14,    15,   176,    16,   134,
    -292,   343,  -292,   175,   176,   344,   837,   838,   102,   186,
     189,   176,   834,   460,    17,   192,   783,   784,   198,   177,
      18,   208,   214,    19,    20,    21,   177,    22,   215,   220,
     235,   221,   222,   177,   223,   236,   261,   263,   226,   529,
     176,   176,   176,   264,   265,   266,   267,   268,   269,   370,
     176,   237,   373,   176,   376,   238,   239,   373,   379,   380,
     381,   382,   177,   177,   177,   176,   240,   176,    23,   241,
     820,   516,   177,   243,   242,   177,   270,    24,    25,   216,
      79,    26,   580,    79,   282,   607,   881,   177,    79,   177,
     630,    79,   470,   632,   291,   283,   450,   885,    79,   333,
     634,   298,   311,    84,   168,   169,   170,   171,   451,   848,
     499,    79,   334,   696,    79,   176,   698,   894,   895,   857,
      79,   176,   700,   341,   365,   513,   224,   366,   369,   176,
     176,   764,   386,   765,   752,   638,   764,   177,   906,   442,
     371,   485,   377,   177,   388,   912,   389,   225,   399,   226,
     227,   177,   177,   401,   443,   918,   228,   229,   230,   231,
     884,   176,   934,   935,   297,   407,   485,   140,   447,   471,
     176,   176,   472,   518,   519,   520,   175,   175,   556,   557,
     938,   473,   475,   177,   896,   476,   143,   478,   144,   943,
     477,   145,   177,   177,   488,   489,   146,   147,   148,   149,
     150,   501,   949,   508,   503,   951,   952,   953,   638,   511,
     533,   542,   151,   914,   543,   549,   547,   574,   576,   102,
     961,   919,   920,   921,   581,   460,   659,    87,   926,   588,
     599,   600,   823,   566,   256,   604,   605,   606,   610,   829,
     324,   622,   629,    88,    89,    90,   339,   616,    91,    92,
     640,   641,   643,   644,   647,   944,   646,   649,   638,  -326,
     638,    93,   638,   664,    94,   681,   686,   687,   849,   850,
     851,   692,   703,   716,   516,   717,   720,   734,   858,   740,
    -326,   859,  -326,  -326,   628,   210,   744,   739,    95,  -326,
    -326,  -326,  -326,   392,   393,   868,    96,   746,   645,    97,
      98,   650,   749,   748,   753,   651,   751,   754,    99,   776,
     777,   100,   101,   621,   779,   780,    85,   211,   781,   102,
     782,   788,   789,   790,   638,   263,   638,   793,   638,   795,
     805,   264,   265,   266,   267,   268,   269,   806,   682,   683,
     684,   685,   807,   897,   808,   822,   819,   812,   826,   899,
     694,   695,   828,   830,   831,   839,   845,   903,   904,   661,
     662,   847,   854,   855,   270,   856,   860,   861,   864,   865,
     869,   874,   516,   870,   676,   877,   883,   498,   891,   516,
     892,   893,   878,   889,   898,    87,   900,   901,   905,   928,
     910,   913,   512,   916,   723,   724,   922,   923,   925,   728,
     729,    88,    89,    90,   927,   932,    91,    92,   516,   516,
     516,   936,   939,   945,   950,   954,   -50,   -50,   516,    93,
     294,   516,    94,   957,   958,   959,   -50,   -50,   -50,   960,
     962,   295,   963,   460,   296,   516,   297,   398,   -50,   396,
     349,   907,   552,   553,   554,   555,    95,   397,   402,   -50,
     550,   931,   933,   564,   263,   565,   400,    97,   353,   804,
     264,   265,   266,   267,   268,   269,    99,   635,   211,   100,
     101,   701,   583,   771,   785,   786,   660,   102,   836,   840,
     756,   757,     0,   516,   567,     0,   161,   162,   372,   516,
       0,     0,     0,   270,     0,     0,   163,   516,   516,     0,
     798,   799,     0,     0,   802,   803,     0,     0,     0,     0,
       0,     0,   809,   810,   164,   165,     0,     0,     0,     0,
       0,   166,     0,     0,   167,   608,   609,  -326,     0,   516,
     613,   614,   168,   169,   170,   171,   172,     0,     0,     0,
     173,     0,     0,   174,     0,     0,   161,   162,  -326,   209,
    -326,  -326,     0,   210,     0,   102,   163,  -326,  -326,  -326,
    -326,     0,     0,   485,   485,   408,   409,   410,   411,   412,
     413,   414,   415,     0,   164,   165,     0,     0,   827,     0,
       0,   166,     0,     0,   167,   211,   862,   863,     0,     0,
     140,     0,   168,   169,   170,   171,   172,   842,   843,     0,
     233,   141,   142,   174,     0,     0,     0,     0,     0,   143,
     255,   144,   688,   689,   145,   102,     0,   140,     0,   146,
     147,   148,   149,   150,     0,     0,     0,     0,   141,   142,
       0,     0,     0,     0,     0,   151,   143,     0,   144,   356,
       0,   145,   102,     0,     0,     0,   146,   147,   148,   149,
     150,     0,   882,   714,   715,     0,   140,     0,   718,   719,
       0,     0,   151,     0,     0,   726,   727,   141,   142,   102,
       0,     0,     0,     0,     0,   143,     0,   144,   367,     0,
     145,     0,     0,     0,     0,   146,   147,   148,   149,   150,
       0,   140,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   151,   141,   142,     0,   485,     0,   485,   102,     0,
     143,     0,   144,   374,     0,   145,   140,     0,     0,     0,
     146,   147,   148,   149,   150,     0,     0,   141,   142,     0,
       0,     0,     0,     0,     0,   143,   151,   144,   538,     0,
     145,     0,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   140,     0,     0,     0,     0,     0,     0,
       0,   151,     0,     0,   141,   142,     0,     0,   102,     0,
     800,   801,   143,     0,   144,   672,     0,   145,   140,     0,
       0,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     674,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   677,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   483,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   483,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   618,     0,     0,     0,   420,     0,     0,     0,     0,
       0,     0,   143,   151,   144,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,     0,
     422,     0,     0,     0,     0,     0,     0,   143,   151,   144,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,   143,   151,   144,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,   143,   151,   144,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,     0,   428,     0,     0,     0,     0,
       0,  -319,   143,   151,   144,     0,     0,   145,     0,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,     0,
       0,   351,  -319,     0,  -319,  -319,     0,   547,   151,     0,
     263,  -319,  -319,  -319,  -319,   102,   264,   265,   266,   267,
     268,   269,   263,     0,     0,     0,     0,     0,   264,   265,
     266,   267,   268,   269,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   263,   352,     0,     0,     0,   270,
     264,   265,   266,   267,   268,   269,   263,     0,     0,     0,
       0,   270,   264,   265,   266,   267,   268,   269,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   263,   406,
       0,     0,     0,   270,   264,   265,   266,   267,   268,   269,
       0,   263,     0,     0,     0,   270,   530,   264,   265,   266,
     267,   268,   269,   263,     0,     0,     0,     0,     0,   264,
     265,   266,   267,   268,   269,   767,   263,   270,     0,     0,
       0,   768,   264,   265,   266,   267,   268,   269,     0,     0,
     270,     0,     0,     0,     0,     0,     0,   888,   263,     0,
     448,   449,   270,     0,   264,   265,   266,   267,   268,   269,
     450,     0,     0,     0,     0,   270,     0,     0,   168,   169,
     170,   171,   451,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   270
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   336,    89,   211,    39,    71,    93,   347,
      80,   296,   297,   351,    99,   490,    80,    51,    54,   163,
      51,   525,    51,    71,   195,    45,    51,   216,   303,   597,
       2,   598,    80,   318,   223,   319,   320,   321,   322,   316,
     317,     6,     5,   294,     8,     5,     5,   125,   275,   276,
     277,   278,   279,   280,   281,   125,     5,    48,   140,     5,
      91,   125,    91,   145,    17,     5,     5,    18,    19,     5,
       5,    26,     5,     5,    48,   141,   142,   125,    48,     5,
      31,    36,    26,    54,    55,    59,     8,     5,   294,    59,
     164,   165,    36,    53,    45,    66,   166,    30,    17,   580,
     164,   165,    74,     5,    26,    96,     5,    60,    41,    58,
      32,    76,    76,   680,    36,   366,    76,    76,   166,    65,
       8,    76,    85,    86,    75,    65,    65,    59,    64,    64,
      95,    82,    76,     5,     8,   610,    80,    63,    26,    58,
      58,   211,    60,    65,    32,    89,    90,   211,    36,   630,
     483,   632,    26,   634,    76,    76,    58,    65,    32,    58,
     366,   195,    36,   211,   195,   450,   195,    58,    76,    60,
     195,   448,   449,   124,   208,   209,   365,   208,   209,   208,
     209,   210,    58,   208,   209,     8,    58,   216,    76,    38,
      50,    65,    28,   420,   223,   422,     5,   424,    58,   426,
     282,   428,    76,    26,   479,   271,   272,   273,   274,    32,
      38,    48,     5,    36,    48,   696,    60,   698,     5,   700,
      64,    97,    59,    64,   294,    59,    62,    76,   298,    16,
     294,    28,    25,    42,    54,    55,   187,   805,    76,    48,
      49,    50,    51,    52,    53,    65,   294,    72,    76,    76,
     298,    76,    96,    76,    41,    96,   760,     7,     8,    96,
      10,    47,    96,    60,    14,    44,    45,    94,    54,    55,
      26,   341,    81,     0,    30,   340,    26,    56,   363,   364,
      36,   487,    32,   489,   235,    35,    36,    37,    76,    39,
     241,   242,    78,   341,     4,    59,   366,   248,    77,    59,
      64,    11,   366,   588,    76,    38,    26,    95,   592,   593,
     594,   595,   589,   590,   652,   385,    36,   348,   366,   348,
      76,   659,     5,    95,    80,   531,    59,    76,    61,    62,
      80,    64,   283,    89,    90,    91,   365,   385,    76,    89,
      90,    76,    26,    93,    64,    76,    30,    30,   299,     5,
      17,   522,    36,   542,   543,    26,    76,   501,   547,   503,
      80,    26,    76,   396,    95,    36,   402,    23,     5,    89,
      90,    36,     5,   443,   325,    42,   327,     5,    64,   330,
     400,    48,    49,    50,    51,    52,    53,    44,    45,    75,
      76,    53,    76,    30,    76,   443,    80,    30,    60,    56,
     733,   734,    30,    76,     5,    89,    90,    91,   486,    95,
     488,    17,    54,    55,    81,    76,   486,   487,   488,   489,
      77,    63,   486,   487,   488,   489,    50,   765,     6,    30,
     574,   769,   576,    59,    58,    61,    42,   507,   486,   487,
     488,   489,    48,    49,    50,    51,    52,    53,    59,   400,
      61,    29,    50,    31,    54,    55,    34,    50,    26,   507,
      58,   531,    54,    55,   166,    58,    66,   531,    36,    54,
      55,   549,     5,   948,   759,    81,   561,   562,    50,   549,
      65,    50,   433,   531,   435,   549,    58,   438,   522,    58,
      58,   522,    60,   522,    50,   833,   834,   522,   527,   208,
     209,   549,    58,    44,    45,    46,    54,    55,    76,    12,
     580,    50,    80,   542,   543,    56,    57,    65,   547,    58,
      48,    89,    90,    58,    76,    53,    58,   597,    58,   607,
      65,   737,   580,    65,    59,    65,    77,   607,    58,   617,
      84,    82,   875,   607,   877,    65,    58,   617,    60,   597,
       8,   889,    10,   617,   505,    13,    58,    72,    76,   607,
     630,    76,   632,    65,   634,    58,    58,    72,    26,   617,
      70,    76,    65,    65,    32,    58,   646,   642,    36,    76,
      63,    39,   630,   668,   632,    72,   634,    17,    58,    76,
      90,    76,    92,    63,    94,    95,   298,    58,   646,    58,
     100,   443,    58,   681,    65,    58,    65,   692,    58,    65,
      63,   681,    42,    76,    58,    65,    60,   681,    48,    49,
      50,    51,    52,    53,    58,    76,   696,   578,   698,   629,
     700,    65,    59,   681,    61,    62,    58,    54,    55,   341,
      59,    63,    61,   143,   144,    64,    59,    64,   696,   649,
     698,    81,   700,    59,    64,    72,    73,    74,    75,    76,
      72,    73,    74,    75,    76,     7,     8,   737,    10,    76,
      59,    38,    61,   737,   744,    42,   761,   762,    95,    76,
      76,   751,    17,   385,    26,    76,   686,   687,    76,   737,
      32,    60,    76,    35,    36,    37,   744,    39,    76,    59,
      48,    61,    62,   751,    64,    59,    59,    42,    61,    62,
     780,   781,   782,    48,    49,    50,    51,    52,    53,   219,
     790,    59,   222,   793,   224,    59,    76,   227,   228,   229,
     230,   231,   780,   781,   782,   805,    76,   807,    80,    48,
     740,   443,   790,    59,    48,   793,    81,    89,    90,    64,
      58,    93,    60,    58,    79,    60,   821,   805,    58,   807,
      60,    58,   713,    60,    76,    64,    64,   832,    58,   720,
      60,    64,    53,    49,    72,    73,    74,    75,    76,   779,
     324,    58,     5,    60,    58,   855,    60,   852,   853,   789,
      58,   861,    60,    60,    64,   339,    38,    48,    76,   869,
     870,    58,     5,    60,   646,   507,    58,   855,    60,    28,
      76,   311,    76,   861,    76,   880,    76,    59,    76,    61,
      62,   869,   870,    76,    60,   890,    68,    69,    70,    71,
     830,   901,   910,   911,    64,    76,   336,    43,    76,    76,
     910,   911,    76,   343,   344,   345,   910,   911,   392,   393,
     915,    76,    76,   901,   854,    76,    62,    30,    64,   924,
      76,    67,   910,   911,    28,    28,    72,    73,    74,    75,
      76,    76,   937,     9,    76,   940,   941,   942,   580,    76,
      92,    64,    88,   883,    64,    53,    64,    76,    76,    95,
     955,   891,   892,   893,    76,   597,    41,     3,   898,    64,
      76,    76,   744,   403,   404,    76,    76,    76,    65,   751,
     186,     5,     5,    19,    20,    21,   192,    76,    24,    25,
       5,    76,     5,     5,     5,   925,    60,     5,   630,    38,
     632,    37,   634,    76,    40,    53,     5,     5,   780,   781,
     782,    15,    28,    76,   646,    76,    62,    58,   790,     5,
      59,   793,    61,    62,   498,    64,    60,    30,    64,    68,
      69,    70,    71,   239,   240,   807,    72,    28,   512,    75,
      76,    18,     5,    53,    76,    22,    60,     9,    84,     5,
       5,    87,    88,   483,     5,    60,     5,    96,    60,    95,
      60,    30,     5,    60,   696,    42,   698,    60,   700,    76,
      60,    48,    49,    50,    51,    52,    53,     5,   552,   553,
     554,   555,    60,   855,    30,    53,    28,    76,    76,   861,
     564,   565,     5,     5,     5,    30,    76,   869,   870,   529,
     530,     5,     5,    60,    81,    59,     5,    60,     5,     5,
      60,    28,   744,    60,   544,    60,     5,   323,     5,   751,
       5,     5,    63,    17,     5,     3,     5,    60,     5,   901,
      28,     5,   338,     5,   608,   609,     5,     5,     5,   613,
     614,    19,    20,    21,     5,    64,    24,    25,   780,   781,
     782,     5,     5,     5,     5,     5,    44,    45,   790,    37,
      48,   793,    40,     5,     5,     5,    54,    55,    56,     5,
      65,    59,     5,   805,    62,   807,    64,   246,    66,   244,
      28,   874,   388,   389,   390,   391,    64,   245,   250,    77,
     385,   905,   907,   399,    42,   401,   248,    75,    76,   720,
      48,    49,    50,    51,    52,    53,    84,   505,    96,    87,
      88,   578,   444,   666,   688,   689,   527,    95,   760,   764,
     650,   651,    -1,   855,   404,    -1,    26,    27,   222,   861,
      -1,    -1,    -1,    81,    -1,    -1,    36,   869,   870,    -1,
     714,   715,    -1,    -1,   718,   719,    -1,    -1,    -1,    -1,
      -1,    -1,   726,   727,    54,    55,    -1,    -1,    -1,    -1,
      -1,    61,    -1,    -1,    64,   471,   472,    38,    -1,   901,
     476,   477,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      80,    -1,    -1,    83,    -1,    -1,    26,    27,    59,    60,
      61,    62,    -1,    64,    -1,    95,    36,    68,    69,    70,
      71,    -1,    -1,   733,   734,   263,   264,   265,   266,   267,
     268,   269,   270,    -1,    54,    55,    -1,    -1,   748,    -1,
      -1,    61,    -1,    -1,    64,    96,   800,   801,    -1,    -1,
      43,    -1,    72,    73,    74,    75,    76,   767,   768,    -1,
      80,    54,    55,    83,    -1,    -1,    -1,    -1,    -1,    62,
      63,    64,   558,   559,    67,    95,    -1,    43,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    88,    62,    -1,    64,    65,
      -1,    67,    95,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,   822,   599,   600,    -1,    43,    -1,   604,   605,
      -1,    -1,    88,    -1,    -1,   611,   612,    54,    55,    95,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    64,    65,    -1,
      67,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    54,    55,    -1,   875,    -1,   877,    95,    -1,
      62,    -1,    64,    65,    -1,    67,    43,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,
      67,    -1,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    54,    55,    -1,    -1,    95,    -1,
     716,   717,    62,    -1,    64,    65,    -1,    67,    43,    -1,
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
      -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    38,    62,    88,    64,    -1,    -1,    67,    -1,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    33,    59,    -1,    61,    62,    -1,    64,    88,    -1,
      42,    68,    69,    70,    71,    95,    48,    49,    50,    51,
      52,    53,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    65,    -1,    -1,    -1,    81,
      48,    49,    50,    51,    52,    53,    42,    -1,    -1,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    65,
      -1,    -1,    -1,    81,    48,    49,    50,    51,    52,    53,
      -1,    42,    -1,    -1,    -1,    81,    60,    48,    49,    50,
      51,    52,    53,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    66,    42,    81,    -1,    -1,
      -1,    60,    48,    49,    50,    51,    52,    53,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    63,    42,    -1,
      54,    55,    81,    -1,    48,    49,    50,    51,    52,    53,
      64,    -1,    -1,    -1,    -1,    81,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    99,   100,   103,   104,   106,    76,   101,
      76,   121,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    89,    90,    93,   105,   156,   184,
      59,    64,   102,     5,    59,    72,    76,   111,    76,   112,
     113,   120,   121,    76,    76,   154,   155,   183,   183,    76,
     185,   187,    76,    94,   118,   119,    26,    36,    76,    76,
      76,    26,    36,    76,   183,     5,    12,    76,     5,    58,
      53,    60,   113,     5,    58,    48,    59,   172,   155,    58,
      60,     5,    48,    59,   172,     5,    30,     3,    19,    20,
      21,    24,    25,    37,    40,    64,    72,    75,    76,    84,
      87,    88,    95,   122,   123,   126,   190,   191,   192,   195,
     196,   197,   198,   199,   202,   203,   205,   206,   207,   208,
     215,   216,   217,   218,    48,    53,    76,   119,    76,    76,
      76,    59,    59,    76,    76,    65,   103,   107,    72,    76,
      43,    54,    55,    62,    64,    67,    72,    73,    74,    75,
      76,    88,   123,   196,   199,   203,   220,   221,   222,   223,
     224,    26,    27,    36,    54,    55,    61,    64,    72,    73,
      74,    75,    76,    80,    83,   117,   122,   126,   127,   129,
     138,   139,   140,   141,   121,   183,    76,    64,   173,    76,
     129,   183,    76,     5,   173,   186,    72,    76,    76,   185,
     220,   199,   203,   219,   220,   185,   220,   220,    60,    60,
      64,    96,   185,   220,    76,    76,    64,    59,    61,    38,
      59,    61,    62,    64,    38,    59,    61,    62,    68,    69,
      70,    71,   183,    80,   129,    48,    59,    59,    59,    76,
      76,    48,    48,    59,     8,    10,    13,    26,    32,    36,
      39,   108,   224,   222,   222,    63,   220,   225,   226,   220,
     224,    59,     5,    42,    48,    49,    50,    51,    52,    53,
      81,    47,    54,    55,    78,    44,    45,    46,    56,    57,
      77,    82,    79,    64,   178,   178,    76,   141,   141,    76,
     127,    76,   181,   182,    48,    59,    62,    64,    64,   134,
      26,    36,    64,    76,    80,    89,    90,   145,   146,   147,
     183,    53,     6,    29,    31,    34,    54,    55,    66,    44,
      45,    56,    77,    50,   172,     8,    26,    32,    36,    65,
      76,   174,   175,   183,     5,     5,    53,    76,    50,   172,
     159,    60,   190,    38,    42,    23,    17,    17,    58,    28,
      30,    33,    65,    76,   191,   191,    65,   200,   201,   203,
     220,   128,   139,    85,    86,    64,    48,    65,   200,    76,
     220,    76,   204,   220,    65,   200,   220,    76,   204,   220,
     220,   220,   220,    50,     5,    64,     5,   183,    76,    76,
      38,    76,   172,   172,   183,   183,   112,   120,   104,    76,
     154,    76,   118,    66,    58,    63,    65,    76,   221,   221,
     221,   221,   221,   221,   221,   221,   222,   222,   222,   222,
      55,   223,    55,   223,    55,   223,    55,   223,    55,   223,
     223,   223,   224,     8,    26,    32,    36,    65,    76,   179,
     180,   183,    28,    60,    58,    65,   128,    76,    54,    55,
      64,    76,   117,   142,   143,   144,   142,    76,   124,   125,
     127,    26,    30,    36,    76,    80,    89,    90,    91,   135,
     183,    76,    76,    76,   147,    76,    76,    76,    30,     5,
      25,   151,    60,    64,   114,   220,    28,    62,    28,    28,
     132,   143,   143,   142,   144,   144,   144,   144,   172,   173,
     183,    76,   183,    76,   183,     5,    65,    60,     9,   177,
     114,    76,   172,   173,   103,    76,   127,   133,   220,   220,
     220,   188,   189,   188,   203,   209,   188,    58,    65,    62,
      60,    58,    97,    92,   185,   193,   194,   185,    65,   200,
     128,    65,    64,    64,    58,    63,    65,    64,    63,    53,
     124,    50,   172,   172,   172,   172,   173,   173,    50,    50,
       5,    14,    37,   109,   172,   172,   220,   226,   223,   223,
     223,   223,   223,   183,    76,   183,    76,   183,     5,    65,
      60,    76,   133,   182,    50,   143,   143,   142,    64,    54,
      55,    63,    44,    45,    56,    77,    65,    58,    65,    76,
      76,     5,    26,    36,    76,    76,    76,    60,   172,   172,
      65,    38,    76,   172,   172,   147,    76,   148,    76,   114,
     116,   220,     5,   129,   139,   129,   139,   145,   173,     5,
      60,   178,    60,   178,    60,   175,     6,    76,   127,   176,
       5,    76,   157,     5,     5,   173,    60,     5,    59,     5,
      18,    22,    17,   190,   142,   211,   212,   213,   214,    41,
     201,   220,   220,   139,    76,    30,     5,    30,    41,    30,
      65,    50,    65,   200,    65,   200,   220,    65,   200,   129,
      65,    53,   173,   173,   173,   173,     5,     5,   172,   172,
     185,   185,    15,   110,   173,   173,    60,   178,    60,   178,
      60,   180,   176,    28,    65,   142,   143,   143,   144,   144,
     144,   144,   125,   134,   172,   172,    76,    76,   172,   172,
      62,   136,   129,   173,   173,   145,   172,   172,   173,   173,
      28,    60,   129,    60,    58,     5,    65,    58,    63,    30,
       5,   177,   176,   176,    60,   176,    28,    59,    53,     5,
     103,    60,   133,    76,     9,   177,   220,   220,   188,    66,
       5,    16,    41,   210,    58,    60,   188,    66,    60,    17,
      60,   194,   185,    65,    65,    65,     5,     5,   129,     5,
      60,    60,    60,   177,   177,   173,   173,   185,    30,     5,
      60,   176,   176,    60,   176,    76,    65,    30,   173,   173,
     172,   172,   173,   173,   174,    60,     5,    60,    30,   173,
     173,   150,    76,   114,   114,   115,    65,    76,   139,    28,
     177,   164,    53,   133,     8,    76,    76,   220,     5,   133,
       5,     5,   158,    17,    17,   142,   212,   185,   185,    30,
     214,   188,   220,   220,   188,    76,    30,     5,   177,   133,
     133,   133,   166,   167,     5,    60,    59,   177,   133,   133,
       5,    60,   173,   173,     5,     5,    63,   125,   133,    60,
      60,   152,   153,   213,    28,    58,    65,    60,    63,   130,
     162,   103,   220,     5,   177,   103,   188,   188,    63,    17,
     168,     5,     5,     5,   103,   103,   177,   133,     5,   133,
       5,    60,   137,   133,   133,     5,    60,   150,   114,   114,
      28,   131,   103,     5,   177,   165,     5,   188,   103,   177,
     177,   177,     5,     5,   160,     5,   177,     5,   133,     5,
      76,   153,    64,   152,   129,   129,     5,   163,   103,     5,
     171,   170,   169,   103,   177,     5,    72,    76,   149,   103,
       5,   103,   103,   103,     5,   161,   145,     5,     5,     5,
       5,   103,    65,     5
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
     119,   119,   120,   120,   121,   121,   122,   123,   123,   124,
     124,   125,   125,   126,   127,   127,   127,   128,   128,   129,
     129,   129,   130,   131,   129,   129,   129,   129,   129,   129,
     129,   132,   129,   129,   129,   129,   129,   129,   129,   129,
     133,   133,   133,   134,   134,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   136,   136,   137,   137,   137,
     138,   138,   139,   139,   139,   139,   139,   139,   140,   140,
     140,   140,   140,   141,   141,   141,   141,   141,   141,   142,
     142,   142,   142,   142,   143,   143,   143,   143,   143,   144,
     144,   144,   145,   146,   146,   148,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   149,   150,   151,   151,   151,
     152,   152,   153,   153,   154,   154,   155,   155,   155,   156,
     156,   157,   156,   158,   156,   159,   156,   160,   156,   161,
     156,   162,   156,   163,   156,   164,   156,   165,   156,   166,
     156,   167,   156,   168,   156,   169,   156,   170,   156,   171,
     156,   172,   173,   173,   173,   174,   174,   175,   175,   175,
     175,   175,   175,   175,   175,   176,   176,   176,   176,   176,
     177,   177,   178,   178,   178,   179,   179,   180,   180,   180,
     180,   180,   180,   181,   181,   182,   183,   183,   184,   186,
     185,   187,   185,   189,   188,   190,   190,   190,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   192,   192,   192,   192,   193,   193,   193,
     194,   194,   195,   195,   196,   196,   196,   197,   197,   197,
     197,   198,   198,   198,   199,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   200,   200,   201,   201,
     201,   201,   202,   202,   202,   202,   202,   202,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   204,   204,   205,
     206,   206,   207,   207,   209,   208,   210,   210,   210,   211,
     211,   212,   212,   213,   213,   214,   214,   215,   216,   217,
     217,   217,   218,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   221,   221,   221,   221,   221,   221,
     221,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   223,   223,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     226,   226
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
       9,     0,     5,     4,     2,     4,     3,     6,     3,     6,
       1,     3,     1,     0,     2,     1,     2,     4,     5,     7,
       5,     5,     6,     8,     7,     0,     3,     0,     3,     3,
       1,     0,     3,     1,     4,     3,     1,     3,     1,     2,
       2,     3,     3,     1,     4,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     1,     3,     3,     3,     3,     1,
       4,     3,     2,     3,     1,     0,     4,     4,     6,     4,
       4,     7,     2,     7,     0,     0,     0,     7,     5,     0,
       3,     1,     6,     0,     2,     1,     4,     6,     6,     7,
       9,     0,     9,     0,    11,     0,     7,     0,    13,     0,
      15,     0,    12,     0,    14,     0,    11,     0,    13,     0,
      11,     0,    11,     0,    12,     0,    14,     0,    14,     0,
      14,     0,     3,     2,     0,     3,     1,     3,     5,     4,
       3,     4,     5,     6,     4,     1,     3,     3,     3,     1,
       0,     3,     3,     2,     0,     3,     1,     3,     4,     4,
       4,     3,     5,     3,     1,     1,     3,     1,     3,     0,
       4,     0,     2,     0,     2,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     5,     5,     7,     5,     1,     3,     2,
       6,     4,     1,     2,     2,     4,     5,     1,     1,     3,
       1,     1,     3,     1,     4,     4,     3,     3,     4,     3,
       6,     5,     6,     5,     6,     5,     3,     1,     1,     3,
       5,     6,     3,     3,     3,     3,     3,     3,     4,     3,
       2,     3,     4,     2,     3,     2,     1,     3,     1,     3,
       2,     2,     4,     6,     0,     7,     0,     2,     2,     3,
       1,     3,     0,     3,     1,     1,     3,     4,     4,     8,
       8,     6,     4,     3,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     3,     4,     4,     4,
       4,     4,     3,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     1,     2,     2,     2,     3,     1,     3,
       1,     3
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
#line 2302 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2308 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2314 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2320 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2326 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2332 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2338 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2344 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2350 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2356 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2362 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2368 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2374 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2380 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2386 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2392 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2398 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2404 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2410 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2416 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2422 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2428 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2434 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2440 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2446 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2452 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2458 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2464 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2470 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2476 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2482 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2488 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2494 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2500 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2506 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2512 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2518 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2524 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2530 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2536 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2542 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2548 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2554 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2560 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2566 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2572 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2578 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2584 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2590 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2596 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2602 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2608 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2614 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2620 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2626 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2632 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2638 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2644 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2650 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2656 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2662 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2668 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2674 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2680 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2686 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2692 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2698 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2704 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2710 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2716 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2722 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2728 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2734 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2740 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2746 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2752 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2758 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2764 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2770 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2776 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2782 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2788 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2794 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2800 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2806 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2812 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2818 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2824 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2830 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2836 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2842 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2848 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2854 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2860 "pascal.tab.c"
    break;

  case 96: /* type: OBJECTSY record_body ENDSY  */
#line 375 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2866 "pascal.tab.c"
    break;

  case 97: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 376 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2872 "pascal.tab.c"
    break;

  case 98: /* type: CLASSSY class_members ENDSY  */
#line 377 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2878 "pascal.tab.c"
    break;

  case 99: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 378 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2884 "pascal.tab.c"
    break;

  case 100: /* result_type: IDENT  */
#line 381 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2890 "pascal.tab.c"
    break;

  case 101: /* result_type: IDENT PERIOD IDENT  */
#line 382 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2896 "pascal.tab.c"
    break;

  case 102: /* result_type: spec_ref  */
#line 383 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2902 "pascal.tab.c"
    break;

  case 103: /* class_members: %empty  */
#line 386 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2908 "pascal.tab.c"
    break;

  case 104: /* class_members: class_members class_member  */
#line 387 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2914 "pascal.tab.c"
    break;

  case 105: /* class_member: IDENT  */
#line 390 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2920 "pascal.tab.c"
    break;

  case 106: /* class_member: IDENT SEMICOLON  */
#line 391 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2926 "pascal.tab.c"
    break;

  case 107: /* class_member: id_list COLON type SEMICOLON  */
#line 392 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2932 "pascal.tab.c"
    break;

  case 108: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2938 "pascal.tab.c"
    break;

  case 109: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 394 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2944 "pascal.tab.c"
    break;

  case 110: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2950 "pascal.tab.c"
    break;

  case 111: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 396 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2956 "pascal.tab.c"
    break;

  case 112: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 397 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2962 "pascal.tab.c"
    break;

  case 113: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 398 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2968 "pascal.tab.c"
    break;

  case 114: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 399 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2974 "pascal.tab.c"
    break;

  case 115: /* prop_index_opt: %empty  */
#line 402 "pascal.y"
    { (yyval.node) = NULL; }
#line 2980 "pascal.tab.c"
    break;

  case 116: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 403 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2986 "pascal.tab.c"
    break;

  case 117: /* prop_specs: %empty  */
#line 406 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2992 "pascal.tab.c"
    break;

  case 118: /* prop_specs: prop_specs IDENT IDENT  */
#line 407 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2998 "pascal.tab.c"
    break;

  case 119: /* prop_specs: prop_specs IDENT INTCONST  */
#line 408 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3004 "pascal.tab.c"
    break;

  case 120: /* packed_opt: PACKEDSY  */
#line 411 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3010 "pascal.tab.c"
    break;

  case 121: /* packed_opt: %empty  */
#line 412 "pascal.y"
     { (yyval.node) = NULL; }
#line 3016 "pascal.tab.c"
    break;

  case 122: /* simple_type: LPARENT enum_list RPARENT  */
#line 415 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3022 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT  */
#line 416 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3028 "pascal.tab.c"
    break;

  case 124: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 417 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3034 "pascal.tab.c"
    break;

  case 125: /* simple_type: IDENT PERIOD IDENT  */
#line 418 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3040 "pascal.tab.c"
    break;

  case 126: /* simple_type: spec_ref  */
#line 419 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3046 "pascal.tab.c"
    break;

  case 127: /* simple_type: lo_bound DOTDOT b_expr  */
#line 420 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3052 "pascal.tab.c"
    break;

  case 128: /* lo_bound: lo_term  */
#line 423 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3058 "pascal.tab.c"
    break;

  case 129: /* lo_bound: PLUS lo_term  */
#line 424 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3064 "pascal.tab.c"
    break;

  case 130: /* lo_bound: MINUS lo_term  */
#line 425 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3070 "pascal.tab.c"
    break;

  case 131: /* lo_bound: lo_bound PLUS b_term  */
#line 426 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3076 "pascal.tab.c"
    break;

  case 132: /* lo_bound: lo_bound MINUS b_term  */
#line 427 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3082 "pascal.tab.c"
    break;

  case 133: /* lo_term: scalar_constant  */
#line 430 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3088 "pascal.tab.c"
    break;

  case 134: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 431 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3094 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term MUL b_factor  */
#line 432 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3100 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term IDIV b_factor  */
#line 433 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3106 "pascal.tab.c"
    break;

  case 137: /* lo_term: lo_term IMOD b_factor  */
#line 434 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3112 "pascal.tab.c"
    break;

  case 138: /* lo_term: lo_term SHIFTOP b_factor  */
#line 435 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3118 "pascal.tab.c"
    break;

  case 139: /* b_expr: b_term  */
#line 438 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3124 "pascal.tab.c"
    break;

  case 140: /* b_expr: PLUS b_term  */
#line 439 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3130 "pascal.tab.c"
    break;

  case 141: /* b_expr: MINUS b_term  */
#line 440 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3136 "pascal.tab.c"
    break;

  case 142: /* b_expr: b_expr PLUS b_term  */
#line 441 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3142 "pascal.tab.c"
    break;

  case 143: /* b_expr: b_expr MINUS b_term  */
#line 442 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3148 "pascal.tab.c"
    break;

  case 144: /* b_term: b_factor  */
#line 445 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3154 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term MUL b_factor  */
#line 446 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3160 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term IDIV b_factor  */
#line 447 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3166 "pascal.tab.c"
    break;

  case 147: /* b_term: b_term IMOD b_factor  */
#line 448 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3172 "pascal.tab.c"
    break;

  case 148: /* b_term: b_term SHIFTOP b_factor  */
#line 449 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3178 "pascal.tab.c"
    break;

  case 149: /* b_factor: scalar_constant  */
#line 452 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3184 "pascal.tab.c"
    break;

  case 150: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 453 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3190 "pascal.tab.c"
    break;

  case 151: /* b_factor: LPARENT b_expr RPARENT  */
#line 454 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3196 "pascal.tab.c"
    break;

  case 152: /* record_body: record_field_list record_case_opt  */
#line 457 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3202 "pascal.tab.c"
    break;

  case 153: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 460 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3208 "pascal.tab.c"
    break;

  case 154: /* record_field_list: record_field  */
#line 461 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3214 "pascal.tab.c"
    break;

  case 155: /* $@4: %empty  */
#line 464 "pascal.y"
                  { }
#line 3220 "pascal.tab.c"
    break;

  case 156: /* record_field: id_list COLON $@4 type  */
#line 464 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3226 "pascal.tab.c"
    break;

  case 157: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 465 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3232 "pascal.tab.c"
    break;

  case 158: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 466 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3238 "pascal.tab.c"
    break;

  case 159: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 467 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3244 "pascal.tab.c"
    break;

  case 160: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 468 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3250 "pascal.tab.c"
    break;

  case 161: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 469 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3256 "pascal.tab.c"
    break;

  case 162: /* record_field: IDENT record_field  */
#line 470 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3262 "pascal.tab.c"
    break;

  case 163: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 471 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3268 "pascal.tab.c"
    break;

  case 164: /* record_field: %empty  */
#line 472 "pascal.y"
     { (yyval.node) = NULL; }
#line 3274 "pascal.tab.c"
    break;

  case 165: /* case_arm_mark: %empty  */
#line 475 "pascal.y"
    { }
#line 3280 "pascal.tab.c"
    break;

  case 166: /* case_open: %empty  */
#line 478 "pascal.y"
    { }
#line 3286 "pascal.tab.c"
    break;

  case 167: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 481 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3292 "pascal.tab.c"
    break;

  case 168: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 482 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3298 "pascal.tab.c"
    break;

  case 169: /* record_case_opt: %empty  */
#line 483 "pascal.y"
     { (yyval.node) = NULL; }
#line 3304 "pascal.tab.c"
    break;

  case 170: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 486 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3310 "pascal.tab.c"
    break;

  case 171: /* record_case_list: record_case_arm  */
#line 487 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3316 "pascal.tab.c"
    break;

  case 172: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 490 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3322 "pascal.tab.c"
    break;

  case 173: /* record_case_arm: %empty  */
#line 491 "pascal.y"
     { (yyval.node) = NULL; }
#line 3328 "pascal.tab.c"
    break;

  case 174: /* var_decl_list: var_decl_list var_decl  */
#line 494 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3334 "pascal.tab.c"
    break;

  case 175: /* var_decl_list: var_decl  */
#line 495 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3340 "pascal.tab.c"
    break;

  case 176: /* var_decl: id_list COLON type SEMICOLON  */
#line 498 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3346 "pascal.tab.c"
    break;

  case 177: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 499 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3352 "pascal.tab.c"
    break;

  case 178: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 500 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3358 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 503 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3364 "pascal.tab.c"
    break;

  case 180: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 504 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3370 "pascal.tab.c"
    break;

  case 181: /* $@5: %empty  */
#line 505 "pascal.y"
                                                                               { }
#line 3376 "pascal.tab.c"
    break;

  case 182: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 505 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3382 "pascal.tab.c"
    break;

  case 183: /* $@6: %empty  */
#line 506 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3388 "pascal.tab.c"
    break;

  case 184: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 506 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3394 "pascal.tab.c"
    break;

  case 185: /* $@7: %empty  */
#line 507 "pascal.y"
                                        { }
#line 3400 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 507 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3406 "pascal.tab.c"
    break;

  case 187: /* $@8: %empty  */
#line 508 "pascal.y"
                                                                                                           { }
#line 3412 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 508 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3418 "pascal.tab.c"
    break;

  case 189: /* $@9: %empty  */
#line 509 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3424 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 509 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3430 "pascal.tab.c"
    break;

  case 191: /* $@10: %empty  */
#line 510 "pascal.y"
                                                                                                 { }
#line 3436 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3442 "pascal.tab.c"
    break;

  case 193: /* $@11: %empty  */
#line 511 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3448 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 511 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3454 "pascal.tab.c"
    break;

  case 195: /* $@12: %empty  */
#line 512 "pascal.y"
                                                                                            { }
#line 3460 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 512 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3466 "pascal.tab.c"
    break;

  case 197: /* $@13: %empty  */
#line 513 "pascal.y"
                                                                                                             { }
#line 3472 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3480 "pascal.tab.c"
    break;

  case 199: /* $@14: %empty  */
#line 516 "pascal.y"
                                                                                              { }
#line 3486 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3494 "pascal.tab.c"
    break;

  case 201: /* $@15: %empty  */
#line 519 "pascal.y"
                                                                                             { }
#line 3500 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3508 "pascal.tab.c"
    break;

  case 203: /* $@16: %empty  */
#line 522 "pascal.y"
                                                                                                    { }
#line 3514 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3522 "pascal.tab.c"
    break;

  case 205: /* $@17: %empty  */
#line 525 "pascal.y"
                                                                                                                { }
#line 3528 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 525 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3537 "pascal.tab.c"
    break;

  case 207: /* $@18: %empty  */
#line 529 "pascal.y"
                                                                                                                  { }
#line 3543 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 529 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3552 "pascal.tab.c"
    break;

  case 209: /* $@19: %empty  */
#line 533 "pascal.y"
                                                                                                                     { }
#line 3558 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 533 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3566 "pascal.tab.c"
    break;

  case 211: /* pv_mark: %empty  */
#line 538 "pascal.y"
    { }
#line 3572 "pascal.tab.c"
    break;

  case 212: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 541 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3578 "pascal.tab.c"
    break;

  case 213: /* parameter_list_opt: LPARENT RPARENT  */
#line 542 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3584 "pascal.tab.c"
    break;

  case 214: /* parameter_list_opt: %empty  */
#line 543 "pascal.y"
     { (yyval.node) = NULL; }
#line 3590 "pascal.tab.c"
    break;

  case 215: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 546 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3596 "pascal.tab.c"
    break;

  case 216: /* parameter_decl_list: parameter_decl  */
#line 547 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3602 "pascal.tab.c"
    break;

  case 217: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 550 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3608 "pascal.tab.c"
    break;

  case 218: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 551 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3614 "pascal.tab.c"
    break;

  case 219: /* parameter_decl: VARSY id_list COLON param_type  */
#line 552 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3620 "pascal.tab.c"
    break;

  case 220: /* parameter_decl: id_list COLON param_type  */
#line 553 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3626 "pascal.tab.c"
    break;

  case 221: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 554 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3632 "pascal.tab.c"
    break;

  case 222: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 555 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3638 "pascal.tab.c"
    break;

  case 223: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 556 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3644 "pascal.tab.c"
    break;

  case 224: /* parameter_decl: IDENT id_list COLON param_type  */
#line 557 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3650 "pascal.tab.c"
    break;

  case 225: /* param_type: IDENT  */
#line 560 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3656 "pascal.tab.c"
    break;

  case 226: /* param_type: IDENT PERIOD IDENT  */
#line 561 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3662 "pascal.tab.c"
    break;

  case 227: /* param_type: ARRAYSY OFSY IDENT  */
#line 562 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3668 "pascal.tab.c"
    break;

  case 228: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 563 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3674 "pascal.tab.c"
    break;

  case 229: /* param_type: spec_ref  */
#line 564 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3680 "pascal.tab.c"
    break;

  case 230: /* routine_directives: %empty  */
#line 567 "pascal.y"
    { (yyval.node) = NULL; }
#line 3686 "pascal.tab.c"
    break;

  case 231: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 568 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3699 "pascal.tab.c"
    break;

  case 232: /* pf_params: LPARENT pf_sections RPARENT  */
#line 578 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3705 "pascal.tab.c"
    break;

  case 233: /* pf_params: LPARENT RPARENT  */
#line 579 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3711 "pascal.tab.c"
    break;

  case 234: /* pf_params: %empty  */
#line 580 "pascal.y"
     { (yyval.node) = NULL; }
#line 3717 "pascal.tab.c"
    break;

  case 235: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 583 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3723 "pascal.tab.c"
    break;

  case 236: /* pf_sections: pf_section  */
#line 584 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3729 "pascal.tab.c"
    break;

  case 237: /* pf_section: id_list COLON param_type  */
#line 587 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3735 "pascal.tab.c"
    break;

  case 238: /* pf_section: VARSY id_list COLON param_type  */
#line 588 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3741 "pascal.tab.c"
    break;

  case 239: /* pf_section: CONSTSY id_list COLON param_type  */
#line 589 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3747 "pascal.tab.c"
    break;

  case 240: /* pf_section: IDENT id_list COLON param_type  */
#line 590 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3753 "pascal.tab.c"
    break;

  case 241: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 591 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3759 "pascal.tab.c"
    break;

  case 242: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 592 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3765 "pascal.tab.c"
    break;

  case 243: /* enum_list: enum_list COMMA enum_member  */
#line 595 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3771 "pascal.tab.c"
    break;

  case 244: /* enum_list: enum_member  */
#line 596 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3777 "pascal.tab.c"
    break;

  case 245: /* enum_member: IDENT  */
#line 599 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3783 "pascal.tab.c"
    break;

  case 246: /* id_list: id_list COMMA IDENT  */
#line 602 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3789 "pascal.tab.c"
    break;

  case 247: /* id_list: IDENT  */
#line 603 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3795 "pascal.tab.c"
    break;

  case 248: /* body: BEGINSY statement_list ENDSY  */
#line 606 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3801 "pascal.tab.c"
    break;

  case 249: /* $@20: %empty  */
#line 609 "pascal.y"
                             { pas_stmt_mark(); }
#line 3807 "pascal.tab.c"
    break;

  case 250: /* statement_list: statement_list SEMICOLON $@20 statement  */
#line 609 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3813 "pascal.tab.c"
    break;

  case 251: /* $@21: %empty  */
#line 610 "pascal.y"
     { pas_stmt_mark(); }
#line 3819 "pascal.tab.c"
    break;

  case 252: /* statement_list: $@21 statement  */
#line 610 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3825 "pascal.tab.c"
    break;

  case 253: /* $@22: %empty  */
#line 613 "pascal.y"
    { pas_stmt_mark(); }
#line 3831 "pascal.tab.c"
    break;

  case 254: /* body_stmt: $@22 statement  */
#line 613 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3837 "pascal.tab.c"
    break;

  case 255: /* statement: statement_no_label  */
#line 616 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3843 "pascal.tab.c"
    break;

  case 256: /* statement: INTCONST COLON statement_no_label  */
#line 617 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3849 "pascal.tab.c"
    break;

  case 257: /* statement: IDENT COLON statement_no_label  */
#line 618 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3855 "pascal.tab.c"
    break;

  case 258: /* statement_no_label: assignment  */
#line 621 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3861 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: call  */
#line 622 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3867 "pascal.tab.c"
    break;

  case 260: /* statement_no_label: compound_statement  */
#line 623 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3873 "pascal.tab.c"
    break;

  case 261: /* statement_no_label: goto_statement  */
#line 624 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3879 "pascal.tab.c"
    break;

  case 262: /* statement_no_label: if_statement  */
#line 625 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3885 "pascal.tab.c"
    break;

  case 263: /* statement_no_label: case_statement  */
#line 626 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3891 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: while_statement  */
#line 627 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3897 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: repeat_statement  */
#line 628 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3903 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: for_statement  */
#line 629 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3909 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: with_statement  */
#line 630 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3915 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: try_statement  */
#line 631 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3921 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: raise_statement  */
#line 632 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3927 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: inherited_call  */
#line 633 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3933 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: INHERITEDSY  */
#line 634 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3939 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: %empty  */
#line 635 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3945 "pascal.tab.c"
    break;

  case 273: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 638 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3951 "pascal.tab.c"
    break;

  case 274: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 639 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3957 "pascal.tab.c"
    break;

  case 275: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 640 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3963 "pascal.tab.c"
    break;

  case 276: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 641 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3969 "pascal.tab.c"
    break;

  case 277: /* handler_list: handler  */
#line 644 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3975 "pascal.tab.c"
    break;

  case 278: /* handler_list: handler_list SEMICOLON handler  */
#line 645 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3981 "pascal.tab.c"
    break;

  case 279: /* handler_list: handler_list SEMICOLON  */
#line 646 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3987 "pascal.tab.c"
    break;

  case 280: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 649 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3993 "pascal.tab.c"
    break;

  case 281: /* handler: ONSY IDENT DOSY body_stmt  */
#line 650 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3999 "pascal.tab.c"
    break;

  case 282: /* raise_statement: RAISESY  */
#line 653 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4005 "pascal.tab.c"
    break;

  case 283: /* raise_statement: RAISESY expression  */
#line 654 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4011 "pascal.tab.c"
    break;

  case 284: /* inherited_call: INHERITEDSY IDENT  */
#line 657 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4017 "pascal.tab.c"
    break;

  case 285: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 658 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4023 "pascal.tab.c"
    break;

  case 286: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 659 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4029 "pascal.tab.c"
    break;

  case 287: /* call: IDENT  */
#line 662 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4035 "pascal.tab.c"
    break;

  case 288: /* call: spec_expr  */
#line 663 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4041 "pascal.tab.c"
    break;

  case 289: /* call: selector PERIOD IDENT  */
#line 664 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4047 "pascal.tab.c"
    break;

  case 290: /* call: call_with_args  */
#line 665 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4053 "pascal.tab.c"
    break;

  case 291: /* recv_primary: STRINGCONST  */
#line 668 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4059 "pascal.tab.c"
    break;

  case 292: /* recv_primary: LPARENT expression RPARENT  */
#line 669 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4065 "pascal.tab.c"
    break;

  case 293: /* recv_primary: spec_expr  */
#line 670 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4071 "pascal.tab.c"
    break;

  case 294: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 673 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4077 "pascal.tab.c"
    break;

  case 295: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 674 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4083 "pascal.tab.c"
    break;

  case 296: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 675 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4089 "pascal.tab.c"
    break;

  case 297: /* call_with_args: IDENT LPARENT RPARENT  */
#line 676 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4095 "pascal.tab.c"
    break;

  case 298: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 677 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4101 "pascal.tab.c"
    break;

  case 299: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 678 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4107 "pascal.tab.c"
    break;

  case 300: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 679 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4113 "pascal.tab.c"
    break;

  case 301: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 680 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4119 "pascal.tab.c"
    break;

  case 302: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 681 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4125 "pascal.tab.c"
    break;

  case 303: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 682 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4131 "pascal.tab.c"
    break;

  case 304: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 683 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4137 "pascal.tab.c"
    break;

  case 305: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 684 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4143 "pascal.tab.c"
    break;

  case 306: /* argument_list: argument_list COMMA argument  */
#line 687 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4149 "pascal.tab.c"
    break;

  case 307: /* argument_list: argument  */
#line 688 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4155 "pascal.tab.c"
    break;

  case 308: /* argument: expression  */
#line 691 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4161 "pascal.tab.c"
    break;

  case 309: /* argument: expression COLON expression  */
#line 692 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4167 "pascal.tab.c"
    break;

  case 310: /* argument: expression COLON expression COLON expression  */
#line 693 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4173 "pascal.tab.c"
    break;

  case 311: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 694 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4179 "pascal.tab.c"
    break;

  case 312: /* assignment: selector BECOMES expression  */
#line 697 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4185 "pascal.tab.c"
    break;

  case 313: /* assignment: selector ADDBECOMES expression  */
#line 698 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4191 "pascal.tab.c"
    break;

  case 314: /* assignment: selector SUBBECOMES expression  */
#line 699 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4197 "pascal.tab.c"
    break;

  case 315: /* assignment: selector MULBECOMES expression  */
#line 700 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4203 "pascal.tab.c"
    break;

  case 316: /* assignment: selector DIVBECOMES expression  */
#line 701 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4209 "pascal.tab.c"
    break;

  case 317: /* assignment: call_with_args BECOMES expression  */
#line 702 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4215 "pascal.tab.c"
    break;

  case 318: /* selector: selector LBRACK expression_list RBRACK  */
#line 705 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4221 "pascal.tab.c"
    break;

  case 319: /* selector: selector PERIOD IDENT  */
#line 706 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4227 "pascal.tab.c"
    break;

  case 320: /* selector: selector ARROW  */
#line 707 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4233 "pascal.tab.c"
    break;

  case 321: /* selector: call_with_args PERIOD IDENT  */
#line 708 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4239 "pascal.tab.c"
    break;

  case 322: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 709 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4245 "pascal.tab.c"
    break;

  case 323: /* selector: call_with_args ARROW  */
#line 710 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4251 "pascal.tab.c"
    break;

  case 324: /* selector: recv_primary PERIOD IDENT  */
#line 711 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4257 "pascal.tab.c"
    break;

  case 325: /* selector: recv_primary ARROW  */
#line 712 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4263 "pascal.tab.c"
    break;

  case 326: /* selector: IDENT  */
#line 713 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4269 "pascal.tab.c"
    break;

  case 327: /* expression_list: expression_list COMMA expression  */
#line 716 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4275 "pascal.tab.c"
    break;

  case 328: /* expression_list: expression  */
#line 717 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4281 "pascal.tab.c"
    break;

  case 329: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 720 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4287 "pascal.tab.c"
    break;

  case 330: /* goto_statement: GOTOSY INTCONST  */
#line 723 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4293 "pascal.tab.c"
    break;

  case 331: /* goto_statement: GOTOSY IDENT  */
#line 724 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4299 "pascal.tab.c"
    break;

  case 332: /* if_statement: IFSY expression THENSY body_stmt  */
#line 727 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4305 "pascal.tab.c"
    break;

  case 333: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 728 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4311 "pascal.tab.c"
    break;

  case 334: /* $@23: %empty  */
#line 731 "pascal.y"
                           { }
#line 4317 "pascal.tab.c"
    break;

  case 335: /* case_statement: CASESY expression OFSY $@23 case_list case_tail ENDSY  */
#line 731 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4323 "pascal.tab.c"
    break;

  case 336: /* case_tail: %empty  */
#line 734 "pascal.y"
    { (yyval.node) = NULL; }
#line 4329 "pascal.tab.c"
    break;

  case 337: /* case_tail: ELSESY statement_list  */
#line 735 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4335 "pascal.tab.c"
    break;

  case 338: /* case_tail: OTHERWISESY statement_list  */
#line 736 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4341 "pascal.tab.c"
    break;

  case 339: /* case_list: case_list SEMICOLON case_elem  */
#line 739 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4347 "pascal.tab.c"
    break;

  case 340: /* case_list: case_elem  */
#line 740 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4353 "pascal.tab.c"
    break;

  case 341: /* case_elem: constant_list COLON body_stmt  */
#line 743 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4359 "pascal.tab.c"
    break;

  case 342: /* case_elem: %empty  */
#line 744 "pascal.y"
     { (yyval.node) = NULL; }
#line 4365 "pascal.tab.c"
    break;

  case 343: /* constant_list: constant_list COMMA case_label  */
#line 747 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4371 "pascal.tab.c"
    break;

  case 344: /* constant_list: case_label  */
#line 748 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4377 "pascal.tab.c"
    break;

  case 345: /* case_label: b_expr  */
#line 751 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4383 "pascal.tab.c"
    break;

  case 346: /* case_label: b_expr DOTDOT b_expr  */
#line 752 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4389 "pascal.tab.c"
    break;

  case 347: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 755 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4395 "pascal.tab.c"
    break;

  case 348: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 758 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4401 "pascal.tab.c"
    break;

  case 349: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 761 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4407 "pascal.tab.c"
    break;

  case 350: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 762 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4413 "pascal.tab.c"
    break;

  case 351: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 763 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4419 "pascal.tab.c"
    break;

  case 352: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 766 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4425 "pascal.tab.c"
    break;

  case 353: /* with_open: with_open COMMA selector  */
#line 769 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4431 "pascal.tab.c"
    break;

  case 354: /* with_open: selector  */
#line 770 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4437 "pascal.tab.c"
    break;

  case 355: /* expression: simple_expression  */
#line 777 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4443 "pascal.tab.c"
    break;

  case 356: /* expression: expression INOP simple_expression  */
#line 778 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4449 "pascal.tab.c"
    break;

  case 357: /* expression: expression LTOP simple_expression  */
#line 779 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4455 "pascal.tab.c"
    break;

  case 358: /* expression: expression LEOP simple_expression  */
#line 780 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4461 "pascal.tab.c"
    break;

  case 359: /* expression: expression GTOP simple_expression  */
#line 781 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4467 "pascal.tab.c"
    break;

  case 360: /* expression: expression GEOP simple_expression  */
#line 782 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4473 "pascal.tab.c"
    break;

  case 361: /* expression: expression NEOP simple_expression  */
#line 783 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4479 "pascal.tab.c"
    break;

  case 362: /* expression: expression EQOP simple_expression  */
#line 784 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4485 "pascal.tab.c"
    break;

  case 363: /* expression: expression ISOP simple_expression  */
#line 785 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4491 "pascal.tab.c"
    break;

  case 364: /* simple_expression: term  */
#line 788 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4497 "pascal.tab.c"
    break;

  case 365: /* simple_expression: PLUS term  */
#line 789 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4503 "pascal.tab.c"
    break;

  case 366: /* simple_expression: MINUS term  */
#line 790 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4509 "pascal.tab.c"
    break;

  case 367: /* simple_expression: simple_expression PLUS term  */
#line 791 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4515 "pascal.tab.c"
    break;

  case 368: /* simple_expression: simple_expression MINUS term  */
#line 792 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4521 "pascal.tab.c"
    break;

  case 369: /* simple_expression: simple_expression OROP term  */
#line 793 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4527 "pascal.tab.c"
    break;

  case 370: /* simple_expression: simple_expression XOROP term  */
#line 794 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4533 "pascal.tab.c"
    break;

  case 371: /* term: power  */
#line 797 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4539 "pascal.tab.c"
    break;

  case 372: /* term: term MUL power  */
#line 798 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4545 "pascal.tab.c"
    break;

  case 373: /* term: term RDIV power  */
#line 799 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4551 "pascal.tab.c"
    break;

  case 374: /* term: term IDIV power  */
#line 800 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4557 "pascal.tab.c"
    break;

  case 375: /* term: term IMOD power  */
#line 801 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4563 "pascal.tab.c"
    break;

  case 376: /* term: term ANDOP power  */
#line 802 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4569 "pascal.tab.c"
    break;

  case 377: /* term: term MUL MINUS power  */
#line 803 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4575 "pascal.tab.c"
    break;

  case 378: /* term: term RDIV MINUS power  */
#line 804 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4581 "pascal.tab.c"
    break;

  case 379: /* term: term IDIV MINUS power  */
#line 805 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4587 "pascal.tab.c"
    break;

  case 380: /* term: term IMOD MINUS power  */
#line 806 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4593 "pascal.tab.c"
    break;

  case 381: /* term: term ANDOP MINUS power  */
#line 807 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4599 "pascal.tab.c"
    break;

  case 382: /* term: term SHIFTOP power  */
#line 808 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4605 "pascal.tab.c"
    break;

  case 383: /* term: term ASOP power  */
#line 809 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4611 "pascal.tab.c"
    break;

  case 384: /* power: factor  */
#line 812 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4617 "pascal.tab.c"
    break;

  case 385: /* power: power POWOP factor  */
#line 813 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4623 "pascal.tab.c"
    break;

  case 386: /* factor: selector  */
#line 816 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4629 "pascal.tab.c"
    break;

  case 387: /* factor: call_with_args  */
#line 817 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4635 "pascal.tab.c"
    break;

  case 388: /* factor: INTCONST  */
#line 818 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4641 "pascal.tab.c"
    break;

  case 389: /* factor: REALCONST  */
#line 819 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4647 "pascal.tab.c"
    break;

  case 390: /* factor: STRINGCONST  */
#line 820 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4653 "pascal.tab.c"
    break;

  case 391: /* factor: CHARCODE  */
#line 821 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4659 "pascal.tab.c"
    break;

  case 392: /* factor: LPARENT expression RPARENT  */
#line 822 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4665 "pascal.tab.c"
    break;

  case 393: /* factor: inherited_call  */
#line 823 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4671 "pascal.tab.c"
    break;

  case 394: /* factor: NOTSY factor  */
#line 824 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4677 "pascal.tab.c"
    break;

  case 395: /* factor: ATSIGN factor  */
#line 825 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4683 "pascal.tab.c"
    break;

  case 396: /* factor: LBRACK RBRACK  */
#line 826 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4689 "pascal.tab.c"
    break;

  case 397: /* factor: LBRACK set_member_list RBRACK  */
#line 827 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4695 "pascal.tab.c"
    break;

  case 398: /* set_member_list: set_member  */
#line 830 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4701 "pascal.tab.c"
    break;

  case 399: /* set_member_list: set_member_list COMMA set_member  */
#line 831 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4707 "pascal.tab.c"
    break;

  case 400: /* set_member: expression  */
#line 834 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4713 "pascal.tab.c"
    break;

  case 401: /* set_member: expression DOTDOT expression  */
#line 835 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4719 "pascal.tab.c"
    break;


#line 4723 "pascal.tab.c"

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

#line 841 "pascal.y"

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
