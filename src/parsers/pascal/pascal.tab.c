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
#define YYLAST   1856

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  130
/* YYNRULES -- Number of rules.  */
#define YYNRULES  409
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  985

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
     480,   481,   482,   485,   488,   491,   492,   493,   496,   497,
     500,   501,   504,   505,   508,   509,   510,   513,   514,   515,
     515,   516,   516,   517,   517,   518,   518,   519,   519,   520,
     520,   521,   521,   522,   522,   523,   523,   526,   526,   529,
     529,   532,   532,   535,   535,   539,   539,   543,   543,   548,
     551,   552,   553,   556,   557,   560,   561,   562,   563,   564,
     565,   566,   567,   570,   571,   572,   573,   574,   577,   578,
     588,   589,   590,   593,   594,   597,   598,   599,   600,   601,
     602,   605,   606,   609,   612,   613,   616,   619,   619,   620,
     620,   623,   623,   626,   627,   628,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   648,   649,   650,   651,   654,   655,   656,   659,   660,
     663,   664,   667,   668,   669,   672,   673,   674,   675,   678,
     679,   680,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   697,   698,   701,   702,   703,   704,
     707,   708,   709,   710,   711,   712,   715,   716,   717,   718,
     719,   720,   721,   722,   723,   726,   727,   730,   733,   734,
     737,   738,   741,   741,   744,   745,   746,   749,   750,   753,
     754,   757,   758,   761,   762,   765,   768,   771,   772,   773,
     776,   779,   780,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   798,   799,   800,   801,   802,   803,   804,   807,
     808,   809,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   822,   823,   826,   827,   828,   829,   830,   831,
     832,   833,   834,   835,   836,   837,   840,   841,   844,   845
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

#define YYPACT_NINF (-559)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-335)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      65,   -10,    16,   158,  -559,   138,  1031,  -559,  -559,    19,
    -559,    45,  -559,  -559,   395,   133,    16,   145,   148,   148,
     161,  -559,   278,    64,   191,   222,   268,  -559,  -559,  -559,
     249,   148,   225,   328,   270,  -559,  -559,    93,   475,   133,
    -559,   147,   329,    23,  -559,   148,  -559,   313,   154,   242,
     221,   862,   370,   318,   278,  -559,   327,   348,   362,   430,
     445,   410,   467,  -559,   245,  -559,  -559,  -559,  -559,   573,
    1422,   326,  -559,  -559,    16,   148,   490,   463,  -559,   503,
     326,  -559,   148,   512,    81,  -559,  -559,   677,   538,  -559,
    1422,   332,  1422,  -559,  1422,  1422,   557,  -559,  1157,  -559,
    1422,   571,   594,  -559,   493,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,   664,   509,  -559,   250,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,   148,   629,   576,  -559,   603,   637,
     651,   606,   670,   704,   707,  -559,   699,  1809,  -559,  -559,
     775,   775,   775,  1222,  1422,   775,  -559,  -559,  -559,   672,
     172,   571,   700,  -559,   730,   740,   559,   414,   586,   702,
    -559,   721,   718,  -559,   718,   816,   816,    -6,   711,  -559,
    -559,  -559,  -559,  1507,   235,   315,  -559,  -559,  -559,  -559,
     759,   464,  -559,   398,   305,   329,   316,  -559,    73,   788,
    -559,    58,   367,  -559,  -559,   738,   862,  -559,  -559,   719,
     409,  1023,   730,   740,    37,  1207,   239,   666,  1654,   144,
     144,  1247,  1618,    46,  1762,   751,   768,  1272,   747,  -559,
    1422,   760,  -559,  1422,  1297,  1422,   783,  -559,  1422,  1422,
    1422,  1422,  1422,   389,    79,   139,   850,   148,   784,   793,
     157,  -559,  -559,   148,   148,  -559,   133,    16,  -559,   796,
     148,   802,   278,  -559,  -559,   586,   586,  -559,  1672,   393,
    -559,  1691,  -559,   803,  -559,  1422,  1422,  1422,  1422,  1422,
    1422,  1422,  1422,   775,   775,   775,   775,  1513,  1538,  1563,
    1588,  1613,   775,   775,   775,   809,   231,  -559,   178,   866,
     825,   532,   305,   305,     5,  -559,  -559,   474,  -559,  1618,
     819,  1084,  1084,   824,   231,   631,   829,   833,   836,  1125,
     842,   856,   868,   903,    60,  -559,   681,  1447,    34,   914,
     917,  -559,   562,   562,  1084,   562,   562,   562,   562,  -559,
     463,   148,   880,   148,   882,  -559,   148,    41,  -559,   745,
    -559,  -559,  1447,   883,  -559,   463,  -559,   237,  -559,  1422,
    1422,  1422,  -559,  -559,   332,  -559,  -559,  -559,  -559,  1175,
    -559,  -559,  -559,   479,  -559,   779,   940,    97,  -559,   869,
    -559,  1322,  1618,  -559,   516,   898,  1762,   899,   601,  1762,
    -559,   529,  1762,  1759,   623,  1762,  1762,  1762,  1762,   912,
    -559,  -559,   231,  -559,   415,  -559,  -559,  -559,  -559,   463,
     463,   427,   450,   133,   167,   998,  -559,   148,  -559,   278,
    1422,  1422,  -559,   763,   902,   414,   414,   414,   414,   414,
     414,   414,   414,   586,   586,   586,   586,   775,   702,   775,
     702,   775,   702,   775,   702,   775,   702,   702,   702,  -559,
     908,     5,   539,  -559,  -559,   754,   148,   896,   148,   900,
    -559,   148,    72,  -559,   771,   901,   237,   905,   711,  -559,
     468,   506,   562,   562,  1084,   668,  -559,   276,   333,  -559,
     279,  -559,   548,   907,  -559,   909,   141,   301,   918,   920,
     922,  -559,   817,  -559,  -559,   919,  -559,   177,  -559,  -559,
    -559,   216,   926,  -559,  -559,  1472,   999,  1762,   326,  1618,
     326,  1618,   216,   333,   333,   634,  -559,  -559,  -559,  -559,
     463,  1002,   846,   718,   857,   718,   861,   170,  -559,    26,
      47,  1005,  1008,   463,   943,  1009,   102,  -559,  1010,   461,
    1066,  1762,  -559,   862,  -559,   740,  1084,   970,  1422,  -559,
    1422,  1422,  1618,  -559,   941,   240,   218,  -559,   259,  -559,
     560,   472,  -559,  1347,  1372,  1422,  -559,  -559,  1397,  -559,
     326,   564,   963,   463,   463,   463,   463,  1021,  1022,  -559,
    -559,  -559,  -559,  -559,  1013,   463,   463,  1762,  -559,   702,
     702,   702,   702,   702,  -559,   231,   967,  -559,   864,   718,
     865,   718,   870,   181,  -559,    26,  -559,  1003,  -559,  -559,
    -559,   333,   333,   426,   968,  1084,   562,   562,  -559,   562,
     562,   562,   562,  -559,  -559,  -559,  -559,  -559,   969,   971,
    -559,  -559,   984,   326,   463,   463,   216,  -559,  -559,   463,
     463,  -559,   190,   326,   201,   990,    74,  1691,  -559,  -559,
     682,  -559,  -559,  1019,  1045,  -559,    26,  -559,    26,   993,
      26,  -559,  1026,   179,  -559,  1011,  1053,  1054,  -559,  -559,
    -559,  1017,   237,  -559,   986,  -559,  1422,  1422,  -559,  -559,
     451,    75,  -559,   893,  -559,  -559,  -559,  1709,  1728,  -559,
      28,  -559,   869,  -559,  -559,  -559,  -559,  -559,  -559,   593,
    -559,   595,  1762,  -559,   614,  1064,  1079,   326,  1080,  1030,
    1032,  1034,  -559,  -559,   463,   463,  1093,  1093,  -559,  1069,
    1096,  1042,  -559,  -559,    26,  -559,    26,  1043,    26,  -559,
    -559,  1033,  -559,  -559,   469,   333,   333,  -559,  -559,  -559,
    -559,   884,   463,   463,  -559,  -559,   463,   463,   170,  1046,
    1105,  -559,  1062,  1097,   463,   463,  -559,  -559,  -559,  1047,
    -559,  1447,  1447,   350,  -559,  1618,  1098,  -559,  -559,  1052,
    1076,  -559,   237,  -559,    36,  1061,  1422,  -559,  -559,  1135,
     237,  1139,  -559,    59,  1083,  1126,  -559,  1084,  1084,  -559,
    -559,  1100,  1084,  -559,  -559,  1422,  1422,  -559,  1070,  -559,
     286,  -559,  -559,  -559,  -559,  -559,  1140,  -559,   237,   237,
     237,  1052,  1052,  1144,  1090,  1093,  1094,  -559,   237,  1006,
    -559,  -559,   237,  -559,  -559,  -559,  -559,  1147,  1103,   463,
     463,  1149,  1160,    55,   231,  -559,   237,  -559,  1106,  1107,
    1084,  1141,  -559,  -559,   622,  -559,  1112,  1110,  -559,  1052,
    -559,  1422,  -559,  -559,  -559,  -559,  1762,  -559,  1176,  -559,
    1177,  -559,  -559,  -559,   634,  -559,  1093,  1093,  -559,  -559,
    -559,  1743,  1762,  -559,  1163,  -559,  -559,  1052,  1179,  1181,
    1184,  -559,  -559,  -559,   237,  -559,  1052,  1185,  -559,  -559,
    -559,   237,  1186,  1134,  -559,  -559,  -559,  -559,  -559,   237,
     237,  1191,  -559,   894,  -559,  1447,  -559,  1447,  1174,  -559,
    -559,  1199,  1762,  -559,  1052,  -559,  1201,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,  1203,  1204,  1052,  1205,  -559,
    1206,  -559,   237,    44,  -559,  -559,  1084,  1156,  1084,  -559,
    -559,   326,   326,  1217,  -559,  1052,  -559,  -559,  -559,  1218,
    1052,  1052,  1052,  -559,  -559,  -559,  -559,  1052,  -559,  1219,
    -559,   698,  -559,  -559,  1191,  -559,  -559,  -559,  -559,  1224,
    -559,  -559,  -559,  -559,  1225,  1052,  -559,  -559,  -559,   216,
    1228,  -559,  1233,  1235,  1236,  -559,  -559,  1182,  -559,  -559,
    -559,  -559,  1237,  -559,  -559
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   259,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   219,   255,    16,   183,     0,     0,   219,
       0,   280,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   127,    38,    18,     0,     0,     0,   222,   182,     0,
     127,    19,     0,     0,   222,   257,   256,     0,     0,   259,
       0,     0,     0,   259,     0,     0,     0,   299,   295,   259,
     290,   279,     0,    69,   296,    68,   260,   263,   276,   277,
     278,   267,     0,   298,   266,     0,   268,   269,   270,   271,
     272,   273,   274,   275,     0,   127,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   396,   399,   397,   398,
     334,     0,   301,   401,   395,   394,     0,   363,   372,   379,
     392,   107,   242,   126,   242,     0,     0,     0,     0,    51,
      54,    52,    53,   129,   109,   172,   139,    76,    75,   132,
       0,     0,    80,     0,   134,    64,     0,   219,     0,     0,
     254,     0,     0,   219,   193,     0,   280,   338,   339,     0,
       0,     0,     0,   362,     0,     0,     0,     0,     0,   280,
     280,     0,     0,     0,   291,   292,     0,     0,     0,   333,
       0,     0,   331,     0,     0,     0,     0,   328,     0,     0,
       0,     0,     0,     0,   107,   109,     0,     0,     0,     0,
       0,   219,   219,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   402,   373,   374,   404,   408,     0,
     406,     0,   403,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    88,
       0,    50,   135,   136,    81,    83,   253,     0,   252,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
       0,     0,     0,     0,   177,   162,     0,     0,     0,    96,
       0,    93,     0,     0,     0,     0,     0,     0,     0,   219,
     222,     0,     0,     0,     0,   221,   255,     0,   224,     0,
     238,   184,     0,     0,   219,   222,    12,     0,   258,     0,
       0,     0,   261,   261,     0,   342,   337,   261,   300,   295,
     264,   265,   305,     0,   315,   394,   316,     0,    78,   259,
     259,     0,     0,   304,     0,   332,   325,   329,     0,   336,
     307,     0,   320,   297,     0,   321,   322,   323,   324,     0,
      61,    60,     0,    57,     0,   219,   219,   219,   219,   222,
     222,     0,     0,    24,     0,    31,   219,    26,   219,    25,
       0,     0,   405,   400,   327,   364,   365,   366,   367,   368,
     369,   370,   371,   377,   375,   376,   378,     0,   382,     0,
     383,     0,   384,     0,   380,     0,   381,   390,   391,   393,
       0,    72,     0,    70,    73,     0,     0,     0,     0,     0,
     241,   255,     0,   244,     0,     0,     0,     0,     0,   128,
       0,   131,     0,     0,     0,    50,   156,     0,   146,   151,
       0,    82,     0,     0,   100,     0,   111,     0,     0,     0,
       0,   110,     0,   219,   219,     0,   170,     0,   219,   219,
      98,   172,     0,   160,   163,     0,     0,    42,   127,     0,
     127,     0,   172,   137,   138,   133,   143,   144,   142,   145,
     222,     0,     0,   242,     0,   242,     0,     0,   220,     0,
     189,     0,     0,   222,     0,     0,   104,   106,     0,     0,
       0,   356,   355,   280,   360,   361,   350,   340,     0,   302,
       0,     0,     0,    67,     0,     0,     0,   285,     0,   293,
       0,     0,   303,     0,     0,     0,   330,   306,     0,   326,
     127,     0,     0,   222,   222,   222,   222,     0,     0,   219,
     219,    23,   259,   259,    33,   222,   222,   409,   407,   387,
     388,   389,   385,   386,   108,     0,   107,   102,     0,   242,
       0,   242,     0,     0,   240,     0,    89,    90,   140,   251,
      77,   147,   148,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,   141,   109,   219,   219,   112,     0,     0,
     219,   219,   121,   127,   222,   222,   172,   219,   219,   222,
     222,   161,     0,   127,   334,     0,     0,    42,    41,    87,
       0,    97,    95,     0,     0,   238,     0,   225,     0,     0,
       0,   223,     0,   233,   237,   228,     0,     0,    12,   185,
     186,     0,     0,   194,     0,   238,     0,     0,   261,   262,
     353,   344,   348,     0,   352,   261,   314,   336,   317,    79,
       0,   281,   287,   282,   259,   284,   294,    74,   313,     0,
     311,     0,   335,   309,     0,     0,   109,   127,     0,     0,
       0,     0,   238,   238,   222,   222,    29,    30,   259,     0,
       0,     0,    71,   109,     0,   249,     0,     0,     0,   243,
     245,     0,   159,   157,     0,   149,   150,   153,   154,   152,
     155,     0,   222,   222,   219,   219,   222,   222,     0,     0,
       0,   165,     0,     0,   222,   222,   167,   168,   174,     0,
     164,     0,     0,     0,    44,     0,     0,    94,   238,   203,
     229,   227,     0,   232,     0,     0,     0,   187,   239,     0,
       0,     0,   105,   191,     0,     0,   359,     0,   350,   259,
     259,     0,     0,   261,   341,     0,     0,   261,     0,   286,
       0,   312,   310,   308,    59,    62,     0,   238,     0,     0,
       0,   207,   209,     0,     0,    32,     0,   238,     0,     0,
     247,   246,     0,   248,    91,   158,   101,     0,     0,   222,
     222,     0,     0,     0,     0,   113,     0,    99,     0,     0,
     181,     0,    48,    46,     0,    45,     0,     0,    84,   199,
      12,     0,   226,   236,   235,   234,   230,   190,     0,   238,
       0,    12,   261,   261,   354,   347,   346,   345,   343,   351,
     349,     0,   318,   289,     0,   283,    58,   211,     0,     0,
       0,    12,    12,   238,     0,    20,    27,     0,   103,   250,
     114,     0,     0,     0,   116,   117,   122,   123,   166,     0,
       0,   176,   179,     0,   174,     0,    43,     0,     0,    85,
      12,     0,   231,   238,   205,   188,     0,   358,   357,   319,
     261,    12,   238,   238,   238,     0,     0,   195,     0,   238,
       0,   118,     0,     0,   171,   169,   181,     0,   181,    47,
      49,   127,   127,     0,   204,   201,    12,   192,   288,     0,
     217,   215,   213,   208,   210,    12,   238,    28,   115,     0,
     120,     0,   178,   173,   175,    92,    86,   200,    12,     0,
     212,    12,    12,    12,     0,   197,   119,   125,   124,   172,
       0,   206,     0,     0,     0,   196,    12,     0,   202,   218,
     216,   214,     0,   180,   198
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -559,  -559,  -559,  -559,  -559,   -65,  1000,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  1004,   -28,  -339,  -559,  -559,   -58,
    1012,   -33,  1007,    40,   -70,   -36,  -270,  -558,   -64,   281,
    -262,   -78,  -559,  -559,  -559,  -415,   665,  -251,  -559,  -559,
    -559,  -559,  -206,  -559,   607,  -276,  -283,  -253,  -493,  -559,
    -280,  -559,  -559,   358,  -559,   334,   335,  1016,   -40,  -559,
    -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,   572,   203,   525,   750,  -531,
     106,  -145,  -559,   675,  -559,   811,    12,  -559,   -85,  -559,
    -559,  -333,  -559,  -173,   567,  -559,  -559,   588,  -559,   -34,
    -559,  -559,   -39,  -189,   734,  -559,    62,  1050,  -559,  -559,
    -559,  -559,  -559,  -559,  -559,   496,  -518,   497,  -559,  -559,
    -559,  -559,  -559,   446,  1584,   -83,  -153,  -107,  -559,   867
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   574,   709,    37,    39,    40,   496,   834,   636,   466,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   899,   932,   502,   528,   287,   305,   481,   739,
     923,   181,   182,   183,   184,   670,   468,   469,   313,   314,
     315,   633,   969,   830,   493,   891,   892,    45,    46,    28,
     658,   851,   346,   945,   976,   900,   958,   840,   936,   871,
     872,   911,   963,   962,   961,    77,   189,   337,   338,   655,
     520,   289,   452,   453,   297,   298,   316,    29,    50,   196,
      51,   532,   533,   106,   107,   108,   546,   547,   109,   153,
     111,   112,   154,   363,   364,   114,   155,   384,   116,   117,
     118,   119,   536,   781,   671,   672,   893,   674,   120,   121,
     122,   123,   204,   366,   157,   158,   159,   160,   259,   260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,   521,   200,    78,   368,   178,   206,   643,
     177,    72,   113,   176,   213,   104,   178,   110,   673,   290,
     534,   127,   176,   348,   537,   467,   470,   712,   374,   486,
      47,    48,   652,   254,   472,   381,   445,   460,   262,   503,
     504,   597,    11,    64,   843,   787,   517,   236,   505,   950,
      33,    85,   202,   299,   353,   177,   656,    47,   255,   256,
     517,   178,   498,   341,   720,   491,     8,   176,   850,     1,
     294,    75,   506,   507,   508,   509,     2,   593,    30,   753,
     778,   331,    76,    31,   390,   492,   194,   186,   788,   102,
      56,   779,    10,   368,   192,   354,   499,   177,    68,   332,
      57,   212,   653,   178,    34,   333,   518,   176,   176,   334,
     551,   342,   844,   115,   185,   760,   780,   761,   886,   763,
     951,   102,   561,   657,   428,   430,   432,   434,   436,   437,
     438,   369,   370,   743,   343,   657,   233,   594,   335,   754,
      58,   285,   177,   286,   391,   188,   617,    87,   178,   336,
     299,    69,    73,   203,   176,   542,   635,   113,    12,    81,
     104,   664,   110,    88,    89,    90,   368,   303,    91,    92,
     113,   113,   571,   104,   104,   110,   110,   439,   331,   601,
     602,    93,   550,   810,    94,   811,   446,   813,   603,   446,
     423,   424,   425,   426,   543,   397,   332,    13,   212,  -255,
     339,  -255,   333,   392,   447,    74,   334,   447,    95,    38,
     448,   631,    79,   448,   449,   627,   177,   449,   748,    97,
     359,    43,   178,   682,    44,    74,    85,   299,    99,   177,
      65,   100,   101,   398,   177,   178,   211,    49,   765,   102,
     178,   176,   306,   450,    85,    85,   336,   771,   683,   394,
     749,    86,   307,   628,   451,   401,   402,   451,   115,   684,
     673,   751,    47,   303,    85,   211,   887,    59,   212,   356,
     681,   115,   115,   365,   579,   212,   580,   177,   581,   365,
     582,   525,   583,   178,   545,   548,   365,   195,   225,   685,
      82,    85,   309,   640,    61,   642,   310,   212,    60,   304,
     454,    83,   177,    79,    62,   311,   312,   441,   178,   226,
     135,   227,   228,   526,   176,   202,   865,   482,   229,   230,
     231,   232,   177,   725,   726,    63,   102,   618,   178,   724,
     606,   607,   102,   606,   607,   776,   679,   619,   161,   608,
      66,   306,   784,   512,   613,   514,    67,   842,   516,   325,
     326,   307,   162,   163,    52,   848,   727,   728,   729,   730,
     669,   327,   164,   731,   689,   691,   329,    78,   647,   694,
     649,    79,    53,    80,    79,    72,   127,   609,   610,   308,
     165,   166,   328,   868,   869,   870,   177,   167,    34,   611,
     168,   309,   178,   877,   126,   310,    95,   879,   169,   170,
     171,   172,   173,   128,   311,   312,   174,    97,   150,   175,
     612,   888,   832,   833,    85,   835,   535,   344,   124,    47,
     639,   102,   641,   125,   129,    79,   836,   102,   177,   177,
     177,   177,   351,   365,   178,   178,   178,   178,   130,   389,
     176,   176,   176,   176,   715,   731,   717,    79,   295,   177,
     860,   411,   322,   323,   863,   178,   412,   482,   588,   918,
     590,   273,   809,   592,   324,   562,   920,    35,   274,   275,
     318,    36,   177,    79,   924,   925,   977,   569,   178,   666,
     606,   607,   695,   667,   176,    79,   133,   706,   707,   131,
     177,   722,   276,   319,   113,   320,   178,   104,   321,   110,
     570,   854,   176,   265,   132,   606,   607,   949,    79,   266,
     267,   268,   269,   270,   271,   177,   156,   777,   600,   907,
     908,   178,   687,   606,   607,   177,   542,   188,    70,   339,
     542,   178,   458,   511,   815,    71,   201,   538,   205,   459,
     207,   208,   272,   134,   539,   740,   214,   220,   524,   837,
    -140,  -140,  -301,   177,  -301,   750,   929,   217,   930,   178,
    -140,  -140,  -140,   177,   264,   176,   187,   444,   221,   178,
     222,   223,  -140,   224,   538,   176,   177,   938,   177,   190,
     177,   552,   178,  -140,   178,   444,   178,   538,   193,   258,
     261,   457,   177,   769,   557,   115,   302,   585,   178,   790,
     365,   265,   567,   568,   586,   454,   585,   266,   267,   268,
     269,   270,   271,   614,   199,   365,   365,   209,   538,   796,
     365,    84,   585,   805,   237,   686,   464,   177,   527,   696,
     277,   278,   279,   178,   169,   170,   171,   172,   465,   176,
     272,   234,   280,   281,   177,   138,   177,   215,   177,   139,
     178,   538,   178,   538,   178,   162,   163,   473,   791,   555,
     792,   474,   238,   282,   556,   164,   376,   475,   283,   379,
     216,   382,   538,   444,   379,   385,   386,   387,   388,   793,
     895,   555,   241,   165,   166,   177,   559,   896,   606,   607,
     167,   178,   177,   168,   856,   857,   239,   176,   178,   357,
     177,   169,   170,   171,   172,   173,   178,   476,   265,   235,
     240,   477,   175,   644,   266,   267,   268,   269,   270,   271,
     478,   479,   480,   218,   102,   219,   661,   604,   177,   177,
     177,  -299,   605,  -299,   178,   178,   178,   527,   177,    79,
     755,   494,   177,   482,   178,   756,   242,   272,   178,   197,
     339,   759,   243,   198,   177,   244,   177,   349,   245,   330,
     178,   350,   178,   497,   217,   345,   698,   699,   700,   701,
     967,   773,   292,   293,   968,   901,   360,   361,   710,   711,
     473,   284,   288,   285,   587,   286,   906,   296,   497,   221,
     475,   222,   223,   340,   224,   529,   530,   531,   347,   263,
     654,   227,   228,    79,   177,   519,   915,   916,   801,   802,
     178,   177,   317,   399,   400,   371,   372,   178,   140,   177,
     177,   482,  -300,   375,  -300,   178,   178,   741,   742,    79,
     476,   595,   746,   747,   477,   933,   377,   143,   263,   144,
     227,   540,   145,   478,   479,   480,   939,   146,   147,   148,
     149,   150,   177,   955,   956,   393,   577,   258,   178,   383,
     395,   177,   177,   151,   839,    87,   444,   178,   178,   396,
     102,   959,   406,   176,   176,    79,   654,   623,   408,   414,
     964,    88,    89,    90,   440,   456,    91,    92,   169,   170,
     171,   172,   291,   970,   455,   461,   972,   973,   974,    93,
     471,   510,    94,   867,    79,   483,   646,   803,   804,   484,
     473,   982,   485,   876,   816,    79,   523,   648,   487,    79,
     475,   650,    79,    79,   714,   716,    95,   654,    79,   654,
     718,   654,   488,   490,    96,   817,   818,    97,    98,   821,
     822,   637,   500,   527,   489,   501,    99,   828,   829,   100,
     101,   782,   782,   783,   927,   904,   513,   102,   515,   522,
     476,   544,   553,   554,   477,   560,   558,   563,   564,   565,
     566,   584,   589,   478,   479,   480,   591,   596,   575,   917,
     576,   598,   265,   615,   626,   616,   677,   678,   266,   267,
     268,   269,   270,   271,   620,   654,   621,   654,   622,   654,
     541,   692,   632,   662,   638,    14,    15,   645,    16,   935,
     659,   675,   572,   660,   663,   665,   697,   680,   940,   941,
     942,   272,   882,   883,    17,   947,   702,   703,   708,   285,
      18,   721,   473,    19,    20,   573,   878,    22,    14,    15,
     352,    16,   475,   527,   723,   734,   738,   735,   752,   757,
     758,   527,   965,   762,   764,   624,   625,    17,   767,   768,
     629,   630,   772,    18,   766,   265,    19,    20,    21,   794,
      22,   266,   267,   268,   269,   270,   271,   770,    23,   527,
     527,   527,   476,   668,   795,   797,   477,    24,    25,   527,
     798,    26,   799,   527,   800,   478,   479,   480,    85,   806,
     852,   807,   808,   812,   272,   444,   824,   527,   265,   814,
     825,    23,   774,   775,   266,   267,   268,   269,   270,   271,
      24,    25,   826,   831,    26,   265,   838,   827,   657,   841,
     858,   266,   267,   268,   269,   270,   271,   845,   462,   463,
     847,   704,   705,   853,   849,   866,   864,   272,   464,   873,
     874,   306,   880,   875,   884,   527,   169,   170,   171,   172,
     465,   307,   527,   881,   272,   885,   889,   890,   265,   894,
     527,   527,   897,   898,   266,   267,   268,   269,   270,   271,
     910,   903,   905,  -255,   912,  -255,   913,   732,   733,   914,
     919,   921,   736,   737,   922,  -334,   926,   497,   497,   744,
     745,   309,   931,   527,   934,   310,   937,   272,   943,   944,
     946,   948,   846,  -334,   311,   312,  -334,   210,  -334,  -334,
     953,   211,   957,   960,   966,  -334,  -334,  -334,  -334,   971,
     975,   861,   862,   978,  -334,   355,  -334,  -334,   979,   211,
     980,   981,   984,  -334,  -334,  -334,  -334,   983,   405,   265,
     403,   713,   928,   212,   404,   266,   267,   268,   269,   270,
     271,   952,   954,   823,   409,   140,   407,   651,   719,   599,
     789,   212,   676,   378,   855,     0,   141,   142,   578,   859,
       0,     0,     0,     0,   143,   257,   144,   902,   272,   145,
     140,     0,     0,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,   819,   820,     0,   143,
     151,   144,   362,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   373,     0,   145,
     140,   497,   102,   497,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   380,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   549,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   688,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   690,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   693,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,     0,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   495,     0,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   495,     0,     0,   145,
       0,     0,   102,     0,   146,   147,   148,   149,   634,     0,
       0,   -50,   -50,     0,     0,   299,   140,     0,     0,     0,
     151,   -50,   -50,   -50,     0,     0,   300,   102,   427,   301,
       0,   302,     0,   -50,     0,   143,     0,   144,     0,     0,
     145,   140,     0,     0,   -50,   146,   147,   148,   149,   150,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
     143,   151,   144,   212,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   143,   151,   144,     0,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
     143,   151,   144,     0,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   435,     0,
       0,     0,   165,   166,     0,   143,   151,   144,     0,     0,
     145,     0,   168,   102,     0,   146,   147,   148,   149,   150,
     169,   170,   171,   172,   173,     0,   265,     0,     0,     0,
       0,   151,   266,   267,   268,   269,   270,   271,   102,     0,
       0,     0,     0,   102,   265,     0,     0,     0,     0,   358,
     266,   267,   268,   269,   270,   271,     0,     0,     0,     0,
       0,     0,     0,   265,     0,   272,     0,     0,   410,   266,
     267,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,   265,     0,   272,     0,     0,   413,   266,   267,   268,
     269,   270,   271,     0,     0,     0,     0,     0,     0,     0,
     265,     0,   272,     0,     0,   785,   266,   267,   268,   269,
     270,   271,     0,     0,     0,   265,     0,     0,   786,     0,
     272,   266,   267,   268,   269,   270,   271,  -327,     0,     0,
       0,     0,     0,     0,   265,     0,   909,     0,     0,   272,
     266,   267,   268,   269,   270,   271,     0,   246,  -327,   247,
    -327,  -327,   248,   558,   272,     0,     0,  -327,  -327,  -327,
    -327,     0,     0,     0,     0,   249,     0,     0,     0,     0,
       0,   250,     0,   272,     0,   251,     0,     0,   252,   415,
     416,   417,   418,   419,   420,   421,   422
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   342,    89,    45,   212,    71,    93,   502,
      80,    39,    51,    71,    99,    51,    80,    51,   536,   164,
     353,    54,    80,   196,   357,   301,   302,   585,   217,   309,
      18,    19,     6,   140,   304,   224,   287,   299,   145,   322,
     323,   456,     2,    31,     8,    17,     5,   125,   324,     5,
       5,     5,    91,    48,    17,   125,     9,    45,   141,   142,
       5,   125,    28,     5,   595,     5,    76,   125,     9,     4,
      76,    48,   325,   326,   327,   328,    11,     5,    59,     5,
       5,     8,    59,    64,     5,    25,     5,    75,    60,    95,
      26,    16,    76,   299,    82,    58,    62,   167,     5,    26,
      36,    96,    76,   167,    59,    32,    65,   165,   166,    36,
     372,    53,    76,    51,    74,   646,    41,   648,    63,   650,
      76,    95,   392,    76,   277,   278,   279,   280,   281,   282,
     283,    85,    86,   626,    76,    76,   124,    65,    65,    65,
      76,    62,   212,    64,     5,    64,     5,     3,   212,    76,
      48,    58,     5,    91,   212,    58,   495,   196,     0,     5,
     196,    59,   196,    19,    20,    21,   372,    28,    24,    25,
     209,   210,     5,   209,   210,   209,   210,   284,     8,   462,
     463,    37,   371,   714,    40,   716,     8,   718,   464,     8,
     273,   274,   275,   276,    97,    38,    26,    59,    96,    58,
     188,    60,    32,    64,    26,    58,    36,    26,    64,    76,
      32,   491,    58,    32,    36,    38,   286,    36,    28,    75,
      76,    76,   286,     5,    76,    58,     5,    48,    84,   299,
       5,    87,    88,    76,   304,   299,    64,    76,    59,    95,
     304,   299,    26,    65,     5,     5,    76,   662,    30,   237,
      60,    30,    36,    76,    76,   243,   244,    76,   196,    41,
     778,    60,   250,    28,     5,    64,   824,    76,    96,    30,
      30,   209,   210,   211,   427,    96,   429,   347,   431,   217,
     433,   346,   435,   347,   369,   370,   224,    84,    38,    30,
      48,     5,    76,   499,    26,   501,    80,    96,    76,    64,
     288,    59,   372,    58,    36,    89,    90,    76,   372,    59,
      65,    61,    62,    76,   372,   354,    30,   305,    68,    69,
      70,    71,   392,   606,   607,    76,    95,    26,   392,   605,
      54,    55,    95,    54,    55,   668,   542,    36,    12,    63,
      12,    26,   675,   331,    65,   333,    76,   762,   336,    44,
      45,    36,    26,    27,    76,   770,   609,   610,   611,   612,
     533,    56,    36,   614,   553,   554,    50,   407,   513,   558,
     515,    58,    94,    60,    58,   403,   409,    44,    45,    64,
      54,    55,    77,   798,   799,   800,   456,    61,    59,    56,
      64,    76,   456,   808,    76,    80,    64,   812,    72,    73,
      74,    75,    76,    76,    89,    90,    80,    75,    76,    83,
      77,   826,   751,   752,     5,    65,   354,    50,    48,   407,
     498,    95,   500,    53,    76,    58,    76,    95,   498,   499,
     500,   501,    23,   371,   498,   499,   500,   501,    76,    50,
     498,   499,   500,   501,   589,   696,   591,    58,   167,   519,
     783,    58,    54,    55,   787,   519,    63,   445,   446,   874,
     448,    47,   713,   451,    66,    50,   881,    72,    54,    55,
       6,    76,   542,    58,   889,   890,   969,    50,   542,    18,
      54,    55,   560,    22,   542,    58,    76,   572,   573,    59,
     560,    65,    78,    29,   533,    31,   560,   533,    34,   533,
      50,   777,   560,    42,    59,    54,    55,   922,    58,    48,
      49,    50,    51,    52,    53,   585,    70,    66,    50,   852,
     853,   585,    50,    54,    55,   595,    58,    64,    53,   517,
      58,   595,    58,   330,    65,    60,    90,    58,    92,    65,
      94,    95,    81,    76,    65,   623,   100,    38,   345,   755,
      44,    45,    59,   623,    61,   633,   895,    64,   897,   623,
      54,    55,    56,   633,     5,   623,    76,   286,    59,   633,
      61,    62,    66,    64,    58,   633,   646,   910,   648,    76,
     650,    65,   646,    77,   648,   304,   650,    58,    76,   143,
     144,    59,   662,   658,    65,   533,    64,    58,   662,   684,
     538,    42,   399,   400,    65,   593,    58,    48,    49,    50,
      51,    52,    53,    65,    76,   553,   554,    60,    58,   697,
     558,    49,    58,   708,    48,    65,    64,   697,   347,    65,
      44,    45,    46,   697,    72,    73,    74,    75,    76,   697,
      81,    12,    56,    57,   714,    72,   716,    76,   718,    76,
     714,    58,   716,    58,   718,    26,    27,    26,    65,    58,
      65,    30,    59,    77,    63,    36,   220,    36,    82,   223,
      76,   225,    58,   392,   228,   229,   230,   231,   232,    65,
      58,    58,    76,    54,    55,   755,    63,    65,    54,    55,
      61,   755,   762,    64,   779,   780,    59,   755,   762,    33,
     770,    72,    73,    74,    75,    76,   770,    76,    42,    80,
      59,    80,    83,   510,    48,    49,    50,    51,    52,    53,
      89,    90,    91,    59,    95,    61,   523,    59,   798,   799,
     800,    59,    64,    61,   798,   799,   800,   456,   808,    58,
      58,    60,   812,   731,   808,    63,    76,    81,   812,    72,
     738,   645,    48,    76,   824,    48,   826,    38,    59,   187,
     824,    42,   826,   317,    64,   193,   563,   564,   565,   566,
      72,   665,   165,   166,    76,   840,   209,   210,   575,   576,
      26,    79,    64,    62,    30,    64,   851,    76,   342,    59,
      36,    61,    62,     5,    64,   349,   350,   351,    60,    59,
     519,    61,    62,    58,   874,    60,   871,   872,   702,   703,
     874,   881,    53,   241,   242,    64,    48,   881,    43,   889,
     890,   809,    59,    76,    61,   889,   890,   624,   625,    58,
      76,    60,   629,   630,    80,   900,    76,    62,    59,    64,
      61,    62,    67,    89,    90,    91,   911,    72,    73,    74,
      75,    76,   922,   931,   932,     5,   410,   411,   922,    76,
      76,   931,   932,    88,   758,     3,   585,   931,   932,    76,
      95,   936,    76,   931,   932,    58,   595,    60,    76,    76,
     945,    19,    20,    21,    75,    60,    24,    25,    72,    73,
      74,    75,    76,   958,    28,    76,   961,   962,   963,    37,
      76,   329,    40,   797,    58,    76,    60,   704,   705,    76,
      26,   976,    76,   807,    30,    58,   344,    60,    76,    58,
      36,    60,    58,    58,    60,    60,    64,   646,    58,   648,
      60,   650,    76,    30,    72,   732,   733,    75,    76,   736,
     737,   495,    28,   662,    76,    28,    84,   744,   745,    87,
      88,    58,    58,    60,    60,   849,    76,    95,    76,    76,
      76,    92,    64,    64,    80,    53,    64,   395,   396,   397,
     398,    63,    76,    89,    90,    91,    76,    76,   406,   873,
     408,    76,    42,    76,    65,    76,   540,   541,    48,    49,
      50,    51,    52,    53,    76,   714,    76,   716,    76,   718,
      60,   555,    76,    60,     5,     7,     8,     5,    10,   903,
       5,    41,    14,     5,     5,     5,    53,    76,   912,   913,
     914,    81,   819,   820,    26,   919,     5,     5,    15,    62,
      32,    28,    26,    35,    36,    37,    30,    39,     7,     8,
      17,    10,    36,   762,    76,    76,    62,    76,    58,    30,
       5,   770,   946,    60,    28,   483,   484,    26,     5,     5,
     488,   489,    76,    32,    53,    42,    35,    36,    37,     5,
      39,    48,    49,    50,    51,    52,    53,    60,    80,   798,
     799,   800,    76,    17,     5,     5,    80,    89,    90,   808,
      60,    93,    60,   812,    60,    89,    90,    91,     5,    30,
      17,     5,    60,    60,    81,   824,    60,   826,    42,    76,
       5,    80,   666,   667,    48,    49,    50,    51,    52,    53,
      89,    90,    60,    76,    93,    42,    28,    30,    76,    53,
      30,    48,    49,    50,    51,    52,    53,    76,    54,    55,
       5,   569,   570,    17,     5,     5,    76,    81,    64,     5,
      60,    26,     5,    59,     5,   874,    72,    73,    74,    75,
      76,    36,   881,    60,    81,     5,    60,    60,    42,    28,
     889,   890,    60,    63,    48,    49,    50,    51,    52,    53,
      17,     5,     5,    58,     5,    60,     5,   615,   616,     5,
       5,     5,   620,   621,    60,    38,     5,   751,   752,   627,
     628,    76,    28,   922,     5,    80,     5,    81,     5,     5,
       5,     5,   766,    38,    89,    90,    59,    60,    61,    62,
      64,    64,     5,     5,     5,    68,    69,    70,    71,     5,
       5,   785,   786,     5,    59,    28,    61,    62,     5,    64,
       5,     5,     5,    68,    69,    70,    71,    65,   248,    42,
     246,   586,   894,    96,   247,    48,    49,    50,    51,    52,
      53,   926,   928,   738,   252,    43,   250,   517,   593,   458,
     682,    96,   538,   223,   778,    -1,    54,    55,   411,   782,
      -1,    -1,    -1,    -1,    62,    63,    64,   841,    81,    67,
      43,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,   734,   735,    -1,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      43,   895,    95,   897,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,
      -1,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    44,    45,    -1,    -1,    48,    43,    -1,    -1,    -1,
      88,    54,    55,    56,    -1,    -1,    59,    95,    55,    62,
      -1,    64,    -1,    66,    -1,    62,    -1,    64,    -1,    -1,
      67,    43,    -1,    -1,    77,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    96,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,
      -1,    -1,    54,    55,    -1,    62,    88,    64,    -1,    -1,
      67,    -1,    64,    95,    -1,    72,    73,    74,    75,    76,
      72,    73,    74,    75,    76,    -1,    42,    -1,    -1,    -1,
      -1,    88,    48,    49,    50,    51,    52,    53,    95,    -1,
      -1,    -1,    -1,    95,    42,    -1,    -1,    -1,    -1,    65,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    81,    -1,    -1,    66,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    81,    -1,    -1,    66,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    42,    -1,    -1,    60,    -1,
      81,    48,    49,    50,    51,    52,    53,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    63,    -1,    -1,    81,
      48,    49,    50,    51,    52,    53,    -1,     8,    59,    10,
      61,    62,    13,    64,    81,    -1,    -1,    68,    69,    70,
      71,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    81,    -1,    36,    -1,    -1,    39,   265,
     266,   267,   268,   269,   270,   271,   272
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
      65,    76,   180,   181,   184,    28,    60,    59,    58,    65,
     128,    76,    54,    55,    64,    76,   117,   143,   144,   145,
     143,    76,   124,    26,    30,    36,    76,    80,    89,    90,
      91,   136,   184,    76,    76,    76,   148,    76,    76,    76,
      30,     5,    25,   152,    60,    64,   114,   221,    28,    62,
      28,    28,   132,   144,   144,   143,   145,   145,   145,   145,
     173,   174,   184,    76,   184,    76,   184,     5,    65,    60,
     178,   114,    76,   173,   174,   103,    76,   127,   133,   221,
     221,   221,   189,   190,   189,   204,   210,   189,    58,    65,
      62,    60,    58,    97,    92,   186,   194,   195,   186,    65,
     201,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   173,   173,   173,   173,   174,   174,    50,
      50,     5,    14,    37,   109,   173,   173,   221,   227,   224,
     224,   224,   224,   224,    63,    58,    65,    30,   184,    76,
     184,    76,   184,     5,    65,    60,    76,   133,    76,   183,
      50,   144,   144,   143,    59,    64,    54,    55,    63,    44,
      45,    56,    77,    65,    65,    76,    76,     5,    26,    36,
      76,    76,    76,    60,   173,   173,    65,    38,    76,   173,
     173,   148,    76,   149,    76,   114,   116,   221,     5,   129,
     140,   129,   140,   146,   174,     5,    60,   179,    60,   179,
      60,   176,     6,    76,   127,   177,     9,    76,   158,     5,
       5,   174,    60,     5,    59,     5,    18,    22,    17,   191,
     143,   212,   213,   214,   215,    41,   202,   221,   221,   140,
      76,    30,     5,    30,    41,    30,    65,    50,    65,   201,
      65,   201,   221,    65,   201,   129,    65,    53,   174,   174,
     174,   174,     5,     5,   173,   173,   186,   186,    15,   110,
     174,   174,   125,   134,    60,   179,    60,   179,    60,   181,
     177,    28,    65,    76,   143,   144,   144,   145,   145,   145,
     145,   135,   173,   173,    76,    76,   173,   173,    62,   137,
     129,   174,   174,   146,   173,   173,   174,   174,    28,    60,
     129,    60,    58,     5,    65,    58,    63,    30,     5,   178,
     177,   177,    60,   177,    28,    59,    53,     5,     5,   103,
      60,   133,    76,   178,   221,   221,   189,    66,     5,    16,
      41,   211,    58,    60,   189,    66,    60,    17,    60,   195,
     186,    65,    65,    65,     5,     5,   129,     5,    60,    60,
      60,   178,   178,   174,   174,   186,    30,     5,    60,   135,
     177,   177,    60,   177,    76,    65,    30,   174,   174,   173,
     173,   174,   174,   175,    60,     5,    60,    30,   174,   174,
     151,    76,   114,   114,   115,    65,    76,   140,    28,   178,
     165,    53,   133,     8,    76,    76,   221,     5,   133,     5,
       9,   159,    17,    17,   143,   213,   186,   186,    30,   215,
     189,   221,   221,   189,    76,    30,     5,   178,   133,   133,
     133,   167,   168,     5,    60,    59,   178,   133,    30,   133,
       5,    60,   174,   174,     5,     5,    63,   125,   133,    60,
      60,   153,   154,   214,    28,    58,    65,    60,    63,   130,
     163,   103,   221,     5,   178,     5,   103,   189,   189,    63,
      17,   169,     5,     5,     5,   103,   103,   178,   133,     5,
     133,     5,    60,   138,   133,   133,     5,    60,   151,   114,
     114,    28,   131,   103,     5,   178,   166,     5,   189,   103,
     178,   178,   178,     5,     5,   161,     5,   178,     5,   133,
       5,    76,   154,    64,   153,   129,   129,     5,   164,   103,
       5,   172,   171,   170,   103,   178,     5,    72,    76,   150,
     103,     5,   103,   103,   103,     5,   162,   146,     5,     5,
       5,     5,   103,    65,     5
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
     148,   148,   148,   150,   151,   152,   152,   152,   153,   153,
     154,   154,   155,   155,   156,   156,   156,   157,   157,   158,
     157,   159,   157,   160,   157,   161,   157,   162,   157,   163,
     157,   164,   157,   165,   157,   166,   157,   167,   157,   168,
     157,   169,   157,   170,   157,   171,   157,   172,   157,   173,
     174,   174,   174,   175,   175,   176,   176,   176,   176,   176,
     176,   176,   176,   177,   177,   177,   177,   177,   178,   178,
     179,   179,   179,   180,   180,   181,   181,   181,   181,   181,
     181,   182,   182,   183,   184,   184,   185,   187,   186,   188,
     186,   190,   189,   191,   191,   191,   192,   192,   192,   192,
     192,   192,   192,   192,   192,   192,   192,   192,   192,   192,
     192,   193,   193,   193,   193,   194,   194,   194,   195,   195,
     196,   196,   197,   197,   197,   198,   198,   198,   198,   199,
     199,   199,   200,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   201,   201,   202,   202,   202,   202,
     203,   203,   203,   203,   203,   203,   204,   204,   204,   204,
     204,   204,   204,   204,   204,   205,   205,   206,   207,   207,
     208,   208,   210,   209,   211,   211,   211,   212,   212,   213,
     213,   214,   214,   215,   215,   216,   217,   218,   218,   218,
     219,   220,   220,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   222,   222,   222,   222,   222,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   226,   226,   227,   227
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
       2,     7,     0,     0,     0,     7,     5,     0,     3,     1,
       6,     0,     2,     1,     4,     6,     6,     8,    10,     0,
       9,     0,    11,     0,     7,     0,    13,     0,    15,     0,
      12,     0,    14,     0,    11,     0,    13,     0,    11,     0,
      11,     0,    12,     0,    14,     0,    14,     0,    14,     0,
       3,     2,     0,     3,     1,     3,     5,     4,     3,     4,
       5,     6,     4,     1,     3,     3,     3,     1,     0,     3,
       3,     2,     0,     3,     1,     3,     4,     4,     4,     3,
       5,     3,     1,     1,     3,     1,     3,     0,     4,     0,
       2,     0,     2,     1,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     5,     5,     7,     5,     1,     3,     2,     6,     4,
       1,     2,     2,     4,     5,     1,     1,     3,     1,     1,
       3,     1,     4,     4,     3,     3,     4,     3,     6,     5,
       6,     5,     6,     5,     3,     1,     1,     3,     5,     6,
       3,     3,     3,     3,     3,     3,     4,     3,     2,     3,
       4,     2,     3,     2,     1,     3,     1,     3,     2,     2,
       4,     6,     0,     7,     0,     2,     2,     3,     1,     3,
       0,     3,     1,     1,     3,     4,     4,     8,     8,     6,
       4,     3,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       3,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       3,     1,     2,     2,     2,     3,     1,     3,     1,     3
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
#line 2303 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2309 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2315 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2321 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2327 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2333 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2339 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2345 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2351 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2357 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2363 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2369 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2375 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2381 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2387 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2393 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2399 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2411 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2423 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2429 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2435 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2441 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2447 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2477 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2489 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2495 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2501 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2507 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2525 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2531 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2543 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2549 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2555 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2561 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2579 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2585 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2591 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2597 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2603 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2609 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2615 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2621 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2639 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2645 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2651 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2657 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2663 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2669 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2681 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2687 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2699 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2705 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2711 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2723 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2729 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2735 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2741 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2747 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2753 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2771 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2783 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2795 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2801 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2813 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2837 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2843 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2849 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2855 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2861 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2885 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2897 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2903 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2909 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2915 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2921 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2927 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2933 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2939 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2945 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2951 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2957 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2963 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2969 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2975 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2981 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2987 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2993 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2999 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3005 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3011 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3017 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3023 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3029 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3035 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3047 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3053 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3059 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3065 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3071 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3077 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3083 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3089 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3095 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3119 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3125 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT PERIOD IDENT  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3131 "pascal.tab.c"
    break;

  case 141: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 440 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3137 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term MUL b_factor  */
#line 441 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IDIV b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term IMOD b_factor  */
#line 443 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3155 "pascal.tab.c"
    break;

  case 145: /* lo_term: lo_term SHIFTOP b_factor  */
#line 444 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 146: /* b_expr: b_term  */
#line 447 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3167 "pascal.tab.c"
    break;

  case 147: /* b_expr: PLUS b_term  */
#line 448 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3173 "pascal.tab.c"
    break;

  case 148: /* b_expr: MINUS b_term  */
#line 449 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr PLUS b_term  */
#line 450 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 150: /* b_expr: b_expr MINUS b_term  */
#line 451 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3191 "pascal.tab.c"
    break;

  case 151: /* b_term: b_factor  */
#line 454 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3197 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term MUL b_factor  */
#line 455 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3203 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IDIV b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term IMOD b_factor  */
#line 457 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3215 "pascal.tab.c"
    break;

  case 155: /* b_term: b_term SHIFTOP b_factor  */
#line 458 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3221 "pascal.tab.c"
    break;

  case 156: /* b_factor: scalar_constant  */
#line 461 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3227 "pascal.tab.c"
    break;

  case 157: /* b_factor: IDENT PERIOD IDENT  */
#line 462 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3233 "pascal.tab.c"
    break;

  case 158: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 463 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 159: /* b_factor: LPARENT b_expr RPARENT  */
#line 464 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3245 "pascal.tab.c"
    break;

  case 160: /* record_body: record_field_list record_case_opt  */
#line 467 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3251 "pascal.tab.c"
    break;

  case 161: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 470 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 162: /* record_field_list: record_field  */
#line 471 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 163: /* $@4: %empty  */
#line 474 "pascal.y"
                  { }
#line 3269 "pascal.tab.c"
    break;

  case 164: /* record_field: id_list COLON $@4 type  */
#line 474 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3275 "pascal.tab.c"
    break;

  case 165: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3281 "pascal.tab.c"
    break;

  case 166: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 476 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3287 "pascal.tab.c"
    break;

  case 167: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 477 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3293 "pascal.tab.c"
    break;

  case 168: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 478 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3299 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3305 "pascal.tab.c"
    break;

  case 170: /* record_field: IDENT record_field  */
#line 480 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3311 "pascal.tab.c"
    break;

  case 171: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 481 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3317 "pascal.tab.c"
    break;

  case 172: /* record_field: %empty  */
#line 482 "pascal.y"
     { (yyval.node) = NULL; }
#line 3323 "pascal.tab.c"
    break;

  case 173: /* case_arm_mark: %empty  */
#line 485 "pascal.y"
    { }
#line 3329 "pascal.tab.c"
    break;

  case 174: /* case_open: %empty  */
#line 488 "pascal.y"
    { }
#line 3335 "pascal.tab.c"
    break;

  case 175: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 491 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3341 "pascal.tab.c"
    break;

  case 176: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 492 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3347 "pascal.tab.c"
    break;

  case 177: /* record_case_opt: %empty  */
#line 493 "pascal.y"
     { (yyval.node) = NULL; }
#line 3353 "pascal.tab.c"
    break;

  case 178: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 496 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3359 "pascal.tab.c"
    break;

  case 179: /* record_case_list: record_case_arm  */
#line 497 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 180: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 500 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3371 "pascal.tab.c"
    break;

  case 181: /* record_case_arm: %empty  */
#line 501 "pascal.y"
     { (yyval.node) = NULL; }
#line 3377 "pascal.tab.c"
    break;

  case 182: /* var_decl_list: var_decl_list var_decl  */
#line 504 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3383 "pascal.tab.c"
    break;

  case 183: /* var_decl_list: var_decl  */
#line 505 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3389 "pascal.tab.c"
    break;

  case 184: /* var_decl: id_list COLON type SEMICOLON  */
#line 508 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3395 "pascal.tab.c"
    break;

  case 185: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 509 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3401 "pascal.tab.c"
    break;

  case 186: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 510 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3407 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 513 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3413 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3419 "pascal.tab.c"
    break;

  case 189: /* $@5: %empty  */
#line 515 "pascal.y"
                                                                               { }
#line 3425 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 515 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3431 "pascal.tab.c"
    break;

  case 191: /* $@6: %empty  */
#line 516 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3437 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3443 "pascal.tab.c"
    break;

  case 193: /* $@7: %empty  */
#line 517 "pascal.y"
                                        { }
#line 3449 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 517 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3455 "pascal.tab.c"
    break;

  case 195: /* $@8: %empty  */
#line 518 "pascal.y"
                                                                                                           { }
#line 3461 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3467 "pascal.tab.c"
    break;

  case 197: /* $@9: %empty  */
#line 519 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3473 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3479 "pascal.tab.c"
    break;

  case 199: /* $@10: %empty  */
#line 520 "pascal.y"
                                                                                                 { }
#line 3485 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3491 "pascal.tab.c"
    break;

  case 201: /* $@11: %empty  */
#line 521 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3497 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3503 "pascal.tab.c"
    break;

  case 203: /* $@12: %empty  */
#line 522 "pascal.y"
                                                                                            { }
#line 3509 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3515 "pascal.tab.c"
    break;

  case 205: /* $@13: %empty  */
#line 523 "pascal.y"
                                                                                                             { }
#line 3521 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 523 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3529 "pascal.tab.c"
    break;

  case 207: /* $@14: %empty  */
#line 526 "pascal.y"
                                                                                              { }
#line 3535 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 526 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3543 "pascal.tab.c"
    break;

  case 209: /* $@15: %empty  */
#line 529 "pascal.y"
                                                                                             { }
#line 3549 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 529 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3557 "pascal.tab.c"
    break;

  case 211: /* $@16: %empty  */
#line 532 "pascal.y"
                                                                                                    { }
#line 3563 "pascal.tab.c"
    break;

  case 212: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 532 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3571 "pascal.tab.c"
    break;

  case 213: /* $@17: %empty  */
#line 535 "pascal.y"
                                                                                                                { }
#line 3577 "pascal.tab.c"
    break;

  case 214: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 535 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3586 "pascal.tab.c"
    break;

  case 215: /* $@18: %empty  */
#line 539 "pascal.y"
                                                                                                                  { }
#line 3592 "pascal.tab.c"
    break;

  case 216: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 539 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3601 "pascal.tab.c"
    break;

  case 217: /* $@19: %empty  */
#line 543 "pascal.y"
                                                                                                                     { }
#line 3607 "pascal.tab.c"
    break;

  case 218: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 543 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3615 "pascal.tab.c"
    break;

  case 219: /* pv_mark: %empty  */
#line 548 "pascal.y"
    { }
#line 3621 "pascal.tab.c"
    break;

  case 220: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 551 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3627 "pascal.tab.c"
    break;

  case 221: /* parameter_list_opt: LPARENT RPARENT  */
#line 552 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3633 "pascal.tab.c"
    break;

  case 222: /* parameter_list_opt: %empty  */
#line 553 "pascal.y"
     { (yyval.node) = NULL; }
#line 3639 "pascal.tab.c"
    break;

  case 223: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 556 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3645 "pascal.tab.c"
    break;

  case 224: /* parameter_decl_list: parameter_decl  */
#line 557 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3651 "pascal.tab.c"
    break;

  case 225: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 560 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3657 "pascal.tab.c"
    break;

  case 226: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 561 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3663 "pascal.tab.c"
    break;

  case 227: /* parameter_decl: VARSY id_list COLON param_type  */
#line 562 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3669 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: id_list COLON param_type  */
#line 563 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3675 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 564 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3681 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 565 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3687 "pascal.tab.c"
    break;

  case 231: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 566 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3693 "pascal.tab.c"
    break;

  case 232: /* parameter_decl: IDENT id_list COLON param_type  */
#line 567 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3699 "pascal.tab.c"
    break;

  case 233: /* param_type: IDENT  */
#line 570 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3705 "pascal.tab.c"
    break;

  case 234: /* param_type: IDENT PERIOD IDENT  */
#line 571 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3711 "pascal.tab.c"
    break;

  case 235: /* param_type: ARRAYSY OFSY IDENT  */
#line 572 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3717 "pascal.tab.c"
    break;

  case 236: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 573 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3723 "pascal.tab.c"
    break;

  case 237: /* param_type: spec_ref  */
#line 574 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3729 "pascal.tab.c"
    break;

  case 238: /* routine_directives: %empty  */
#line 577 "pascal.y"
    { (yyval.node) = NULL; }
#line 3735 "pascal.tab.c"
    break;

  case 239: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 578 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3748 "pascal.tab.c"
    break;

  case 240: /* pf_params: LPARENT pf_sections RPARENT  */
#line 588 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3754 "pascal.tab.c"
    break;

  case 241: /* pf_params: LPARENT RPARENT  */
#line 589 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3760 "pascal.tab.c"
    break;

  case 242: /* pf_params: %empty  */
#line 590 "pascal.y"
     { (yyval.node) = NULL; }
#line 3766 "pascal.tab.c"
    break;

  case 243: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 593 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3772 "pascal.tab.c"
    break;

  case 244: /* pf_sections: pf_section  */
#line 594 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3778 "pascal.tab.c"
    break;

  case 245: /* pf_section: id_list COLON param_type  */
#line 597 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3784 "pascal.tab.c"
    break;

  case 246: /* pf_section: VARSY id_list COLON param_type  */
#line 598 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3790 "pascal.tab.c"
    break;

  case 247: /* pf_section: CONSTSY id_list COLON param_type  */
#line 599 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3796 "pascal.tab.c"
    break;

  case 248: /* pf_section: IDENT id_list COLON param_type  */
#line 600 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3802 "pascal.tab.c"
    break;

  case 249: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 601 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3808 "pascal.tab.c"
    break;

  case 250: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 602 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3814 "pascal.tab.c"
    break;

  case 251: /* enum_list: enum_list COMMA enum_member  */
#line 605 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3820 "pascal.tab.c"
    break;

  case 252: /* enum_list: enum_member  */
#line 606 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3826 "pascal.tab.c"
    break;

  case 253: /* enum_member: IDENT  */
#line 609 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3832 "pascal.tab.c"
    break;

  case 254: /* id_list: id_list COMMA IDENT  */
#line 612 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3838 "pascal.tab.c"
    break;

  case 255: /* id_list: IDENT  */
#line 613 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3844 "pascal.tab.c"
    break;

  case 256: /* body: BEGINSY statement_list ENDSY  */
#line 616 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3850 "pascal.tab.c"
    break;

  case 257: /* $@20: %empty  */
#line 619 "pascal.y"
                             { pas_stmt_mark(); }
#line 3856 "pascal.tab.c"
    break;

  case 258: /* statement_list: statement_list SEMICOLON $@20 statement  */
#line 619 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3862 "pascal.tab.c"
    break;

  case 259: /* $@21: %empty  */
#line 620 "pascal.y"
     { pas_stmt_mark(); }
#line 3868 "pascal.tab.c"
    break;

  case 260: /* statement_list: $@21 statement  */
#line 620 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3874 "pascal.tab.c"
    break;

  case 261: /* $@22: %empty  */
#line 623 "pascal.y"
    { pas_stmt_mark(); }
#line 3880 "pascal.tab.c"
    break;

  case 262: /* body_stmt: $@22 statement  */
#line 623 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3886 "pascal.tab.c"
    break;

  case 263: /* statement: statement_no_label  */
#line 626 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3892 "pascal.tab.c"
    break;

  case 264: /* statement: INTCONST COLON statement_no_label  */
#line 627 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 265: /* statement: IDENT COLON statement_no_label  */
#line 628 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3904 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: assignment  */
#line 631 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3910 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: call  */
#line 632 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3916 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: compound_statement  */
#line 633 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3922 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: goto_statement  */
#line 634 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3928 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: if_statement  */
#line 635 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3934 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: case_statement  */
#line 636 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3940 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: while_statement  */
#line 637 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3946 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: repeat_statement  */
#line 638 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3952 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: for_statement  */
#line 639 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3958 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: with_statement  */
#line 640 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3964 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: try_statement  */
#line 641 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3970 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: raise_statement  */
#line 642 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3976 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: inherited_call  */
#line 643 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3982 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: INHERITEDSY  */
#line 644 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3988 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: %empty  */
#line 645 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3994 "pascal.tab.c"
    break;

  case 281: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 648 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4000 "pascal.tab.c"
    break;

  case 282: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 649 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4006 "pascal.tab.c"
    break;

  case 283: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 650 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4012 "pascal.tab.c"
    break;

  case 284: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 651 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4018 "pascal.tab.c"
    break;

  case 285: /* handler_list: handler  */
#line 654 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4024 "pascal.tab.c"
    break;

  case 286: /* handler_list: handler_list SEMICOLON handler  */
#line 655 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4030 "pascal.tab.c"
    break;

  case 287: /* handler_list: handler_list SEMICOLON  */
#line 656 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4036 "pascal.tab.c"
    break;

  case 288: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 659 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4042 "pascal.tab.c"
    break;

  case 289: /* handler: ONSY IDENT DOSY body_stmt  */
#line 660 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4048 "pascal.tab.c"
    break;

  case 290: /* raise_statement: RAISESY  */
#line 663 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4054 "pascal.tab.c"
    break;

  case 291: /* raise_statement: RAISESY expression  */
#line 664 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4060 "pascal.tab.c"
    break;

  case 292: /* inherited_call: INHERITEDSY IDENT  */
#line 667 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4066 "pascal.tab.c"
    break;

  case 293: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 668 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4072 "pascal.tab.c"
    break;

  case 294: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 669 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4078 "pascal.tab.c"
    break;

  case 295: /* call: IDENT  */
#line 672 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4084 "pascal.tab.c"
    break;

  case 296: /* call: spec_expr  */
#line 673 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4090 "pascal.tab.c"
    break;

  case 297: /* call: selector PERIOD IDENT  */
#line 674 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4096 "pascal.tab.c"
    break;

  case 298: /* call: call_with_args  */
#line 675 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4102 "pascal.tab.c"
    break;

  case 299: /* recv_primary: STRINGCONST  */
#line 678 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4108 "pascal.tab.c"
    break;

  case 300: /* recv_primary: LPARENT expression RPARENT  */
#line 679 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4114 "pascal.tab.c"
    break;

  case 301: /* recv_primary: spec_expr  */
#line 680 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4120 "pascal.tab.c"
    break;

  case 302: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 683 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 303: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 684 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4132 "pascal.tab.c"
    break;

  case 304: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 685 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4138 "pascal.tab.c"
    break;

  case 305: /* call_with_args: IDENT LPARENT RPARENT  */
#line 686 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4144 "pascal.tab.c"
    break;

  case 306: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 687 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4150 "pascal.tab.c"
    break;

  case 307: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 688 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4156 "pascal.tab.c"
    break;

  case 308: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 689 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4162 "pascal.tab.c"
    break;

  case 309: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 690 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4168 "pascal.tab.c"
    break;

  case 310: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 691 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4174 "pascal.tab.c"
    break;

  case 311: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 692 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4180 "pascal.tab.c"
    break;

  case 312: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 693 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4186 "pascal.tab.c"
    break;

  case 313: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 694 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4192 "pascal.tab.c"
    break;

  case 314: /* argument_list: argument_list COMMA argument  */
#line 697 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4198 "pascal.tab.c"
    break;

  case 315: /* argument_list: argument  */
#line 698 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4204 "pascal.tab.c"
    break;

  case 316: /* argument: expression  */
#line 701 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4210 "pascal.tab.c"
    break;

  case 317: /* argument: expression COLON expression  */
#line 702 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4216 "pascal.tab.c"
    break;

  case 318: /* argument: expression COLON expression COLON expression  */
#line 703 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4222 "pascal.tab.c"
    break;

  case 319: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 704 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4228 "pascal.tab.c"
    break;

  case 320: /* assignment: selector BECOMES expression  */
#line 707 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4234 "pascal.tab.c"
    break;

  case 321: /* assignment: selector ADDBECOMES expression  */
#line 708 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4240 "pascal.tab.c"
    break;

  case 322: /* assignment: selector SUBBECOMES expression  */
#line 709 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4246 "pascal.tab.c"
    break;

  case 323: /* assignment: selector MULBECOMES expression  */
#line 710 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4252 "pascal.tab.c"
    break;

  case 324: /* assignment: selector DIVBECOMES expression  */
#line 711 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4258 "pascal.tab.c"
    break;

  case 325: /* assignment: call_with_args BECOMES expression  */
#line 712 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4264 "pascal.tab.c"
    break;

  case 326: /* selector: selector LBRACK expression_list RBRACK  */
#line 715 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4270 "pascal.tab.c"
    break;

  case 327: /* selector: selector PERIOD IDENT  */
#line 716 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4276 "pascal.tab.c"
    break;

  case 328: /* selector: selector ARROW  */
#line 717 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4282 "pascal.tab.c"
    break;

  case 329: /* selector: call_with_args PERIOD IDENT  */
#line 718 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4288 "pascal.tab.c"
    break;

  case 330: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 719 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4294 "pascal.tab.c"
    break;

  case 331: /* selector: call_with_args ARROW  */
#line 720 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4300 "pascal.tab.c"
    break;

  case 332: /* selector: recv_primary PERIOD IDENT  */
#line 721 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4306 "pascal.tab.c"
    break;

  case 333: /* selector: recv_primary ARROW  */
#line 722 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4312 "pascal.tab.c"
    break;

  case 334: /* selector: IDENT  */
#line 723 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4318 "pascal.tab.c"
    break;

  case 335: /* expression_list: expression_list COMMA expression  */
#line 726 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 336: /* expression_list: expression  */
#line 727 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4330 "pascal.tab.c"
    break;

  case 337: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 730 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4336 "pascal.tab.c"
    break;

  case 338: /* goto_statement: GOTOSY INTCONST  */
#line 733 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4342 "pascal.tab.c"
    break;

  case 339: /* goto_statement: GOTOSY IDENT  */
#line 734 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4348 "pascal.tab.c"
    break;

  case 340: /* if_statement: IFSY expression THENSY body_stmt  */
#line 737 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4354 "pascal.tab.c"
    break;

  case 341: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 738 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 342: /* $@23: %empty  */
#line 741 "pascal.y"
                           { }
#line 4366 "pascal.tab.c"
    break;

  case 343: /* case_statement: CASESY expression OFSY $@23 case_list case_tail ENDSY  */
#line 741 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4372 "pascal.tab.c"
    break;

  case 344: /* case_tail: %empty  */
#line 744 "pascal.y"
    { (yyval.node) = NULL; }
#line 4378 "pascal.tab.c"
    break;

  case 345: /* case_tail: ELSESY statement_list  */
#line 745 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4384 "pascal.tab.c"
    break;

  case 346: /* case_tail: OTHERWISESY statement_list  */
#line 746 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4390 "pascal.tab.c"
    break;

  case 347: /* case_list: case_list SEMICOLON case_elem  */
#line 749 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4396 "pascal.tab.c"
    break;

  case 348: /* case_list: case_elem  */
#line 750 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4402 "pascal.tab.c"
    break;

  case 349: /* case_elem: constant_list COLON body_stmt  */
#line 753 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4408 "pascal.tab.c"
    break;

  case 350: /* case_elem: %empty  */
#line 754 "pascal.y"
     { (yyval.node) = NULL; }
#line 4414 "pascal.tab.c"
    break;

  case 351: /* constant_list: constant_list COMMA case_label  */
#line 757 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4420 "pascal.tab.c"
    break;

  case 352: /* constant_list: case_label  */
#line 758 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4426 "pascal.tab.c"
    break;

  case 353: /* case_label: b_expr  */
#line 761 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4432 "pascal.tab.c"
    break;

  case 354: /* case_label: b_expr DOTDOT b_expr  */
#line 762 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4438 "pascal.tab.c"
    break;

  case 355: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 765 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4444 "pascal.tab.c"
    break;

  case 356: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 768 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4450 "pascal.tab.c"
    break;

  case 357: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 771 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4456 "pascal.tab.c"
    break;

  case 358: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 772 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4462 "pascal.tab.c"
    break;

  case 359: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 773 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4468 "pascal.tab.c"
    break;

  case 360: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 776 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4474 "pascal.tab.c"
    break;

  case 361: /* with_open: with_open COMMA selector  */
#line 779 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4480 "pascal.tab.c"
    break;

  case 362: /* with_open: selector  */
#line 780 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4486 "pascal.tab.c"
    break;

  case 363: /* expression: simple_expression  */
#line 787 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4492 "pascal.tab.c"
    break;

  case 364: /* expression: expression INOP simple_expression  */
#line 788 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4498 "pascal.tab.c"
    break;

  case 365: /* expression: expression LTOP simple_expression  */
#line 789 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4504 "pascal.tab.c"
    break;

  case 366: /* expression: expression LEOP simple_expression  */
#line 790 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4510 "pascal.tab.c"
    break;

  case 367: /* expression: expression GTOP simple_expression  */
#line 791 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4516 "pascal.tab.c"
    break;

  case 368: /* expression: expression GEOP simple_expression  */
#line 792 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4522 "pascal.tab.c"
    break;

  case 369: /* expression: expression NEOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4528 "pascal.tab.c"
    break;

  case 370: /* expression: expression EQOP simple_expression  */
#line 794 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4534 "pascal.tab.c"
    break;

  case 371: /* expression: expression ISOP simple_expression  */
#line 795 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4540 "pascal.tab.c"
    break;

  case 372: /* simple_expression: term  */
#line 798 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4546 "pascal.tab.c"
    break;

  case 373: /* simple_expression: PLUS term  */
#line 799 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4552 "pascal.tab.c"
    break;

  case 374: /* simple_expression: MINUS term  */
#line 800 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4558 "pascal.tab.c"
    break;

  case 375: /* simple_expression: simple_expression PLUS term  */
#line 801 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4564 "pascal.tab.c"
    break;

  case 376: /* simple_expression: simple_expression MINUS term  */
#line 802 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4570 "pascal.tab.c"
    break;

  case 377: /* simple_expression: simple_expression OROP term  */
#line 803 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4576 "pascal.tab.c"
    break;

  case 378: /* simple_expression: simple_expression XOROP term  */
#line 804 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4582 "pascal.tab.c"
    break;

  case 379: /* term: power  */
#line 807 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4588 "pascal.tab.c"
    break;

  case 380: /* term: term MUL power  */
#line 808 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4594 "pascal.tab.c"
    break;

  case 381: /* term: term RDIV power  */
#line 809 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4600 "pascal.tab.c"
    break;

  case 382: /* term: term IDIV power  */
#line 810 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4606 "pascal.tab.c"
    break;

  case 383: /* term: term IMOD power  */
#line 811 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4612 "pascal.tab.c"
    break;

  case 384: /* term: term ANDOP power  */
#line 812 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4618 "pascal.tab.c"
    break;

  case 385: /* term: term MUL MINUS power  */
#line 813 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4624 "pascal.tab.c"
    break;

  case 386: /* term: term RDIV MINUS power  */
#line 814 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4630 "pascal.tab.c"
    break;

  case 387: /* term: term IDIV MINUS power  */
#line 815 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4636 "pascal.tab.c"
    break;

  case 388: /* term: term IMOD MINUS power  */
#line 816 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4642 "pascal.tab.c"
    break;

  case 389: /* term: term ANDOP MINUS power  */
#line 817 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4648 "pascal.tab.c"
    break;

  case 390: /* term: term SHIFTOP power  */
#line 818 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4654 "pascal.tab.c"
    break;

  case 391: /* term: term ASOP power  */
#line 819 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4660 "pascal.tab.c"
    break;

  case 392: /* power: factor  */
#line 822 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4666 "pascal.tab.c"
    break;

  case 393: /* power: power POWOP factor  */
#line 823 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4672 "pascal.tab.c"
    break;

  case 394: /* factor: selector  */
#line 826 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4678 "pascal.tab.c"
    break;

  case 395: /* factor: call_with_args  */
#line 827 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4684 "pascal.tab.c"
    break;

  case 396: /* factor: INTCONST  */
#line 828 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4690 "pascal.tab.c"
    break;

  case 397: /* factor: REALCONST  */
#line 829 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4696 "pascal.tab.c"
    break;

  case 398: /* factor: STRINGCONST  */
#line 830 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4702 "pascal.tab.c"
    break;

  case 399: /* factor: CHARCODE  */
#line 831 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4708 "pascal.tab.c"
    break;

  case 400: /* factor: LPARENT expression RPARENT  */
#line 832 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4714 "pascal.tab.c"
    break;

  case 401: /* factor: inherited_call  */
#line 833 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4720 "pascal.tab.c"
    break;

  case 402: /* factor: NOTSY factor  */
#line 834 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4726 "pascal.tab.c"
    break;

  case 403: /* factor: ATSIGN factor  */
#line 835 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4732 "pascal.tab.c"
    break;

  case 404: /* factor: LBRACK RBRACK  */
#line 836 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4738 "pascal.tab.c"
    break;

  case 405: /* factor: LBRACK set_member_list RBRACK  */
#line 837 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4744 "pascal.tab.c"
    break;

  case 406: /* set_member_list: set_member  */
#line 840 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4750 "pascal.tab.c"
    break;

  case 407: /* set_member_list: set_member_list COMMA set_member  */
#line 841 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4756 "pascal.tab.c"
    break;

  case 408: /* set_member: expression  */
#line 844 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4762 "pascal.tab.c"
    break;

  case 409: /* set_member: expression DOTDOT expression  */
#line 845 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4768 "pascal.tab.c"
    break;


#line 4772 "pascal.tab.c"

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

#line 851 "pascal.y"

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
