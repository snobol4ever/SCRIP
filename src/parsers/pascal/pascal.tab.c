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
  YYSYMBOL_pv_mark = 173,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 174,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 175,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 176,           /* parameter_decl  */
  YYSYMBOL_param_type = 177,               /* param_type  */
  YYSYMBOL_routine_directives = 178,       /* routine_directives  */
  YYSYMBOL_pf_params = 179,                /* pf_params  */
  YYSYMBOL_pf_sections = 180,              /* pf_sections  */
  YYSYMBOL_pf_section = 181,               /* pf_section  */
  YYSYMBOL_enum_list = 182,                /* enum_list  */
  YYSYMBOL_enum_member = 183,              /* enum_member  */
  YYSYMBOL_id_list = 184,                  /* id_list  */
  YYSYMBOL_body = 185,                     /* body  */
  YYSYMBOL_statement_list = 186,           /* statement_list  */
  YYSYMBOL_187_20 = 187,                   /* $@20  */
  YYSYMBOL_188_21 = 188,                   /* $@21  */
  YYSYMBOL_body_stmt = 189,                /* body_stmt  */
  YYSYMBOL_190_22 = 190,                   /* $@22  */
  YYSYMBOL_statement = 191,                /* statement  */
  YYSYMBOL_statement_no_label = 192,       /* statement_no_label  */
  YYSYMBOL_try_statement = 193,            /* try_statement  */
  YYSYMBOL_handler_list = 194,             /* handler_list  */
  YYSYMBOL_handler = 195,                  /* handler  */
  YYSYMBOL_raise_statement = 196,          /* raise_statement  */
  YYSYMBOL_inherited_call = 197,           /* inherited_call  */
  YYSYMBOL_call = 198,                     /* call  */
  YYSYMBOL_recv_primary = 199,             /* recv_primary  */
  YYSYMBOL_call_with_args = 200,           /* call_with_args  */
  YYSYMBOL_argument_list = 201,            /* argument_list  */
  YYSYMBOL_argument = 202,                 /* argument  */
  YYSYMBOL_assignment = 203,               /* assignment  */
  YYSYMBOL_selector = 204,                 /* selector  */
  YYSYMBOL_expression_list = 205,          /* expression_list  */
  YYSYMBOL_compound_statement = 206,       /* compound_statement  */
  YYSYMBOL_goto_statement = 207,           /* goto_statement  */
  YYSYMBOL_if_statement = 208,             /* if_statement  */
  YYSYMBOL_case_statement = 209,           /* case_statement  */
  YYSYMBOL_210_23 = 210,                   /* $@23  */
  YYSYMBOL_case_tail = 211,                /* case_tail  */
  YYSYMBOL_case_list = 212,                /* case_list  */
  YYSYMBOL_case_elem = 213,                /* case_elem  */
  YYSYMBOL_constant_list = 214,            /* constant_list  */
  YYSYMBOL_case_label = 215,               /* case_label  */
  YYSYMBOL_while_statement = 216,          /* while_statement  */
  YYSYMBOL_repeat_statement = 217,         /* repeat_statement  */
  YYSYMBOL_for_statement = 218,            /* for_statement  */
  YYSYMBOL_with_statement = 219,           /* with_statement  */
  YYSYMBOL_with_open = 220,                /* with_open  */
  YYSYMBOL_expression = 221,               /* expression  */
  YYSYMBOL_simple_expression = 222,        /* simple_expression  */
  YYSYMBOL_term = 223,                     /* term  */
  YYSYMBOL_power = 224,                    /* power  */
  YYSYMBOL_factor = 225,                   /* factor  */
  YYSYMBOL_set_member_list = 226,          /* set_member_list  */
  YYSYMBOL_set_member = 227                /* set_member  */
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
#define YYLAST   1894

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  407
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  981

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
     480,   483,   486,   489,   490,   491,   494,   495,   498,   499,
     502,   503,   506,   507,   508,   511,   512,   513,   513,   514,
     514,   515,   515,   516,   516,   517,   517,   518,   518,   519,
     519,   520,   520,   521,   521,   524,   524,   527,   527,   530,
     530,   533,   533,   537,   537,   541,   541,   546,   549,   550,
     551,   554,   555,   558,   559,   560,   561,   562,   563,   564,
     565,   568,   569,   570,   571,   572,   575,   576,   586,   587,
     588,   591,   592,   595,   596,   597,   598,   599,   600,   603,
     604,   607,   610,   611,   614,   617,   617,   618,   618,   621,
     621,   624,   625,   626,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   646,
     647,   648,   649,   652,   653,   654,   657,   658,   661,   662,
     665,   666,   667,   670,   671,   672,   673,   676,   677,   678,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   695,   696,   699,   700,   701,   702,   705,   706,
     707,   708,   709,   710,   713,   714,   715,   716,   717,   718,
     719,   720,   721,   724,   725,   728,   731,   732,   735,   736,
     739,   739,   742,   743,   744,   747,   748,   751,   752,   755,
     756,   759,   760,   763,   766,   769,   770,   771,   774,   777,
     778,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     796,   797,   798,   799,   800,   801,   802,   805,   806,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   817,
     820,   821,   824,   825,   826,   827,   828,   829,   830,   831,
     832,   833,   834,   835,   838,   839,   842,   843
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
  "$@17", "$@18", "$@19", "pv_mark", "parameter_list_opt",
  "parameter_decl_list", "parameter_decl", "param_type",
  "routine_directives", "pf_params", "pf_sections", "pf_section",
  "enum_list", "enum_member", "id_list", "body", "statement_list", "$@20",
  "$@21", "body_stmt", "$@22", "statement", "statement_no_label",
  "try_statement", "handler_list", "handler", "raise_statement",
  "inherited_call", "call", "recv_primary", "call_with_args",
  "argument_list", "argument", "assignment", "selector", "expression_list",
  "compound_statement", "goto_statement", "if_statement", "case_statement",
  "$@23", "case_tail", "case_list", "case_elem", "constant_list",
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

#define YYPACT_NINF (-561)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-333)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     290,     5,    85,    95,  -561,   173,   910,  -561,  -561,   359,
    -561,    43,  -561,  -561,   437,   135,    85,   191,   227,   227,
     312,  -561,   -10,   168,   361,   374,   109,  -561,  -561,  -561,
     390,   227,   301,   379,   396,  -561,  -561,    55,   422,   135,
    -561,   142,   419,   420,  -561,   227,  -561,   195,   154,   469,
      63,   988,   573,   409,   -10,  -561,   412,   415,   421,   468,
     481,   473,   518,  -561,   442,  -561,  -561,  -561,  -561,   534,
    1423,   994,  -561,  -561,    85,   227,   532,   501,  -561,   553,
     994,  -561,   227,   562,    60,  -561,  -561,   568,   584,  -561,
    1423,   325,  1423,  -561,  1423,  1423,   525,  -561,   899,  -561,
    1423,   588,   592,  -561,   595,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,   266,   292,  -561,  1797,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   227,  1100,   624,  -561,   523,   627,
     640,   632,   646,   655,   680,  -561,   670,  1847,  -561,  -561,
    1635,  1635,  1635,  1194,  1423,  1635,  -561,  -561,  -561,   349,
     156,   588,   669,  -561,   768,   777,   223,   360,   417,   657,
    -561,   636,   673,  -561,   673,   736,   736,   184,   666,  -561,
    -561,  -561,  -561,  1512,   187,   108,  -561,  -561,  -561,  -561,
     694,   474,  -561,   350,   255,   419,   160,  -561,   120,   746,
    -561,    46,   367,  -561,  -561,   718,   988,  -561,  -561,   608,
     265,  1048,   768,   777,    71,  1193,   201,   668,  1676,   162,
     162,  1221,   771,    34,  1800,   697,   729,  1259,   705,  -561,
    1423,   708,  -561,  1423,  1284,  1423,   714,  -561,  1423,  1423,
    1423,  1423,  1423,   434,    74,    54,   796,   227,   743,   744,
      24,  -561,  -561,   227,   227,  -561,   135,    85,  -561,   752,
     227,   764,   -10,  -561,  -561,   417,   417,  -561,  1694,   579,
    -561,  1713,  -561,   765,  -561,  1423,  1423,  1423,  1423,  1423,
    1423,  1423,  1423,  1635,  1635,  1635,  1635,  1518,  1543,  1568,
    1593,  1618,  1635,  1635,  1635,   776,   245,  -561,   222,   824,
     805,   804,   255,   255,     8,  -561,  -561,   464,  -561,   771,
     798,   700,   700,   820,   245,    50,   825,   826,   832,   533,
     833,   835,   839,   848,   234,  -561,   364,  1450,   104,   863,
     871,  -561,   528,   528,   700,   528,   528,   528,   528,  -561,
     501,   227,   840,   227,   843,  -561,   227,    44,  -561,   685,
    -561,  -561,  1450,   845,  -561,   501,  -561,   314,  -561,  1423,
    1423,  1423,  -561,  -561,   325,  -561,  -561,  -561,  -561,  1143,
    -561,  -561,  -561,   493,  -561,   794,  1731,    52,  -561,   808,
    -561,  1309,   771,  -561,   503,   859,  1800,   860,   612,  1800,
    -561,   505,  1800,  1357,   613,  1800,  1800,  1800,  1800,   872,
    -561,  -561,   245,  -561,   454,  -561,  -561,  -561,  -561,   501,
     501,   483,   489,   135,   159,   896,  -561,   227,  -561,   -10,
    1423,  1423,  -561,   701,   865,   360,   360,   360,   360,   360,
     360,   360,   360,   417,   417,   417,   417,  1635,   657,  1635,
     657,  1635,   657,  1635,   657,  1635,   657,   657,   657,  -561,
     867,     8,   506,  -561,  -561,   307,   227,   858,   227,   862,
    -561,   227,    58,  -561,   727,   868,   314,   666,  -561,   495,
    -561,   528,   528,   700,   875,  -561,   397,   302,  -561,   348,
    -561,   509,   877,  -561,   879,    73,   303,   880,   881,   886,
    -561,   739,  -561,  -561,   876,  -561,   171,  -561,  -561,  -561,
     305,   888,  -561,  -561,  1477,   938,  1800,   994,   771,   994,
     771,   305,   302,   302,   313,  -561,  -561,  -561,  -561,   501,
     943,   801,   673,   802,   673,   815,   169,  -561,    30,    36,
     946,   961,   501,   911,   967,   165,  -561,   972,   717,  1089,
    1800,  -561,   988,  -561,   777,   700,   937,  1423,  -561,  1423,
    1423,   771,  -561,   903,   203,   275,  -561,   315,  -561,   522,
     496,  -561,  1334,  1369,  1423,  -561,  -561,  1396,  -561,   994,
     547,   931,   501,   501,   501,   501,   982,   983,  -561,  -561,
    -561,  -561,  -561,   979,   501,   501,  1800,  -561,   657,   657,
     657,   657,   657,  -561,   245,   934,  -561,   819,   673,   823,
     673,   827,   260,  -561,    30,  -561,   970,  -561,  -561,   302,
     302,   456,   700,   528,   528,  -561,   528,   528,   528,   528,
    -561,  -561,  -561,  -561,  -561,   925,   926,  -561,  -561,   942,
     994,   501,   501,   305,  -561,  -561,   501,   501,  -561,   284,
     994,   253,   952,    72,  1713,  -561,  -561,   634,  -561,  -561,
     981,  1009,  -561,    30,  -561,    30,   955,    30,  -561,   990,
     230,  -561,   963,  1014,  1017,  -561,  -561,  -561,   964,   314,
    -561,   947,  -561,  1423,  1423,  -561,  -561,   377,   277,  -561,
     830,  -561,  -561,  -561,  1747,  1766,  -561,   133,  -561,   808,
    -561,  -561,  -561,  -561,  -561,  -561,   549,  -561,   569,  1800,
    -561,   570,  1021,  1022,   994,  1024,   971,   976,   977,  -561,
    -561,   501,   501,  1027,  1027,  -561,  1008,  1037,   986,  -561,
    -561,    30,  -561,    30,   991,    30,  -561,  -561,   974,  -561,
     461,   302,   302,  -561,  -561,  -561,  -561,   508,   501,   501,
    -561,  -561,   501,   501,   169,   993,  1049,  -561,   996,  1032,
     501,   501,  -561,  -561,  -561,   997,  -561,  1450,  1450,   258,
    -561,   771,  1050,  -561,  -561,  1003,  1031,  -561,   314,  -561,
      35,  1010,  1423,  -561,  -561,  1080,   314,  1083,  -561,    45,
    1136,  1148,  -561,   700,   700,  -561,  -561,  1061,   700,  -561,
    -561,  1423,  1423,  -561,  1016,  -561,   369,  -561,  -561,  -561,
    -561,  -561,  1088,  -561,   314,   314,   314,  1003,  1003,  1090,
    1034,  1027,  1045,  -561,   314,   589,  -561,  -561,   314,  -561,
    -561,  -561,  -561,  1102,  1051,   501,   501,  1103,  1105,   100,
     245,  -561,   314,  -561,  1053,  1055,   700,  1077,  -561,  -561,
     574,  -561,  1058,  1059,  -561,  1003,  -561,  1423,  -561,  -561,
    -561,  -561,  1800,  -561,  1116,  -561,  1118,  -561,  -561,  -561,
     313,  -561,  1027,  1027,  -561,  -561,  -561,  1781,  1800,  -561,
    1111,  -561,  -561,  1003,  1125,  1127,  1128,  -561,  -561,  -561,
     314,  -561,  1003,  1129,  -561,  -561,  -561,   314,  1130,  1084,
    -561,  -561,  -561,  -561,  -561,   314,   314,  1138,  -561,   837,
    -561,  1450,  -561,  1450,  1117,  -561,  -561,  1144,  1800,  -561,
    1003,  -561,  1147,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  1151,  1155,  1003,  1157,  -561,  1158,  -561,   314,    41,
    -561,  -561,   700,  1104,   700,  -561,  -561,   994,   994,  1161,
    -561,  1003,  -561,  -561,  -561,  1162,  1003,  1003,  1003,  -561,
    -561,  -561,  -561,  1003,  -561,  1164,  -561,   576,  -561,  -561,
    1138,  -561,  -561,  -561,  -561,  1166,  -561,  -561,  -561,  -561,
    1172,  1003,  -561,  -561,  -561,   305,  1174,  -561,  1177,  1186,
    1187,  -561,  -561,  1141,  -561,  -561,  -561,  -561,  1188,  -561,
    -561
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   257,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   217,   253,    16,   181,     0,     0,   217,
       0,   278,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   127,    38,    18,     0,     0,     0,   220,   180,     0,
     127,    19,     0,     0,   220,   255,   254,     0,     0,   257,
       0,     0,     0,   257,     0,     0,     0,   297,   293,   257,
     288,   277,     0,    69,   294,    68,   258,   261,   274,   275,
     276,   265,     0,   296,   264,     0,   266,   267,   268,   269,
     270,   271,   272,   273,     0,   127,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   394,   397,   395,   396,
     332,     0,   299,   399,   393,   392,     0,   361,   370,   377,
     390,   107,   240,   126,   240,     0,     0,     0,     0,    51,
      54,    52,    53,   129,   109,   170,   139,    76,    75,   132,
       0,     0,    80,     0,   134,    64,     0,   217,     0,     0,
     252,     0,     0,   217,   191,     0,   278,   336,   337,     0,
       0,     0,     0,   360,     0,     0,     0,     0,     0,   278,
     278,     0,     0,     0,   289,   290,     0,     0,     0,   331,
       0,     0,   329,     0,     0,     0,     0,   326,     0,     0,
       0,     0,     0,     0,   107,   109,     0,     0,     0,     0,
       0,   217,   217,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   400,   371,   372,   402,   406,     0,
     404,     0,   401,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    88,
       0,    50,   135,   136,    81,    83,   251,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,   175,   160,     0,     0,     0,    96,
       0,    93,     0,     0,     0,     0,     0,     0,     0,   217,
     220,     0,     0,     0,     0,   219,   253,     0,   222,     0,
     236,   182,     0,     0,   217,   220,    12,     0,   256,     0,
       0,     0,   259,   259,     0,   340,   335,   259,   298,   293,
     262,   263,   303,     0,   313,   392,   314,     0,    78,   257,
     257,     0,     0,   302,     0,   330,   323,   327,     0,   334,
     305,     0,   318,   295,     0,   319,   320,   321,   322,     0,
      61,    60,     0,    57,     0,   217,   217,   217,   217,   220,
     220,     0,     0,    24,     0,    31,   217,    26,   217,    25,
       0,     0,   403,   398,   325,   362,   363,   364,   365,   366,
     367,   368,   369,   375,   373,   374,   376,     0,   380,     0,
     381,     0,   382,     0,   378,     0,   379,   388,   389,   391,
       0,    72,     0,    70,    73,     0,     0,     0,     0,     0,
     239,   253,     0,   242,     0,     0,     0,     0,   128,     0,
     131,     0,     0,     0,    50,   155,     0,   145,   150,     0,
      82,     0,     0,   100,     0,   111,     0,     0,     0,     0,
     110,     0,   217,   217,     0,   168,     0,   217,   217,    98,
     170,     0,   158,   161,     0,     0,    42,   127,     0,   127,
       0,   170,   137,   138,   133,   142,   143,   141,   144,   220,
       0,     0,   240,     0,   240,     0,     0,   218,     0,   187,
       0,     0,   220,     0,     0,   104,   106,     0,     0,     0,
     354,   353,   278,   358,   359,   348,   338,     0,   300,     0,
       0,     0,    67,     0,     0,     0,   283,     0,   291,     0,
       0,   301,     0,     0,     0,   328,   304,     0,   324,   127,
       0,     0,   220,   220,   220,   220,     0,     0,   217,   217,
      23,   257,   257,    33,   220,   220,   407,   405,   385,   386,
     387,   383,   384,   108,     0,   107,   102,     0,   240,     0,
     240,     0,     0,   238,     0,    89,    90,   249,    77,   146,
     147,     0,     0,     0,     0,   130,     0,     0,     0,     0,
     140,   109,   217,   217,   112,     0,     0,   217,   217,   121,
     127,   220,   220,   170,   217,   217,   220,   220,   159,     0,
     127,   332,     0,     0,    42,    41,    87,     0,    97,    95,
       0,     0,   236,     0,   223,     0,     0,     0,   221,     0,
     231,   235,   226,     0,     0,    12,   183,   184,     0,     0,
     192,     0,   236,     0,     0,   259,   260,   351,   342,   346,
       0,   350,   259,   312,   334,   315,    79,     0,   279,   285,
     280,   257,   282,   292,    74,   311,     0,   309,     0,   333,
     307,     0,     0,   109,   127,     0,     0,     0,     0,   236,
     236,   220,   220,    29,    30,   257,     0,     0,     0,    71,
     109,     0,   247,     0,     0,     0,   241,   243,     0,   157,
       0,   148,   149,   152,   153,   151,   154,     0,   220,   220,
     217,   217,   220,   220,     0,     0,     0,   163,     0,     0,
     220,   220,   165,   166,   172,     0,   162,     0,     0,     0,
      44,     0,     0,    94,   236,   201,   227,   225,     0,   230,
       0,     0,     0,   185,   237,     0,     0,     0,   105,   189,
       0,     0,   357,     0,   348,   257,   257,     0,     0,   259,
     339,     0,     0,   259,     0,   284,     0,   310,   308,   306,
      59,    62,     0,   236,     0,     0,     0,   205,   207,     0,
       0,    32,     0,   236,     0,     0,   245,   244,     0,   246,
      91,   156,   101,     0,     0,   220,   220,     0,     0,     0,
       0,   113,     0,    99,     0,     0,   179,     0,    48,    46,
       0,    45,     0,     0,    84,   197,    12,     0,   224,   234,
     233,   232,   228,   188,     0,   236,     0,    12,   259,   259,
     352,   345,   344,   343,   341,   349,   347,     0,   316,   287,
       0,   281,    58,   209,     0,     0,     0,    12,    12,   236,
       0,    20,    27,     0,   103,   248,   114,     0,     0,     0,
     116,   117,   122,   123,   164,     0,     0,   174,   177,     0,
     172,     0,    43,     0,     0,    85,    12,     0,   229,   236,
     203,   186,     0,   356,   355,   317,   259,    12,   236,   236,
     236,     0,     0,   193,     0,   236,     0,   118,     0,     0,
     169,   167,   179,     0,   179,    47,    49,   127,   127,     0,
     202,   199,    12,   190,   286,     0,   215,   213,   211,   206,
     208,    12,   236,    28,   115,     0,   120,     0,   176,   171,
     173,    92,    86,   198,    12,     0,   210,    12,    12,    12,
       0,   195,   119,   125,   124,   170,     0,   204,     0,     0,
       0,   194,    12,     0,   200,   216,   214,   212,     0,   178,
     196
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -561,  -561,  -561,  -561,  -561,   -65,   900,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   948,   -19,  -336,  -561,  -561,   -58,
     951,   -43,   962,    42,   -70,   -36,  -278,  -554,   -64,   239,
    -257,   -78,  -561,  -561,  -561,   -81,   623,  -234,  -561,  -561,
    -561,  -561,  -203,  -561,   517,  -283,  -294,  -236,  -496,  -561,
    -276,  -561,  -561,   320,  -561,   291,   294,   968,   -42,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,    49,   418,   485,   704,  -560,
     172,  -132,  -561,   630,  -561,   766,    19,  -561,   -85,  -561,
    -561,  -330,  -561,  -175,   502,  -561,  -561,   545,  -561,   -34,
    -561,  -561,   -39,  -193,   688,  -561,   152,  1005,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,   457,  -510,   452,  -561,  -561,
    -561,  -561,  -561,   563,  1622,   -84,  -208,  -105,  -561,   821
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   573,   706,    37,    39,    40,   495,   830,   633,   465,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   895,   928,   501,   527,   287,   305,   480,   735,
     919,   181,   182,   183,   184,   667,   467,   468,   313,   314,
     315,   630,   965,   826,   492,   887,   888,    45,    46,    28,
     655,   847,   346,   941,   972,   896,   954,   836,   932,   867,
     868,   907,   959,   958,   957,    77,   189,   337,   338,   652,
     519,   289,   452,   453,   297,   298,   316,    29,    50,   196,
      51,   531,   532,   106,   107,   108,   545,   546,   109,   153,
     111,   112,   154,   363,   364,   114,   155,   384,   116,   117,
     118,   119,   535,   777,   668,   669,   889,   671,   120,   121,
     122,   123,   204,   366,   157,   158,   159,   160,   259,   260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,    78,   200,   640,   520,   178,   206,   368,
     177,   127,   113,   176,   213,   104,   178,   110,   466,   469,
      72,   348,   176,   533,   374,   670,   471,   536,   502,   503,
     709,   381,   290,   485,   717,   254,   649,    47,    48,    85,
     262,   504,   459,   839,    11,   653,   946,   236,    33,   516,
      64,   341,   202,   445,   846,   177,   299,   255,   256,   391,
      68,   178,   397,   592,    47,   194,    52,   176,    85,   428,
     430,   432,   434,   436,   437,   438,   472,   749,   614,   390,
     473,     8,   303,   756,    53,   757,   474,   759,   353,   505,
     506,   507,   508,    86,   186,    12,   368,   177,    84,   342,
     398,   192,    34,   178,   212,   516,   650,   176,   176,   517,
     541,   840,   654,    69,   560,   550,   185,   947,   392,   369,
     370,   654,   343,   593,   188,   102,   475,   739,   331,   354,
     476,  -253,   497,  -253,   306,    61,   285,   750,   286,   477,
     478,   479,   177,   233,   307,    62,   332,    73,   178,   542,
     783,   806,   333,   807,   176,   809,   334,   113,   632,    81,
     104,    10,   110,   882,   570,    87,   498,   599,   600,   368,
     113,   113,   308,   104,   104,   110,   110,   331,   549,   439,
     601,    88,    89,    90,   309,   335,    91,    92,   310,   423,
     424,   425,   426,   784,    56,   332,   336,   311,   312,    93,
      74,   333,    94,   115,    57,   334,    85,   339,    85,   624,
     329,    38,    79,   299,   628,   303,   177,    74,    79,   578,
     211,   579,   178,   580,   661,   581,    95,   582,   264,   177,
     446,   356,    13,   678,   177,   178,   330,    97,   359,   490,
     178,   176,   345,   203,    58,   336,    99,   625,   447,   100,
     101,   304,   212,    79,   448,    80,   394,   102,   449,   491,
     294,   212,   401,   402,   670,   265,   883,    43,   446,    47,
      85,   266,   267,   268,   269,   270,   271,   177,   299,   102,
     679,   524,   774,   178,   544,   547,   447,   450,   351,   761,
     399,   400,   448,   775,     1,   637,   449,   639,   451,   325,
     326,     2,   177,    44,   272,   680,    65,   454,   178,   721,
     722,   327,   744,   747,   176,   202,   681,   211,   776,   720,
      85,   441,   177,   831,   481,   218,   212,   219,   178,   615,
     220,   306,   328,   472,   832,   772,   451,   586,   676,   616,
     102,   307,   780,   474,   745,   682,   606,   607,   115,   212,
     511,   221,   513,   222,   223,   515,   224,   666,   608,   686,
     688,   115,   115,   365,   691,    78,   127,   603,   604,   365,
     723,   724,   725,   726,    85,   596,   365,   727,   509,   609,
     644,   309,   646,   475,    72,   310,   177,   476,    49,    95,
     525,    66,   178,   522,   311,   312,   477,   478,   479,   861,
      97,   150,   603,   604,   322,   323,   295,   273,  -297,   102,
    -297,   828,   829,   610,   274,   275,   324,   344,    30,   636,
     102,   638,    79,    31,   493,    79,    47,   177,   177,   177,
     177,   603,   604,   178,   178,   178,   178,    59,   276,   176,
     176,   176,   176,   773,   562,   563,   564,   565,   177,   856,
      60,   603,   604,   859,   178,   574,   712,   575,   714,   727,
     605,   277,   278,   279,   481,   587,    63,   589,    75,   973,
     591,   177,    67,   280,   281,    70,   805,   178,    34,    76,
     318,   692,    71,   176,   389,   126,   703,   704,   128,   177,
     850,   129,    79,   113,   282,   178,   104,   130,   110,   283,
      79,   176,   195,   319,   561,   320,   534,   135,   321,    35,
     603,   604,    79,    36,   177,   603,   604,    82,   903,   904,
     178,   719,   457,   365,   177,   444,   811,   131,    83,   458,
     178,   621,   622,   568,   472,   339,   626,   627,   812,   569,
     132,    79,   736,   444,   474,   598,   684,    79,   833,   133,
     177,   537,   746,   541,   541,   925,   178,   926,   538,   306,
     177,   537,   176,   537,   584,   188,   178,   584,   551,   307,
     556,   585,   176,   177,   611,   177,   934,   177,   767,   178,
     537,   178,   238,   178,   475,   209,   526,   683,   476,   177,
     765,  -253,   463,  -253,   134,   178,   786,   477,   478,   479,
     169,   170,   171,   172,   464,   584,   138,   537,   187,   309,
     139,   454,   693,   310,   787,   472,   792,   701,   702,   874,
     801,   124,   311,   312,   177,   474,   125,   537,   537,   190,
     178,   444,   891,   156,   788,   789,   176,   411,   193,   892,
     197,   177,   412,   177,   198,   177,   349,   178,   963,   178,
     350,   178,   964,   201,  -299,   205,  -299,   207,   208,   217,
     199,   728,   729,   214,   215,   475,   732,   733,   216,   476,
     554,   554,   237,   740,   741,   555,   558,   838,   477,   478,
     479,   177,   292,   293,   115,   844,   239,   178,   177,   365,
     852,   853,   751,   176,   178,   526,   177,   752,   285,   240,
     286,   357,   178,   243,   365,   365,   258,   261,   241,   365,
     265,   360,   361,   864,   865,   866,   266,   267,   268,   269,
     270,   271,   242,   873,   177,   177,   177,   875,   244,   245,
     178,   178,   178,   217,   177,   663,   284,   288,   177,   664,
     178,   884,   296,    79,   178,   518,   481,   317,   510,   272,
     177,   340,   177,   339,   461,   462,   178,   651,   178,   265,
    -298,   371,  -298,   523,   463,   266,   267,   268,   269,   270,
     271,   897,   169,   170,   171,   172,   464,   372,   347,   815,
     816,   375,   902,   376,   377,    79,   379,   594,   382,   914,
     383,   379,   385,   386,   387,   388,   916,    79,   272,   620,
     177,   393,   911,   912,   920,   921,   178,   177,   169,   170,
     171,   172,   291,   178,   755,   177,   177,   566,   567,   395,
     396,   178,   178,   444,   481,   165,   166,   221,   406,   222,
     223,   929,   224,   651,   769,   168,   263,   945,   227,   228,
     408,   414,   935,   169,   170,   171,   172,   173,   177,   951,
     952,   440,   455,   263,   178,   227,   539,   177,   177,    79,
      79,   643,   645,   178,   178,   456,   102,   955,   302,   176,
     176,   797,   798,    79,   460,   647,   960,    79,   489,   711,
     496,    79,   651,   713,   651,    79,   651,   715,   778,   966,
     779,   499,   968,   969,   970,   778,   470,   923,   526,   500,
     543,   482,   483,    14,    15,   496,    16,   978,   484,   486,
     571,   487,   528,   529,   530,   488,   512,    14,    15,   514,
      16,   521,    17,   552,   553,   559,   835,   641,    18,   557,
     583,    19,    20,   572,   588,    22,    17,  -332,   590,   602,
     658,   623,    18,   635,   595,    19,    20,    21,   642,    22,
     651,   656,   651,   612,   651,   613,   617,   618,  -332,   210,
    -332,  -332,   619,   211,   629,   863,   657,  -332,  -332,  -332,
    -332,   659,   660,   576,   258,   872,    23,   662,   672,   677,
     695,   696,   697,   698,   694,    24,    25,   699,   700,    26,
      23,    87,   707,   708,   705,   212,   285,   526,   718,    24,
      25,   730,   731,    26,   734,   526,   161,    88,    89,    90,
     748,   753,    91,    92,   754,   758,   762,   900,   760,   763,
     162,   163,   764,   768,   766,    93,   790,   791,    94,   793,
     164,   794,    85,   526,   526,   526,   795,   796,   802,   737,
     738,   913,   803,   526,   742,   743,   804,   526,   165,   166,
     810,   808,    95,   820,   821,   167,   822,   634,   168,   444,
      96,   526,   823,    97,    98,   352,   169,   170,   171,   172,
     173,   931,    99,   827,   174,   100,   101,   175,   834,   654,
     936,   937,   938,   102,   837,   843,   841,   943,   845,   102,
     265,   854,   860,   862,   870,   869,   266,   267,   268,   269,
     270,   271,   674,   675,   871,   890,   665,   876,   880,   526,
     881,   877,   234,   885,   961,   886,   526,   689,   893,   799,
     800,   899,   894,   901,   526,   526,   162,   163,   906,   272,
     908,   265,   909,   910,   915,   917,   164,   266,   267,   268,
     269,   270,   271,   922,   918,   927,   813,   814,   405,   930,
     817,   818,   933,   848,   165,   166,   939,   526,   824,   825,
     940,   167,   942,   944,   168,   849,   953,   956,   949,   962,
     272,   967,   169,   170,   171,   172,   173,   971,   265,   974,
     235,  -332,   975,   175,   266,   267,   268,   269,   270,   271,
     265,   976,   977,   980,   403,   102,   266,   267,   268,   269,
     270,   271,  -332,   409,  -332,  -332,   979,   211,   710,   404,
     924,  -332,  -332,  -332,  -332,   950,   948,   272,   407,   819,
     648,   355,   716,   597,   785,   673,   770,   771,   378,   272,
     855,   851,   577,   878,   879,   265,     0,   140,     0,   212,
       0,   266,   267,   268,   269,   270,   271,     0,   141,   142,
       0,     0,     0,     0,     0,     0,   143,   257,   144,     0,
       0,   145,     0,     0,   140,     0,   146,   147,   148,   149,
     150,     0,     0,     0,   272,   141,   142,     0,     0,     0,
       0,     0,   151,   143,     0,   144,   362,     0,   145,   102,
       0,     0,     0,   146,   147,   148,   149,   150,     0,     0,
       0,     0,   140,     0,     0,     0,     0,     0,     0,   151,
     496,   496,     0,   141,   142,     0,   102,     0,     0,     0,
       0,   143,     0,   144,   373,   842,   145,   140,     0,     0,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,   857,   858,   143,   151,   144,   380,
       0,   145,   140,     0,   102,     0,   146,   147,   148,   149,
     150,     0,     0,   141,   142,     0,     0,     0,     0,     0,
       0,   143,   151,   144,   548,     0,   145,   140,     0,   102,
       0,   146,   147,   148,   149,   150,     0,     0,   141,   142,
       0,     0,     0,     0,     0,  -325,   143,   151,   144,   685,
     898,   145,     0,     0,   102,     0,   146,   147,   148,   149,
     150,     0,   140,     0,     0,     0,  -325,     0,  -325,  -325,
       0,   557,   151,   141,   142,  -325,  -325,  -325,  -325,   102,
       0,   143,     0,   144,   687,     0,   145,     0,     0,   140,
       0,   146,   147,   148,   149,   150,     0,     0,     0,     0,
     141,   142,     0,     0,   496,     0,   496,   151,   143,     0,
     144,   690,     0,   145,   102,     0,   140,     0,   146,   147,
     148,   149,   150,     0,     0,     0,     0,   141,   142,     0,
       0,     0,     0,     0,   151,   143,     0,   144,     0,     0,
     145,   102,     0,   140,     0,   146,   147,   148,   149,   150,
       0,     0,     0,     0,   141,   142,     0,     0,     0,     0,
       0,   151,   143,     0,   494,     0,     0,   145,   102,     0,
     140,     0,   146,   147,   148,   149,   150,     0,     0,     0,
       0,   141,   142,     0,     0,     0,     0,     0,   151,   143,
       0,   494,     0,     0,   145,   102,     0,     0,     0,   146,
     147,   148,   149,   631,     0,     0,   -50,   -50,     0,     0,
     299,   140,     0,     0,     0,   151,   -50,   -50,   -50,     0,
       0,   300,   102,   427,   301,     0,   302,     0,   -50,     0,
     143,     0,   144,     0,     0,   145,   140,     0,     0,   -50,
     146,   147,   148,   149,   150,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,   143,   151,   144,   212,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     143,   151,   144,     0,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,   143,   151,   144,     0,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   435,     0,     0,     0,     0,   140,     0,
     143,   151,   144,     0,     0,   145,     0,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,   143,     0,   144,
       0,     0,   145,     0,     0,     0,   151,   146,   147,   148,
     149,   150,     0,   102,     0,     0,     0,     0,   265,     0,
       0,     0,     0,   151,   266,   267,   268,   269,   270,   271,
     102,     0,     0,     0,     0,     0,   265,     0,     0,     0,
       0,   358,   266,   267,   268,   269,   270,   271,     0,     0,
       0,     0,     0,     0,     0,   265,     0,   272,     0,     0,
     410,   266,   267,   268,   269,   270,   271,     0,     0,     0,
       0,     0,     0,   265,     0,   272,     0,     0,   413,   266,
     267,   268,   269,   270,   271,     0,     0,     0,     0,   265,
       0,   540,     0,     0,   272,   266,   267,   268,   269,   270,
     271,     0,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,   272,   781,   266,   267,   268,   269,   270,   271,
       0,     0,     0,   265,     0,     0,   782,     0,   272,   266,
     267,   268,   269,   270,   271,   225,     0,     0,     0,     0,
       0,     0,   265,     0,   905,     0,     0,   272,   266,   267,
     268,   269,   270,   271,     0,   246,   226,   247,   227,   228,
     248,     0,   272,     0,     0,   229,   230,   231,   232,     0,
       0,     0,     0,   249,     0,     0,     0,     0,     0,   250,
       0,   272,     0,   251,     0,     0,   252,   415,   416,   417,
     418,   419,   420,   421,   422
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,    45,    89,   501,   342,    71,    93,   212,
      80,    54,    51,    71,    99,    51,    80,    51,   301,   302,
      39,   196,    80,   353,   217,   535,   304,   357,   322,   323,
     584,   224,   164,   309,   594,   140,     6,    18,    19,     5,
     145,   324,   299,     8,     2,     9,     5,   125,     5,     5,
      31,     5,    91,   287,     9,   125,    48,   141,   142,     5,
       5,   125,    38,     5,    45,     5,    76,   125,     5,   277,
     278,   279,   280,   281,   282,   283,    26,     5,     5,     5,
      30,    76,    28,   643,    94,   645,    36,   647,    17,   325,
     326,   327,   328,    30,    75,     0,   299,   167,    49,    53,
      76,    82,    59,   167,    96,     5,    76,   165,   166,    65,
      58,    76,    76,    58,   392,   372,    74,    76,    64,    85,
      86,    76,    76,    65,    64,    95,    76,   623,     8,    58,
      80,    58,    28,    60,    26,    26,    62,    65,    64,    89,
      90,    91,   212,   124,    36,    36,    26,     5,   212,    97,
      17,   711,    32,   713,   212,   715,    36,   196,   494,     5,
     196,    76,   196,    63,     5,     3,    62,   461,   462,   372,
     209,   210,    64,   209,   210,   209,   210,     8,   371,   284,
     463,    19,    20,    21,    76,    65,    24,    25,    80,   273,
     274,   275,   276,    60,    26,    26,    76,    89,    90,    37,
      58,    32,    40,    51,    36,    36,     5,   188,     5,    38,
      50,    76,    58,    48,   490,    28,   286,    58,    58,   427,
      64,   429,   286,   431,    59,   433,    64,   435,     5,   299,
       8,    30,    59,    30,   304,   299,   187,    75,    76,     5,
     304,   299,   193,    91,    76,    76,    84,    76,    26,    87,
      88,    64,    96,    58,    32,    60,   237,    95,    36,    25,
      76,    96,   243,   244,   774,    42,   820,    76,     8,   250,
       5,    48,    49,    50,    51,    52,    53,   347,    48,    95,
       5,   346,     5,   347,   369,   370,    26,    65,    23,    59,
     241,   242,    32,    16,     4,   498,    36,   500,    76,    44,
      45,    11,   372,    76,    81,    30,     5,   288,   372,   603,
     604,    56,    28,    60,   372,   354,    41,    64,    41,   602,
       5,    76,   392,    65,   305,    59,    96,    61,   392,    26,
      38,    26,    77,    26,    76,   665,    76,    30,   541,    36,
      95,    36,   672,    36,    60,    30,    44,    45,   196,    96,
     331,    59,   333,    61,    62,   336,    64,   532,    56,   552,
     553,   209,   210,   211,   557,   407,   409,    54,    55,   217,
     606,   607,   608,   609,     5,   456,   224,   611,   329,    77,
     512,    76,   514,    76,   403,    80,   456,    80,    76,    64,
      76,    12,   456,   344,    89,    90,    89,    90,    91,    30,
      75,    76,    54,    55,    54,    55,   167,    47,    59,    95,
      61,   747,   748,    65,    54,    55,    66,    50,    59,   497,
      95,   499,    58,    64,    60,    58,   407,   497,   498,   499,
     500,    54,    55,   497,   498,   499,   500,    76,    78,   497,
     498,   499,   500,    66,   395,   396,   397,   398,   518,   779,
      76,    54,    55,   783,   518,   406,   588,   408,   590,   693,
      63,    44,    45,    46,   445,   446,    76,   448,    48,   965,
     451,   541,    76,    56,    57,    53,   710,   541,    59,    59,
       6,   559,    60,   541,    50,    76,   571,   572,    76,   559,
     773,    76,    58,   532,    77,   559,   532,    76,   532,    82,
      58,   559,    84,    29,    50,    31,   354,    65,    34,    72,
      54,    55,    58,    76,   584,    54,    55,    48,   848,   849,
     584,    65,    58,   371,   594,   286,    65,    59,    59,    65,
     594,   482,   483,    50,    26,   516,   487,   488,    30,    50,
      59,    58,   620,   304,    36,    50,    50,    58,   751,    76,
     620,    58,   630,    58,    58,   891,   620,   893,    65,    26,
     630,    58,   620,    58,    58,    64,   630,    58,    65,    36,
      65,    65,   630,   643,    65,   645,   906,   647,   659,   643,
      58,   645,    59,   647,    76,    60,   347,    65,    80,   659,
     655,    58,    64,    60,    76,   659,   681,    89,    90,    91,
      72,    73,    74,    75,    76,    58,    72,    58,    76,    76,
      76,   592,    65,    80,    65,    26,   694,   568,   569,    30,
     705,    48,    89,    90,   694,    36,    53,    58,    58,    76,
     694,   392,    58,    70,    65,    65,   694,    58,    76,    65,
      72,   711,    63,   713,    76,   715,    38,   711,    72,   713,
      42,   715,    76,    90,    59,    92,    61,    94,    95,    64,
      76,   612,   613,   100,    76,    76,   617,   618,    76,    80,
      58,    58,    48,   624,   625,    63,    63,   758,    89,    90,
      91,   751,   165,   166,   532,   766,    59,   751,   758,   537,
     775,   776,    58,   751,   758,   456,   766,    63,    62,    59,
      64,    33,   766,    48,   552,   553,   143,   144,    76,   557,
      42,   209,   210,   794,   795,   796,    48,    49,    50,    51,
      52,    53,    76,   804,   794,   795,   796,   808,    48,    59,
     794,   795,   796,    64,   804,    18,    79,    64,   808,    22,
     804,   822,    76,    58,   808,    60,   727,    53,   330,    81,
     820,     5,   822,   734,    54,    55,   820,   518,   822,    42,
      59,    64,    61,   345,    64,    48,    49,    50,    51,    52,
      53,   836,    72,    73,    74,    75,    76,    48,    60,   730,
     731,    76,   847,   220,    76,    58,   223,    60,   225,   870,
      76,   228,   229,   230,   231,   232,   877,    58,    81,    60,
     870,     5,   867,   868,   885,   886,   870,   877,    72,    73,
      74,    75,    76,   877,   642,   885,   886,   399,   400,    76,
      76,   885,   886,   584,   805,    54,    55,    59,    76,    61,
      62,   896,    64,   594,   662,    64,    59,   918,    61,    62,
      76,    76,   907,    72,    73,    74,    75,    76,   918,   927,
     928,    75,    28,    59,   918,    61,    62,   927,   928,    58,
      58,    60,    60,   927,   928,    60,    95,   932,    64,   927,
     928,   699,   700,    58,    76,    60,   941,    58,    30,    60,
     317,    58,   643,    60,   645,    58,   647,    60,    58,   954,
      60,    28,   957,   958,   959,    58,    76,    60,   659,    28,
      92,    76,    76,     7,     8,   342,    10,   972,    76,    76,
      14,    76,   349,   350,   351,    76,    76,     7,     8,    76,
      10,    76,    26,    64,    64,    53,   754,   509,    32,    64,
      63,    35,    36,    37,    76,    39,    26,    38,    76,    64,
     522,    65,    32,     5,    76,    35,    36,    37,     5,    39,
     711,     5,   713,    76,   715,    76,    76,    76,    59,    60,
      61,    62,    76,    64,    76,   793,     5,    68,    69,    70,
      71,    60,     5,   410,   411,   803,    80,     5,    41,    76,
     562,   563,   564,   565,    53,    89,    90,     5,     5,    93,
      80,     3,   574,   575,    15,    96,    62,   758,    28,    89,
      90,    76,    76,    93,    62,   766,    12,    19,    20,    21,
      58,    30,    24,    25,     5,    60,    53,   845,    28,     5,
      26,    27,     5,    76,    60,    37,     5,     5,    40,     5,
      36,    60,     5,   794,   795,   796,    60,    60,    30,   621,
     622,   869,     5,   804,   626,   627,    60,   808,    54,    55,
      76,    60,    64,    60,     5,    61,    60,   494,    64,   820,
      72,   822,    30,    75,    76,    17,    72,    73,    74,    75,
      76,   899,    84,    76,    80,    87,    88,    83,    28,    76,
     908,   909,   910,    95,    53,     5,    76,   915,     5,    95,
      42,    30,    76,     5,    60,     5,    48,    49,    50,    51,
      52,    53,   539,   540,    59,    28,    17,     5,     5,   870,
       5,    60,    12,    60,   942,    60,   877,   554,    60,   701,
     702,     5,    63,     5,   885,   886,    26,    27,    17,    81,
       5,    42,     5,     5,     5,     5,    36,    48,    49,    50,
      51,    52,    53,     5,    60,    28,   728,   729,   248,     5,
     732,   733,     5,    17,    54,    55,     5,   918,   740,   741,
       5,    61,     5,     5,    64,    17,     5,     5,    64,     5,
      81,     5,    72,    73,    74,    75,    76,     5,    42,     5,
      80,    38,     5,    83,    48,    49,    50,    51,    52,    53,
      42,     5,     5,     5,   246,    95,    48,    49,    50,    51,
      52,    53,    59,   252,    61,    62,    65,    64,   585,   247,
     890,    68,    69,    70,    71,   924,   922,    81,   250,   734,
     516,    28,   592,   457,   679,   537,   663,   664,   223,    81,
     778,   774,   411,   815,   816,    42,    -1,    43,    -1,    96,
      -1,    48,    49,    50,    51,    52,    53,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    67,    -1,    -1,    43,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    81,    54,    55,    -1,    -1,    -1,
      -1,    -1,    88,    62,    -1,    64,    65,    -1,    67,    95,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    88,
     747,   748,    -1,    54,    55,    -1,    95,    -1,    -1,    -1,
      -1,    62,    -1,    64,    65,   762,    67,    43,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,   781,   782,    62,    88,    64,    65,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    65,    -1,    67,    43,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    38,    62,    88,    64,    65,
     837,    67,    -1,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    43,    -1,    -1,    -1,    59,    -1,    61,    62,
      -1,    64,    88,    54,    55,    68,    69,    70,    71,    95,
      -1,    62,    -1,    64,    65,    -1,    67,    -1,    -1,    43,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      54,    55,    -1,    -1,   891,    -1,   893,    88,    62,    -1,
      64,    65,    -1,    67,    95,    -1,    43,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    88,    62,    -1,    64,    -1,    -1,
      67,    95,    -1,    43,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    88,    62,    -1,    64,    -1,    -1,    67,    95,    -1,
      43,    -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    88,    62,
      -1,    64,    -1,    -1,    67,    95,    -1,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    44,    45,    -1,    -1,
      48,    43,    -1,    -1,    -1,    88,    54,    55,    56,    -1,
      -1,    59,    95,    55,    62,    -1,    64,    -1,    66,    -1,
      62,    -1,    64,    -1,    -1,    67,    43,    -1,    -1,    77,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    96,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    43,    -1,
      62,    88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    62,    -1,    64,
      -1,    -1,    67,    -1,    -1,    -1,    88,    72,    73,    74,
      75,    76,    -1,    95,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    88,    48,    49,    50,    51,    52,    53,
      95,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    65,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    81,    -1,    -1,
      66,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    81,    -1,    -1,    65,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    42,
      -1,    60,    -1,    -1,    81,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    81,    66,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    42,    -1,    -1,    60,    -1,    81,    48,
      49,    50,    51,    52,    53,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    63,    -1,    -1,    81,    48,    49,
      50,    51,    52,    53,    -1,     8,    59,    10,    61,    62,
      13,    -1,    81,    -1,    -1,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    81,    -1,    36,    -1,    -1,    39,   265,   266,   267,
     268,   269,   270,   271,   272
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    99,   100,   103,   104,   106,    76,   101,
      76,   121,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    89,    90,    93,   105,   157,   185,
      59,    64,   102,     5,    59,    72,    76,   111,    76,   112,
     113,   120,   121,    76,    76,   155,   156,   184,   184,    76,
     186,   188,    76,    94,   118,   119,    26,    36,    76,    76,
      76,    26,    36,    76,   184,     5,    12,    76,     5,    58,
      53,    60,   113,     5,    58,    48,    59,   173,   156,    58,
      60,     5,    48,    59,   173,     5,    30,     3,    19,    20,
      21,    24,    25,    37,    40,    64,    72,    75,    76,    84,
      87,    88,    95,   122,   123,   126,   191,   192,   193,   196,
     197,   198,   199,   200,   203,   204,   206,   207,   208,   209,
     216,   217,   218,   219,    48,    53,    76,   119,    76,    76,
      76,    59,    59,    76,    76,    65,   103,   107,    72,    76,
      43,    54,    55,    62,    64,    67,    72,    73,    74,    75,
      76,    88,   123,   197,   200,   204,   221,   222,   223,   224,
     225,    12,    26,    27,    36,    54,    55,    61,    64,    72,
      73,    74,    75,    76,    80,    83,   117,   122,   126,   127,
     129,   139,   140,   141,   142,   121,   184,    76,    64,   174,
      76,   129,   184,    76,     5,   174,   187,    72,    76,    76,
     186,   221,   200,   204,   220,   221,   186,   221,   221,    60,
      60,    64,    96,   186,   221,    76,    76,    64,    59,    61,
      38,    59,    61,    62,    64,    38,    59,    61,    62,    68,
      69,    70,    71,   184,    12,    80,   129,    48,    59,    59,
      59,    76,    76,    48,    48,    59,     8,    10,    13,    26,
      32,    36,    39,   108,   225,   223,   223,    63,   221,   226,
     227,   221,   225,    59,     5,    42,    48,    49,    50,    51,
      52,    53,    81,    47,    54,    55,    78,    44,    45,    46,
      56,    57,    77,    82,    79,    62,    64,   134,    64,   179,
     179,    76,   142,   142,    76,   127,    76,   182,   183,    48,
      59,    62,    64,    28,    64,   135,    26,    36,    64,    76,
      80,    89,    90,   146,   147,   148,   184,    53,     6,    29,
      31,    34,    54,    55,    66,    44,    45,    56,    77,    50,
     173,     8,    26,    32,    36,    65,    76,   175,   176,   184,
       5,     5,    53,    76,    50,   173,   160,    60,   191,    38,
      42,    23,    17,    17,    58,    28,    30,    33,    65,    76,
     192,   192,    65,   201,   202,   204,   221,   128,   140,    85,
      86,    64,    48,    65,   201,    76,   221,    76,   205,   221,
      65,   201,   221,    76,   205,   221,   221,   221,   221,    50,
       5,     5,    64,     5,   184,    76,    76,    38,    76,   173,
     173,   184,   184,   112,   120,   104,    76,   155,    76,   118,
      66,    58,    63,    65,    76,   222,   222,   222,   222,   222,
     222,   222,   222,   223,   223,   223,   223,    55,   224,    55,
     224,    55,   224,    55,   224,    55,   224,   224,   224,   225,
      75,    76,   124,   125,   127,   135,     8,    26,    32,    36,
      65,    76,   180,   181,   184,    28,    60,    58,    65,   128,
      76,    54,    55,    64,    76,   117,   143,   144,   145,   143,
      76,   124,    26,    30,    36,    76,    80,    89,    90,    91,
     136,   184,    76,    76,    76,   148,    76,    76,    76,    30,
       5,    25,   152,    60,    64,   114,   221,    28,    62,    28,
      28,   132,   144,   144,   143,   145,   145,   145,   145,   173,
     174,   184,    76,   184,    76,   184,     5,    65,    60,   178,
     114,    76,   173,   174,   103,    76,   127,   133,   221,   221,
     221,   189,   190,   189,   204,   210,   189,    58,    65,    62,
      60,    58,    97,    92,   186,   194,   195,   186,    65,   201,
     128,    65,    64,    64,    58,    63,    65,    64,    63,    53,
     124,    50,   173,   173,   173,   173,   174,   174,    50,    50,
       5,    14,    37,   109,   173,   173,   221,   227,   224,   224,
     224,   224,   224,    63,    58,    65,    30,   184,    76,   184,
      76,   184,     5,    65,    60,    76,   133,   183,    50,   144,
     144,   143,    64,    54,    55,    63,    44,    45,    56,    77,
      65,    65,    76,    76,     5,    26,    36,    76,    76,    76,
      60,   173,   173,    65,    38,    76,   173,   173,   148,    76,
     149,    76,   114,   116,   221,     5,   129,   140,   129,   140,
     146,   174,     5,    60,   179,    60,   179,    60,   176,     6,
      76,   127,   177,     9,    76,   158,     5,     5,   174,    60,
       5,    59,     5,    18,    22,    17,   191,   143,   212,   213,
     214,   215,    41,   202,   221,   221,   140,    76,    30,     5,
      30,    41,    30,    65,    50,    65,   201,    65,   201,   221,
      65,   201,   129,    65,    53,   174,   174,   174,   174,     5,
       5,   173,   173,   186,   186,    15,   110,   174,   174,   125,
     134,    60,   179,    60,   179,    60,   181,   177,    28,    65,
     143,   144,   144,   145,   145,   145,   145,   135,   173,   173,
      76,    76,   173,   173,    62,   137,   129,   174,   174,   146,
     173,   173,   174,   174,    28,    60,   129,    60,    58,     5,
      65,    58,    63,    30,     5,   178,   177,   177,    60,   177,
      28,    59,    53,     5,     5,   103,    60,   133,    76,   178,
     221,   221,   189,    66,     5,    16,    41,   211,    58,    60,
     189,    66,    60,    17,    60,   195,   186,    65,    65,    65,
       5,     5,   129,     5,    60,    60,    60,   178,   178,   174,
     174,   186,    30,     5,    60,   135,   177,   177,    60,   177,
      76,    65,    30,   174,   174,   173,   173,   174,   174,   175,
      60,     5,    60,    30,   174,   174,   151,    76,   114,   114,
     115,    65,    76,   140,    28,   178,   165,    53,   133,     8,
      76,    76,   221,     5,   133,     5,     9,   159,    17,    17,
     143,   213,   186,   186,    30,   215,   189,   221,   221,   189,
      76,    30,     5,   178,   133,   133,   133,   167,   168,     5,
      60,    59,   178,   133,    30,   133,     5,    60,   174,   174,
       5,     5,    63,   125,   133,    60,    60,   153,   154,   214,
      28,    58,    65,    60,    63,   130,   163,   103,   221,     5,
     178,     5,   103,   189,   189,    63,    17,   169,     5,     5,
       5,   103,   103,   178,   133,     5,   133,     5,    60,   138,
     133,   133,     5,    60,   151,   114,   114,    28,   131,   103,
       5,   178,   166,     5,   189,   103,   178,   178,   178,     5,
       5,   161,     5,   178,     5,   133,     5,    76,   154,    64,
     153,   129,   129,     5,   164,   103,     5,   172,   171,   170,
     103,   178,     5,    72,    76,   150,   103,     5,   103,   103,
     103,     5,   162,   146,     5,     5,     5,     5,   103,    65,
       5
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
     148,   150,   151,   152,   152,   152,   153,   153,   154,   154,
     155,   155,   156,   156,   156,   157,   157,   158,   157,   159,
     157,   160,   157,   161,   157,   162,   157,   163,   157,   164,
     157,   165,   157,   166,   157,   167,   157,   168,   157,   169,
     157,   170,   157,   171,   157,   172,   157,   173,   174,   174,
     174,   175,   175,   176,   176,   176,   176,   176,   176,   176,
     176,   177,   177,   177,   177,   177,   178,   178,   179,   179,
     179,   180,   180,   181,   181,   181,   181,   181,   181,   182,
     182,   183,   184,   184,   185,   187,   186,   188,   186,   190,
     189,   191,   191,   191,   192,   192,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   193,
     193,   193,   193,   194,   194,   194,   195,   195,   196,   196,
     197,   197,   197,   198,   198,   198,   198,   199,   199,   199,
     200,   200,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   201,   201,   202,   202,   202,   202,   203,   203,
     203,   203,   203,   203,   204,   204,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   206,   207,   207,   208,   208,
     210,   209,   211,   211,   211,   212,   212,   213,   213,   214,
     214,   215,   215,   216,   217,   218,   218,   218,   219,   220,
     220,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   222,   222,   222,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     224,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   227,   227
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
       0,     0,     0,     7,     5,     0,     3,     1,     6,     0,
       2,     1,     4,     6,     6,     8,    10,     0,     9,     0,
      11,     0,     7,     0,    13,     0,    15,     0,    12,     0,
      14,     0,    11,     0,    13,     0,    11,     0,    11,     0,
      12,     0,    14,     0,    14,     0,    14,     0,     3,     2,
       0,     3,     1,     3,     5,     4,     3,     4,     5,     6,
       4,     1,     3,     3,     3,     1,     0,     3,     3,     2,
       0,     3,     1,     3,     4,     4,     4,     3,     5,     3,
       1,     1,     3,     1,     3,     0,     4,     0,     2,     0,
       2,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       5,     7,     5,     1,     3,     2,     6,     4,     1,     2,
       2,     4,     5,     1,     1,     3,     1,     1,     3,     1,
       4,     4,     3,     3,     4,     3,     6,     5,     6,     5,
       6,     5,     3,     1,     1,     3,     5,     6,     3,     3,
       3,     3,     3,     3,     4,     3,     2,     3,     4,     2,
       3,     2,     1,     3,     1,     3,     2,     2,     4,     6,
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

  case 170: /* record_field: %empty  */
#line 480 "pascal.y"
     { (yyval.node) = NULL; }
#line 3319 "pascal.tab.c"
    break;

  case 171: /* case_arm_mark: %empty  */
#line 483 "pascal.y"
    { }
#line 3325 "pascal.tab.c"
    break;

  case 172: /* case_open: %empty  */
#line 486 "pascal.y"
    { }
#line 3331 "pascal.tab.c"
    break;

  case 173: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 489 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3337 "pascal.tab.c"
    break;

  case 174: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 490 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3343 "pascal.tab.c"
    break;

  case 175: /* record_case_opt: %empty  */
#line 491 "pascal.y"
     { (yyval.node) = NULL; }
#line 3349 "pascal.tab.c"
    break;

  case 176: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 494 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3355 "pascal.tab.c"
    break;

  case 177: /* record_case_list: record_case_arm  */
#line 495 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3361 "pascal.tab.c"
    break;

  case 178: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 498 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3367 "pascal.tab.c"
    break;

  case 179: /* record_case_arm: %empty  */
#line 499 "pascal.y"
     { (yyval.node) = NULL; }
#line 3373 "pascal.tab.c"
    break;

  case 180: /* var_decl_list: var_decl_list var_decl  */
#line 502 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3379 "pascal.tab.c"
    break;

  case 181: /* var_decl_list: var_decl  */
#line 503 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3385 "pascal.tab.c"
    break;

  case 182: /* var_decl: id_list COLON type SEMICOLON  */
#line 506 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3391 "pascal.tab.c"
    break;

  case 183: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 507 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3397 "pascal.tab.c"
    break;

  case 184: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 508 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3403 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 511 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3409 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 512 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3415 "pascal.tab.c"
    break;

  case 187: /* $@5: %empty  */
#line 513 "pascal.y"
                                                                               { }
#line 3421 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3427 "pascal.tab.c"
    break;

  case 189: /* $@6: %empty  */
#line 514 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3433 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3439 "pascal.tab.c"
    break;

  case 191: /* $@7: %empty  */
#line 515 "pascal.y"
                                        { }
#line 3445 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 515 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3451 "pascal.tab.c"
    break;

  case 193: /* $@8: %empty  */
#line 516 "pascal.y"
                                                                                                           { }
#line 3457 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3463 "pascal.tab.c"
    break;

  case 195: /* $@9: %empty  */
#line 517 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3469 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3475 "pascal.tab.c"
    break;

  case 197: /* $@10: %empty  */
#line 518 "pascal.y"
                                                                                                 { }
#line 3481 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3487 "pascal.tab.c"
    break;

  case 199: /* $@11: %empty  */
#line 519 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3493 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3499 "pascal.tab.c"
    break;

  case 201: /* $@12: %empty  */
#line 520 "pascal.y"
                                                                                            { }
#line 3505 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3511 "pascal.tab.c"
    break;

  case 203: /* $@13: %empty  */
#line 521 "pascal.y"
                                                                                                             { }
#line 3517 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3525 "pascal.tab.c"
    break;

  case 205: /* $@14: %empty  */
#line 524 "pascal.y"
                                                                                              { }
#line 3531 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 524 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3539 "pascal.tab.c"
    break;

  case 207: /* $@15: %empty  */
#line 527 "pascal.y"
                                                                                             { }
#line 3545 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 527 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3553 "pascal.tab.c"
    break;

  case 209: /* $@16: %empty  */
#line 530 "pascal.y"
                                                                                                    { }
#line 3559 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 530 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3567 "pascal.tab.c"
    break;

  case 211: /* $@17: %empty  */
#line 533 "pascal.y"
                                                                                                                { }
#line 3573 "pascal.tab.c"
    break;

  case 212: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 533 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3582 "pascal.tab.c"
    break;

  case 213: /* $@18: %empty  */
#line 537 "pascal.y"
                                                                                                                  { }
#line 3588 "pascal.tab.c"
    break;

  case 214: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 537 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3597 "pascal.tab.c"
    break;

  case 215: /* $@19: %empty  */
#line 541 "pascal.y"
                                                                                                                     { }
#line 3603 "pascal.tab.c"
    break;

  case 216: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3611 "pascal.tab.c"
    break;

  case 217: /* pv_mark: %empty  */
#line 546 "pascal.y"
    { }
#line 3617 "pascal.tab.c"
    break;

  case 218: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 549 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3623 "pascal.tab.c"
    break;

  case 219: /* parameter_list_opt: LPARENT RPARENT  */
#line 550 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3629 "pascal.tab.c"
    break;

  case 220: /* parameter_list_opt: %empty  */
#line 551 "pascal.y"
     { (yyval.node) = NULL; }
#line 3635 "pascal.tab.c"
    break;

  case 221: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 554 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3641 "pascal.tab.c"
    break;

  case 222: /* parameter_decl_list: parameter_decl  */
#line 555 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3647 "pascal.tab.c"
    break;

  case 223: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 558 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3653 "pascal.tab.c"
    break;

  case 224: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 559 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3659 "pascal.tab.c"
    break;

  case 225: /* parameter_decl: VARSY id_list COLON param_type  */
#line 560 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3665 "pascal.tab.c"
    break;

  case 226: /* parameter_decl: id_list COLON param_type  */
#line 561 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3671 "pascal.tab.c"
    break;

  case 227: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 562 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3677 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 563 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3683 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 564 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3689 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: IDENT id_list COLON param_type  */
#line 565 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3695 "pascal.tab.c"
    break;

  case 231: /* param_type: IDENT  */
#line 568 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3701 "pascal.tab.c"
    break;

  case 232: /* param_type: IDENT PERIOD IDENT  */
#line 569 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3707 "pascal.tab.c"
    break;

  case 233: /* param_type: ARRAYSY OFSY IDENT  */
#line 570 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3713 "pascal.tab.c"
    break;

  case 234: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 571 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3719 "pascal.tab.c"
    break;

  case 235: /* param_type: spec_ref  */
#line 572 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3725 "pascal.tab.c"
    break;

  case 236: /* routine_directives: %empty  */
#line 575 "pascal.y"
    { (yyval.node) = NULL; }
#line 3731 "pascal.tab.c"
    break;

  case 237: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 576 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3744 "pascal.tab.c"
    break;

  case 238: /* pf_params: LPARENT pf_sections RPARENT  */
#line 586 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3750 "pascal.tab.c"
    break;

  case 239: /* pf_params: LPARENT RPARENT  */
#line 587 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3756 "pascal.tab.c"
    break;

  case 240: /* pf_params: %empty  */
#line 588 "pascal.y"
     { (yyval.node) = NULL; }
#line 3762 "pascal.tab.c"
    break;

  case 241: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 591 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3768 "pascal.tab.c"
    break;

  case 242: /* pf_sections: pf_section  */
#line 592 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3774 "pascal.tab.c"
    break;

  case 243: /* pf_section: id_list COLON param_type  */
#line 595 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3780 "pascal.tab.c"
    break;

  case 244: /* pf_section: VARSY id_list COLON param_type  */
#line 596 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3786 "pascal.tab.c"
    break;

  case 245: /* pf_section: CONSTSY id_list COLON param_type  */
#line 597 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3792 "pascal.tab.c"
    break;

  case 246: /* pf_section: IDENT id_list COLON param_type  */
#line 598 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3798 "pascal.tab.c"
    break;

  case 247: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 599 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3804 "pascal.tab.c"
    break;

  case 248: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 600 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3810 "pascal.tab.c"
    break;

  case 249: /* enum_list: enum_list COMMA enum_member  */
#line 603 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3816 "pascal.tab.c"
    break;

  case 250: /* enum_list: enum_member  */
#line 604 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3822 "pascal.tab.c"
    break;

  case 251: /* enum_member: IDENT  */
#line 607 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3828 "pascal.tab.c"
    break;

  case 252: /* id_list: id_list COMMA IDENT  */
#line 610 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3834 "pascal.tab.c"
    break;

  case 253: /* id_list: IDENT  */
#line 611 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3840 "pascal.tab.c"
    break;

  case 254: /* body: BEGINSY statement_list ENDSY  */
#line 614 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3846 "pascal.tab.c"
    break;

  case 255: /* $@20: %empty  */
#line 617 "pascal.y"
                             { pas_stmt_mark(); }
#line 3852 "pascal.tab.c"
    break;

  case 256: /* statement_list: statement_list SEMICOLON $@20 statement  */
#line 617 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3858 "pascal.tab.c"
    break;

  case 257: /* $@21: %empty  */
#line 618 "pascal.y"
     { pas_stmt_mark(); }
#line 3864 "pascal.tab.c"
    break;

  case 258: /* statement_list: $@21 statement  */
#line 618 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3870 "pascal.tab.c"
    break;

  case 259: /* $@22: %empty  */
#line 621 "pascal.y"
    { pas_stmt_mark(); }
#line 3876 "pascal.tab.c"
    break;

  case 260: /* body_stmt: $@22 statement  */
#line 621 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3882 "pascal.tab.c"
    break;

  case 261: /* statement: statement_no_label  */
#line 624 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3888 "pascal.tab.c"
    break;

  case 262: /* statement: INTCONST COLON statement_no_label  */
#line 625 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3894 "pascal.tab.c"
    break;

  case 263: /* statement: IDENT COLON statement_no_label  */
#line 626 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3900 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: assignment  */
#line 629 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3906 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: call  */
#line 630 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3912 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: compound_statement  */
#line 631 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3918 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: goto_statement  */
#line 632 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3924 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: if_statement  */
#line 633 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3930 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: case_statement  */
#line 634 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3936 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: while_statement  */
#line 635 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3942 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: repeat_statement  */
#line 636 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3948 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: for_statement  */
#line 637 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3954 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: with_statement  */
#line 638 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3960 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: try_statement  */
#line 639 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3966 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: raise_statement  */
#line 640 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3972 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: inherited_call  */
#line 641 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3978 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: INHERITEDSY  */
#line 642 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3984 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: %empty  */
#line 643 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3990 "pascal.tab.c"
    break;

  case 279: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 646 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3996 "pascal.tab.c"
    break;

  case 280: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 647 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4002 "pascal.tab.c"
    break;

  case 281: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 648 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4008 "pascal.tab.c"
    break;

  case 282: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 649 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4014 "pascal.tab.c"
    break;

  case 283: /* handler_list: handler  */
#line 652 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4020 "pascal.tab.c"
    break;

  case 284: /* handler_list: handler_list SEMICOLON handler  */
#line 653 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4026 "pascal.tab.c"
    break;

  case 285: /* handler_list: handler_list SEMICOLON  */
#line 654 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4032 "pascal.tab.c"
    break;

  case 286: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 657 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4038 "pascal.tab.c"
    break;

  case 287: /* handler: ONSY IDENT DOSY body_stmt  */
#line 658 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4044 "pascal.tab.c"
    break;

  case 288: /* raise_statement: RAISESY  */
#line 661 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4050 "pascal.tab.c"
    break;

  case 289: /* raise_statement: RAISESY expression  */
#line 662 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4056 "pascal.tab.c"
    break;

  case 290: /* inherited_call: INHERITEDSY IDENT  */
#line 665 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4062 "pascal.tab.c"
    break;

  case 291: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 666 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4068 "pascal.tab.c"
    break;

  case 292: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 667 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4074 "pascal.tab.c"
    break;

  case 293: /* call: IDENT  */
#line 670 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4080 "pascal.tab.c"
    break;

  case 294: /* call: spec_expr  */
#line 671 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4086 "pascal.tab.c"
    break;

  case 295: /* call: selector PERIOD IDENT  */
#line 672 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4092 "pascal.tab.c"
    break;

  case 296: /* call: call_with_args  */
#line 673 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4098 "pascal.tab.c"
    break;

  case 297: /* recv_primary: STRINGCONST  */
#line 676 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4104 "pascal.tab.c"
    break;

  case 298: /* recv_primary: LPARENT expression RPARENT  */
#line 677 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4110 "pascal.tab.c"
    break;

  case 299: /* recv_primary: spec_expr  */
#line 678 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4116 "pascal.tab.c"
    break;

  case 300: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 681 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4122 "pascal.tab.c"
    break;

  case 301: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 682 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4128 "pascal.tab.c"
    break;

  case 302: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 683 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4134 "pascal.tab.c"
    break;

  case 303: /* call_with_args: IDENT LPARENT RPARENT  */
#line 684 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4140 "pascal.tab.c"
    break;

  case 304: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 685 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4146 "pascal.tab.c"
    break;

  case 305: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 686 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4152 "pascal.tab.c"
    break;

  case 306: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 687 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4158 "pascal.tab.c"
    break;

  case 307: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 688 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4164 "pascal.tab.c"
    break;

  case 308: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 689 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4170 "pascal.tab.c"
    break;

  case 309: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 690 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4176 "pascal.tab.c"
    break;

  case 310: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 691 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4182 "pascal.tab.c"
    break;

  case 311: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 692 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4188 "pascal.tab.c"
    break;

  case 312: /* argument_list: argument_list COMMA argument  */
#line 695 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4194 "pascal.tab.c"
    break;

  case 313: /* argument_list: argument  */
#line 696 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4200 "pascal.tab.c"
    break;

  case 314: /* argument: expression  */
#line 699 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4206 "pascal.tab.c"
    break;

  case 315: /* argument: expression COLON expression  */
#line 700 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4212 "pascal.tab.c"
    break;

  case 316: /* argument: expression COLON expression COLON expression  */
#line 701 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4218 "pascal.tab.c"
    break;

  case 317: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 702 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4224 "pascal.tab.c"
    break;

  case 318: /* assignment: selector BECOMES expression  */
#line 705 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4230 "pascal.tab.c"
    break;

  case 319: /* assignment: selector ADDBECOMES expression  */
#line 706 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4236 "pascal.tab.c"
    break;

  case 320: /* assignment: selector SUBBECOMES expression  */
#line 707 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4242 "pascal.tab.c"
    break;

  case 321: /* assignment: selector MULBECOMES expression  */
#line 708 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4248 "pascal.tab.c"
    break;

  case 322: /* assignment: selector DIVBECOMES expression  */
#line 709 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4254 "pascal.tab.c"
    break;

  case 323: /* assignment: call_with_args BECOMES expression  */
#line 710 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4260 "pascal.tab.c"
    break;

  case 324: /* selector: selector LBRACK expression_list RBRACK  */
#line 713 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4266 "pascal.tab.c"
    break;

  case 325: /* selector: selector PERIOD IDENT  */
#line 714 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4272 "pascal.tab.c"
    break;

  case 326: /* selector: selector ARROW  */
#line 715 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4278 "pascal.tab.c"
    break;

  case 327: /* selector: call_with_args PERIOD IDENT  */
#line 716 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4284 "pascal.tab.c"
    break;

  case 328: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 717 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4290 "pascal.tab.c"
    break;

  case 329: /* selector: call_with_args ARROW  */
#line 718 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4296 "pascal.tab.c"
    break;

  case 330: /* selector: recv_primary PERIOD IDENT  */
#line 719 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4302 "pascal.tab.c"
    break;

  case 331: /* selector: recv_primary ARROW  */
#line 720 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4308 "pascal.tab.c"
    break;

  case 332: /* selector: IDENT  */
#line 721 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4314 "pascal.tab.c"
    break;

  case 333: /* expression_list: expression_list COMMA expression  */
#line 724 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4320 "pascal.tab.c"
    break;

  case 334: /* expression_list: expression  */
#line 725 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4326 "pascal.tab.c"
    break;

  case 335: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 728 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4332 "pascal.tab.c"
    break;

  case 336: /* goto_statement: GOTOSY INTCONST  */
#line 731 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4338 "pascal.tab.c"
    break;

  case 337: /* goto_statement: GOTOSY IDENT  */
#line 732 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4344 "pascal.tab.c"
    break;

  case 338: /* if_statement: IFSY expression THENSY body_stmt  */
#line 735 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4350 "pascal.tab.c"
    break;

  case 339: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 736 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4356 "pascal.tab.c"
    break;

  case 340: /* $@23: %empty  */
#line 739 "pascal.y"
                           { }
#line 4362 "pascal.tab.c"
    break;

  case 341: /* case_statement: CASESY expression OFSY $@23 case_list case_tail ENDSY  */
#line 739 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4368 "pascal.tab.c"
    break;

  case 342: /* case_tail: %empty  */
#line 742 "pascal.y"
    { (yyval.node) = NULL; }
#line 4374 "pascal.tab.c"
    break;

  case 343: /* case_tail: ELSESY statement_list  */
#line 743 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4380 "pascal.tab.c"
    break;

  case 344: /* case_tail: OTHERWISESY statement_list  */
#line 744 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4386 "pascal.tab.c"
    break;

  case 345: /* case_list: case_list SEMICOLON case_elem  */
#line 747 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4392 "pascal.tab.c"
    break;

  case 346: /* case_list: case_elem  */
#line 748 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4398 "pascal.tab.c"
    break;

  case 347: /* case_elem: constant_list COLON body_stmt  */
#line 751 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4404 "pascal.tab.c"
    break;

  case 348: /* case_elem: %empty  */
#line 752 "pascal.y"
     { (yyval.node) = NULL; }
#line 4410 "pascal.tab.c"
    break;

  case 349: /* constant_list: constant_list COMMA case_label  */
#line 755 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4416 "pascal.tab.c"
    break;

  case 350: /* constant_list: case_label  */
#line 756 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4422 "pascal.tab.c"
    break;

  case 351: /* case_label: b_expr  */
#line 759 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4428 "pascal.tab.c"
    break;

  case 352: /* case_label: b_expr DOTDOT b_expr  */
#line 760 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4434 "pascal.tab.c"
    break;

  case 353: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 763 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4440 "pascal.tab.c"
    break;

  case 354: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 766 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4446 "pascal.tab.c"
    break;

  case 355: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 769 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4452 "pascal.tab.c"
    break;

  case 356: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 770 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4458 "pascal.tab.c"
    break;

  case 357: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 771 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4464 "pascal.tab.c"
    break;

  case 358: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 774 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4470 "pascal.tab.c"
    break;

  case 359: /* with_open: with_open COMMA selector  */
#line 777 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4476 "pascal.tab.c"
    break;

  case 360: /* with_open: selector  */
#line 778 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4482 "pascal.tab.c"
    break;

  case 361: /* expression: simple_expression  */
#line 785 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4488 "pascal.tab.c"
    break;

  case 362: /* expression: expression INOP simple_expression  */
#line 786 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4494 "pascal.tab.c"
    break;

  case 363: /* expression: expression LTOP simple_expression  */
#line 787 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4500 "pascal.tab.c"
    break;

  case 364: /* expression: expression LEOP simple_expression  */
#line 788 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4506 "pascal.tab.c"
    break;

  case 365: /* expression: expression GTOP simple_expression  */
#line 789 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4512 "pascal.tab.c"
    break;

  case 366: /* expression: expression GEOP simple_expression  */
#line 790 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4518 "pascal.tab.c"
    break;

  case 367: /* expression: expression NEOP simple_expression  */
#line 791 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4524 "pascal.tab.c"
    break;

  case 368: /* expression: expression EQOP simple_expression  */
#line 792 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4530 "pascal.tab.c"
    break;

  case 369: /* expression: expression ISOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4536 "pascal.tab.c"
    break;

  case 370: /* simple_expression: term  */
#line 796 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4542 "pascal.tab.c"
    break;

  case 371: /* simple_expression: PLUS term  */
#line 797 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4548 "pascal.tab.c"
    break;

  case 372: /* simple_expression: MINUS term  */
#line 798 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4554 "pascal.tab.c"
    break;

  case 373: /* simple_expression: simple_expression PLUS term  */
#line 799 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4560 "pascal.tab.c"
    break;

  case 374: /* simple_expression: simple_expression MINUS term  */
#line 800 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4566 "pascal.tab.c"
    break;

  case 375: /* simple_expression: simple_expression OROP term  */
#line 801 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4572 "pascal.tab.c"
    break;

  case 376: /* simple_expression: simple_expression XOROP term  */
#line 802 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4578 "pascal.tab.c"
    break;

  case 377: /* term: power  */
#line 805 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4584 "pascal.tab.c"
    break;

  case 378: /* term: term MUL power  */
#line 806 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4590 "pascal.tab.c"
    break;

  case 379: /* term: term RDIV power  */
#line 807 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4596 "pascal.tab.c"
    break;

  case 380: /* term: term IDIV power  */
#line 808 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4602 "pascal.tab.c"
    break;

  case 381: /* term: term IMOD power  */
#line 809 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4608 "pascal.tab.c"
    break;

  case 382: /* term: term ANDOP power  */
#line 810 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4614 "pascal.tab.c"
    break;

  case 383: /* term: term MUL MINUS power  */
#line 811 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4620 "pascal.tab.c"
    break;

  case 384: /* term: term RDIV MINUS power  */
#line 812 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4626 "pascal.tab.c"
    break;

  case 385: /* term: term IDIV MINUS power  */
#line 813 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4632 "pascal.tab.c"
    break;

  case 386: /* term: term IMOD MINUS power  */
#line 814 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4638 "pascal.tab.c"
    break;

  case 387: /* term: term ANDOP MINUS power  */
#line 815 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4644 "pascal.tab.c"
    break;

  case 388: /* term: term SHIFTOP power  */
#line 816 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4650 "pascal.tab.c"
    break;

  case 389: /* term: term ASOP power  */
#line 817 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4656 "pascal.tab.c"
    break;

  case 390: /* power: factor  */
#line 820 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4662 "pascal.tab.c"
    break;

  case 391: /* power: power POWOP factor  */
#line 821 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4668 "pascal.tab.c"
    break;

  case 392: /* factor: selector  */
#line 824 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4674 "pascal.tab.c"
    break;

  case 393: /* factor: call_with_args  */
#line 825 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4680 "pascal.tab.c"
    break;

  case 394: /* factor: INTCONST  */
#line 826 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4686 "pascal.tab.c"
    break;

  case 395: /* factor: REALCONST  */
#line 827 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4692 "pascal.tab.c"
    break;

  case 396: /* factor: STRINGCONST  */
#line 828 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4698 "pascal.tab.c"
    break;

  case 397: /* factor: CHARCODE  */
#line 829 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4704 "pascal.tab.c"
    break;

  case 398: /* factor: LPARENT expression RPARENT  */
#line 830 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4710 "pascal.tab.c"
    break;

  case 399: /* factor: inherited_call  */
#line 831 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4716 "pascal.tab.c"
    break;

  case 400: /* factor: NOTSY factor  */
#line 832 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4722 "pascal.tab.c"
    break;

  case 401: /* factor: ATSIGN factor  */
#line 833 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4728 "pascal.tab.c"
    break;

  case 402: /* factor: LBRACK RBRACK  */
#line 834 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4734 "pascal.tab.c"
    break;

  case 403: /* factor: LBRACK set_member_list RBRACK  */
#line 835 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4740 "pascal.tab.c"
    break;

  case 404: /* set_member_list: set_member  */
#line 838 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4746 "pascal.tab.c"
    break;

  case 405: /* set_member_list: set_member_list COMMA set_member  */
#line 839 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4752 "pascal.tab.c"
    break;

  case 406: /* set_member: expression  */
#line 842 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4758 "pascal.tab.c"
    break;

  case 407: /* set_member: expression DOTDOT expression  */
#line 843 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4764 "pascal.tab.c"
    break;


#line 4768 "pascal.tab.c"

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

#line 849 "pascal.y"

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
