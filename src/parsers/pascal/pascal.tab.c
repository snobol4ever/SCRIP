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
#define YYLAST   1857

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  413
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  996

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
     480,   481,   482,   483,   486,   489,   492,   493,   494,   495,
     498,   499,   502,   503,   506,   507,   510,   511,   512,   515,
     516,   517,   517,   518,   518,   519,   519,   520,   520,   521,
     521,   522,   522,   523,   523,   524,   524,   525,   525,   528,
     528,   531,   531,   534,   534,   537,   537,   541,   541,   545,
     545,   549,   549,   554,   557,   558,   559,   562,   563,   566,
     567,   568,   569,   570,   571,   572,   573,   576,   577,   578,
     579,   580,   583,   584,   594,   595,   596,   599,   600,   603,
     604,   605,   606,   607,   608,   611,   612,   615,   618,   619,
     622,   625,   625,   626,   626,   629,   629,   632,   633,   634,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   651,   654,   655,   656,   657,   660,
     661,   662,   665,   666,   669,   670,   673,   674,   675,   678,
     679,   680,   681,   684,   685,   686,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   703,   704,
     707,   708,   709,   710,   713,   714,   715,   716,   717,   718,
     721,   722,   723,   724,   725,   726,   727,   728,   729,   732,
     733,   736,   739,   740,   743,   744,   747,   747,   750,   751,
     752,   755,   756,   759,   760,   763,   764,   767,   768,   771,
     774,   777,   778,   779,   782,   785,   786,   793,   794,   795,
     796,   797,   798,   799,   800,   801,   804,   805,   806,   807,
     808,   809,   810,   813,   814,   815,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   828,   829,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,   843,
     846,   847,   850,   851
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

#define YYPACT_NINF (-890)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-339)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     528,    -2,    15,   153,  -890,     9,  1099,  -890,  -890,   347,
    -890,    39,  -890,  -890,   419,   108,    15,   111,   211,   211,
     217,  -890,   184,   231,   234,   258,   428,  -890,  -890,  -890,
     273,   211,   228,   355,   305,  -890,  -890,   132,   514,   108,
    -890,   169,   350,   204,  -890,   211,  -890,   352,   197,   379,
      41,   684,   458,   357,   184,  -890,   387,   431,   434,   410,
     464,   450,   460,  -890,   374,  -890,  -890,  -890,  -890,   478,
    1405,  1078,  -890,  -890,    15,   211,   488,   515,  -890,   544,
    1078,  -890,   211,   556,    95,  -890,  -890,   666,   634,  -890,
    1405,   255,  1405,  -890,  1405,  1405,   629,  -890,  1157,  -890,
    1405,   660,   678,  -890,   782,  -890,  -890,  -890,  -890,  -890,
    -890,  -890,   597,   919,  -890,  1059,  -890,  -890,  -890,  -890,
    -890,  -890,  -890,  -890,   211,  1129,   716,  -890,   545,   598,
     711,   705,   715,   744,   756,  -890,   746,   592,  -890,  -890,
    1613,  1613,  1613,  1205,  1405,  1613,  -890,  -890,  -890,   609,
      -8,   660,   748,  -890,   739,   581,    28,   282,   669,   736,
    -890,   626,   760,  -890,   760,   913,   913,   263,   749,  -890,
    -890,  -890,  -890,  1490,   252,   185,  -890,  -890,  -890,  -890,
     791,   452,  -890,   310,   344,   350,   415,  -890,    57,   843,
    -890,    48,   491,  -890,  -890,   795,   684,  -890,  -890,   588,
     328,   195,   739,   581,    37,   842,   249,  1661,  1673,   990,
     990,  1230,   899,    20,  1776,   794,   814,  1255,   789,  -890,
    1405,   799,  -890,  1405,  1280,  1405,   805,  -890,  1405,  1405,
    1405,  1405,  1405,   498,    77,   160,   866,   211,   809,   812,
     193,  -890,  -890,   211,   211,  -890,   108,    15,  -890,   820,
     211,   824,   184,  -890,  -890,   669,   669,  -890,  1695,   664,
    -890,  1707,  -890,   826,  -890,  1405,  1405,  1405,  1405,  1405,
    1405,  1405,  1405,  1613,  1613,  1613,  1613,  1496,  1521,  1546,
    1571,  1596,  1613,  1613,  1613,   797,   283,  -890,   102,   854,
     850,   612,   344,   344,     0,  -890,  -890,   512,  -890,   899,
     827,  1766,  1766,   835,   283,   377,   858,   863,   883,   589,
     888,   892,   901,   896,   214,  -890,   692,  1430,   107,   954,
     964,  -890,   425,   425,  1766,   425,   425,   425,   425,  -890,
     515,   211,   926,   211,   930,  -890,   211,    61,  -890,   720,
    -890,  -890,  1430,   931,  -890,   515,  -890,   329,  -890,  1405,
    1405,  1405,  -890,  -890,   255,  -890,  -890,  -890,  -890,  1170,
    -890,  -890,  -890,   525,  -890,   640,  1194,   162,  -890,   921,
    -890,  1305,   899,  -890,   571,   961,  1776,   965,   695,  1776,
    -890,   580,  1776,   613,   753,  1776,  1776,  1776,  1776,   975,
    -890,  -890,   283,  -890,   531,  -890,  -890,  -890,  -890,   515,
     515,   538,   558,   108,   213,   627,  -890,   211,  -890,   184,
    1405,  1405,  -890,   781,   967,   282,   282,   282,   282,   282,
     282,   282,   282,   669,   669,   669,   669,  1613,   736,  1613,
     736,  1613,   736,  1613,   736,  1613,   736,   736,   736,  -890,
     969,     0,   602,  -890,  -890,   444,   211,   957,   211,   960,
    -890,   211,    64,  -890,   796,   963,   329,   970,   749,  -890,
     559,   731,   425,   425,  1766,   762,  -890,   489,   346,  -890,
     317,  -890,   615,   972,  -890,   977,   170,   435,   979,   980,
     981,  -890,   839,  -890,  -890,   986,  -890,   212,  -890,  -890,
    -890,   264,   982,  -890,  -890,  1455,  1035,  1776,  1078,   899,
    1078,   899,   264,   346,   346,   397,  -890,  -890,  -890,  -890,
     515,  1054,   848,   760,   856,   760,   860,   147,  -890,    56,
      54,  1055,  1056,   515,  1002,  1058,   104,  -890,  1062,  1095,
     345,  1776,  -890,   684,  -890,   581,  1766,  1023,  1405,  -890,
    1405,  1405,   899,  -890,   992,   287,   221,  -890,   316,  -890,
     628,   564,  -890,  1330,  1355,  1405,  -890,  -890,  1380,  -890,
    1078,   654,  1016,   515,   515,   515,   515,  1067,  1068,  -890,
    -890,  -890,  -890,  -890,  1060,   515,   515,  1776,  -890,   736,
     736,   736,   736,   736,  -890,   283,  1017,  -890,   864,   760,
     875,   760,   880,   206,  -890,    56,  -890,  1052,  -890,  -890,
    -890,   346,   346,   320,  1006,  1766,   425,   425,  -890,   425,
     425,   425,   425,  -890,  -890,  -890,  -890,  -890,  1010,  1011,
    -890,  -890,  1026,  1078,   515,   515,   264,  -890,  -890,   515,
     515,  -890,   136,  1078,   239,  1031,    79,  1707,  -890,  -890,
     806,  -890,  -890,  1063,  1089,  -890,    56,  -890,    56,  1036,
      56,  -890,  1072,   177,  -890,  1042,  1096,  1103,  -890,  -890,
    -890,  1080,   329,  -890,  1039,  -890,  1405,  1405,  -890,  -890,
     393,   304,  -890,   941,  -890,  -890,  -890,  1729,  1741,  -890,
     129,  -890,   921,  -890,  -890,  -890,  -890,  -890,  -890,   665,
    -890,   704,  1776,  -890,   709,  1107,  1111,  1078,  1114,  1097,
    1100,    80,  -890,  -890,   515,   515,  1117,  1117,  -890,  1119,
    1154,  1102,  -890,  -890,    56,  -890,    56,  1104,    56,  -890,
    -890,  1087,  -890,  -890,   475,   346,   346,  -890,  -890,  -890,
    -890,   482,   515,   515,  -890,  -890,   515,   515,   147,  1106,
    1162,  -890,  1108,  1141,   515,   515,  -890,  -890,  -890,   237,
    -890,  1430,  1430,   349,  -890,   899,  1144,  -890,  -890,  1101,
    1125,  -890,   329,  -890,    49,  1109,  1405,  -890,  -890,  1175,
     329,  1186,  -890,    55,   785,   895,  -890,  1766,  1766,  -890,
    -890,  1164,  1766,  -890,  -890,  1405,  1405,  -890,  1120,  -890,
     322,  -890,  -890,  -890,  -890,  -890,  1192,  -890,   329,   329,
    -890,   329,  1101,  1101,  1193,  1139,  1117,  1147,  -890,   329,
     521,  -890,  -890,   329,  -890,  -890,  -890,  -890,  1195,  1150,
     515,   515,  1202,  1206,    29,   283,  -890,   329,  -890,  1153,
    1155,  1766,   749,  1207,  -890,  -890,   724,  -890,  1160,  1151,
    -890,  1101,  -890,  1405,  -890,  -890,  -890,  -890,  1776,  -890,
    1217,  -890,  1218,  -890,  -890,  -890,   397,  -890,  1117,  1117,
    -890,  -890,  -890,  1754,  1776,  -890,  1213,  -890,  -890,  1101,
    1228,  1232,  1101,  1244,  -890,  -890,  -890,   329,  -890,  1101,
    1245,  -890,  -890,  -890,   329,  1246,  1196,  -890,  -890,  -890,
    -890,  -890,   329,   329,  1247,  -890,   945,   725,  -890,  1430,
    -890,  1430,  1227,  -890,  -890,  1252,  1776,  -890,  1101,  -890,
    1253,  -890,  -890,  -890,  -890,  -890,  -890,  -890,  -890,  -890,
    1256,  1257,  1101,  1258,  -890,  1259,  -890,   329,    26,  -890,
    -890,  1766,  1201,  1242,  1766,  -890,  -890,  1078,  1078,  1266,
    -890,  1101,  -890,  -890,  -890,  1269,  1101,  1101,  1271,  1101,
    -890,  -890,  -890,  -890,  1101,  -890,  1277,  -890,   668,  -890,
    -890,  -890,  1247,  -890,  -890,  -890,  -890,  1278,  -890,  -890,
    -890,  -890,  -890,  1281,  1101,  -890,  -890,  -890,   264,  1766,
    1282,  -890,  1285,  1286,  1291,  -890,  -890,  1234,  1247,  -890,
    -890,  -890,  -890,  1296,  -890,  -890
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   223,   259,    16,   185,     0,     0,   223,
       0,   284,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   127,    38,    18,     0,     0,     0,   226,   184,     0,
     127,    19,     0,     0,   226,   261,   260,     0,     0,   263,
       0,     0,     0,   263,     0,     0,     0,   303,   299,   263,
     294,   283,     0,    69,   300,    68,   264,   267,   280,   281,
     282,   271,     0,   302,   270,     0,   272,   273,   274,   275,
     276,   277,   278,   279,     0,   127,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   400,   403,   401,   402,
     338,     0,   305,   405,   399,   398,     0,   367,   376,   383,
     396,   107,   246,   126,   246,     0,     0,     0,     0,    51,
      54,    52,    53,   129,   109,   173,   139,    76,    75,   132,
       0,     0,    80,     0,   134,    64,     0,   223,     0,     0,
     258,     0,     0,   223,   195,     0,   284,   342,   343,     0,
       0,     0,     0,   366,     0,     0,     0,     0,     0,   284,
     284,     0,     0,     0,   295,   296,     0,     0,     0,   337,
       0,     0,   335,     0,     0,     0,     0,   332,     0,     0,
       0,     0,     0,     0,   107,   109,     0,     0,     0,     0,
       0,   223,   223,     0,     0,     4,     0,     0,    12,     0,
       0,     0,     0,    21,   406,   377,   378,   408,   412,     0,
     410,     0,   407,     0,    40,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   109,     0,    88,
       0,    50,   135,   136,    81,    83,   257,     0,   256,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   173,
       0,     0,     0,     0,   179,   162,     0,     0,     0,    96,
       0,    93,     0,     0,     0,     0,     0,     0,     0,   223,
     226,     0,     0,     0,     0,   225,   259,     0,   228,     0,
     242,   186,     0,     0,   223,   226,    12,     0,   262,     0,
       0,     0,   265,   265,     0,   346,   341,   265,   304,   299,
     268,   269,   309,     0,   319,   398,   320,     0,    78,   263,
     263,     0,     0,   308,     0,   336,   329,   333,     0,   340,
     311,     0,   324,   301,     0,   325,   326,   327,   328,     0,
      61,    60,     0,    57,     0,   223,   223,   223,   223,   226,
     226,     0,     0,    24,     0,    31,   223,    26,   223,    25,
       0,     0,   409,   404,   331,   368,   369,   370,   371,   372,
     373,   374,   375,   381,   379,   380,   382,     0,   386,     0,
     387,     0,   388,     0,   384,     0,   385,   394,   395,   397,
       0,    72,     0,    70,    73,     0,     0,     0,     0,     0,
     245,   259,     0,   248,     0,     0,     0,     0,     0,   128,
       0,   131,     0,     0,     0,    50,   156,     0,   146,   151,
       0,    82,     0,     0,   100,     0,   111,     0,     0,     0,
       0,   110,     0,   223,   223,     0,   170,     0,   223,   223,
      98,   173,     0,   160,   163,     0,     0,    42,   127,     0,
     127,     0,   173,   137,   138,   133,   143,   144,   142,   145,
     226,     0,     0,   246,     0,   246,     0,     0,   224,     0,
     191,     0,     0,   226,     0,     0,   104,   106,     0,     0,
       0,   360,   359,   284,   364,   365,   354,   344,     0,   306,
       0,     0,     0,    67,     0,     0,     0,   289,     0,   297,
       0,     0,   307,     0,     0,     0,   334,   310,     0,   330,
     127,     0,     0,   226,   226,   226,   226,     0,     0,   223,
     223,    23,   263,   263,    33,   226,   226,   413,   411,   391,
     392,   393,   389,   390,   108,     0,   107,   102,     0,   246,
       0,   246,     0,     0,   244,     0,    89,    90,   140,   255,
      77,   147,   148,     0,     0,     0,     0,     0,   130,     0,
       0,     0,     0,   141,   109,   223,   223,   112,     0,     0,
     223,   223,   121,   127,   226,   226,   173,   223,   223,   226,
     226,   161,     0,   127,   338,     0,     0,    42,    41,    87,
       0,    97,    95,     0,     0,   242,     0,   229,     0,     0,
       0,   227,     0,   237,   241,   232,     0,     0,    12,   187,
     188,     0,     0,   196,     0,   242,     0,     0,   265,   266,
     357,   348,   352,     0,   356,   265,   318,   340,   321,    79,
       0,   285,   291,   286,   263,   288,   298,    74,   317,     0,
     315,     0,   339,   313,     0,     0,   109,   127,     0,     0,
       0,     0,   242,   242,   226,   226,    29,    30,   263,     0,
       0,     0,    71,   109,     0,   253,     0,     0,     0,   247,
     249,     0,   159,   157,     0,   149,   150,   153,   154,   152,
     155,     0,   226,   226,   223,   223,   226,   226,     0,     0,
       0,   165,     0,     0,   226,   226,   167,   168,   175,     0,
     164,     0,     0,     0,    44,     0,     0,    94,   242,   205,
     233,   231,     0,   236,     0,     0,     0,   189,   243,     0,
       0,     0,   105,   193,     0,     0,   363,     0,   354,   263,
     263,     0,     0,   265,   345,     0,     0,   265,     0,   290,
       0,   316,   314,   312,    59,    62,     0,   242,     0,     0,
     242,     0,   209,   211,     0,     0,    32,     0,   242,     0,
       0,   251,   250,     0,   252,    91,   158,   101,     0,     0,
     226,   226,     0,     0,     0,     0,   113,     0,    99,     0,
     172,   183,     0,     0,    48,    46,     0,    45,     0,     0,
      84,   201,    12,     0,   230,   240,   239,   238,   234,   192,
       0,   242,     0,    12,   265,   265,   358,   351,   350,   349,
     347,   355,   353,     0,   322,   293,     0,   287,    58,   213,
       0,     0,   219,     0,    12,    12,   242,     0,    20,    27,
       0,   103,   254,   114,     0,     0,     0,   116,   117,   122,
     123,   166,     0,     0,   178,   181,     0,     0,   175,     0,
      43,     0,     0,    85,    12,     0,   235,   242,   207,   190,
       0,   362,   361,   323,   265,    12,   242,   242,    12,   242,
       0,     0,   197,     0,   242,     0,   118,     0,     0,   171,
     169,   183,     0,     0,   183,    47,    49,   127,   127,     0,
     206,   203,    12,   194,   292,     0,   221,   217,     0,   215,
     210,   212,    12,   242,    28,   115,     0,   120,     0,   180,
     174,   175,   176,    92,    86,   202,    12,     0,   214,    12,
      12,   220,    12,     0,   199,   119,   125,   124,   173,   183,
       0,   208,     0,     0,     0,   198,    12,     0,   177,   204,
     222,   218,   216,     0,   182,   200
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -890,  -890,  -890,  -890,  -890,   -65,  1064,  -890,  -890,  -890,
    -890,  -890,  -890,  -890,  1061,   -34,  -333,  -890,  -890,   -58,
    1069,   -48,  1066,    47,   -70,   -28,  -280,  -567,   -64,   199,
    -261,   -78,  -890,  -890,  -890,   157,   722,  -251,  -890,  -890,
    -890,  -890,  -195,  -890,   327,  -273,  -264,  -286,  -499,  -890,
    -288,  -890,  -890,  -838,  -890,  -889,   380,  1065,   -30,  -890,
    -890,  -890,  -890,  -890,  -890,  -890,  -890,  -890,  -890,  -890,
    -890,  -890,  -890,  -890,  -890,  -890,   554,   366,   576,   807,
    -501,   128,  -145,  -890,   723,   494,   874,    68,  -890,   -85,
    -890,  -890,  -327,  -890,  -176,   385,  -890,  -890,   651,  -890,
     -24,  -890,  -890,   -39,  -174,   798,  -890,   -16,  1115,  -890,
    -890,  -890,  -890,  -890,  -890,  -890,   561,  -525,   555,  -890,
    -890,  -890,  -890,  -890,    66,   751,   -69,  -163,  -108,  -890,
     929
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   574,   709,    37,    39,    40,   496,   836,   636,   466,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   903,   938,   502,   528,   287,   305,   481,   739,
     928,   181,   182,   183,   184,   670,   468,   469,   313,   314,
     315,   633,   978,   831,   493,   894,   895,    45,    46,    28,
     658,   853,   346,   952,   986,   904,   966,   842,   942,   874,
     875,   915,   972,   970,   918,   969,    77,   189,   337,   338,
     655,   520,   289,   452,   453,   297,   298,   316,    29,    50,
     196,    51,   532,   533,   106,   107,   108,   546,   547,   109,
     153,   111,   112,   154,   363,   364,   114,   155,   384,   116,
     117,   118,   119,   536,   781,   671,   672,   896,   674,   120,
     121,   122,   123,   204,   366,   157,   158,   159,   160,   259,
     260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,   643,   200,    72,   127,   178,   206,   521,
     177,   673,   113,   176,   213,    78,   178,   368,   712,   290,
     348,   486,   176,   104,   472,    85,   534,   110,   467,   470,
     537,   957,   254,   264,   517,   115,   445,   262,   460,   506,
     507,   508,   509,   374,    33,   962,    85,   236,   299,    11,
     381,   505,   202,   341,   353,   177,   211,   845,   503,   504,
     934,   178,   652,   656,   852,   331,   517,   176,    13,   593,
     265,    86,   255,   256,     8,   203,   266,   267,   268,   269,
     270,   271,   390,   332,   753,   800,    47,    48,   212,   333,
     988,    10,   889,   334,   720,   354,   212,   177,    34,    64,
     194,   342,   958,   178,   368,   369,   370,   176,   176,   272,
     446,   551,   561,    47,   428,   430,   432,   434,   436,   437,
     438,   185,   335,   979,   343,   846,   518,   743,   447,   594,
     657,   657,   653,   336,   448,   498,   156,    68,   449,   285,
     801,   286,   177,   186,   754,   760,   787,   761,   178,   763,
     192,   102,   299,    12,   176,   331,   201,   113,   205,   188,
     207,   208,   635,   664,   748,   391,   214,   450,   104,   499,
     113,   113,   110,   332,    73,   617,   439,   368,   451,   333,
     115,   104,   104,   334,    38,   110,   110,    43,   303,   788,
      69,   603,   233,   115,   115,   365,   749,   550,   601,   602,
     212,   365,    81,   631,   423,   424,   425,   426,   365,   258,
     261,   306,   352,   811,   446,   812,   177,   814,   571,   491,
     542,   307,   178,   336,   392,   299,   682,    74,  -259,   177,
    -259,   397,   447,    65,   177,   178,   765,   265,   448,   492,
     178,   176,   449,   266,   267,   268,   269,   270,   271,   308,
     627,   683,    75,   673,    85,    79,   339,    56,   890,   543,
      52,   309,   684,    76,   579,   310,   580,    57,   581,   398,
     582,    74,   583,   212,   311,   312,   272,   177,    53,   356,
     303,   525,   451,   178,   545,   548,   376,    44,   628,   379,
     306,   382,    85,    49,   379,   385,   386,   387,   388,   751,
     307,   832,   177,   211,   640,   394,   642,    58,   178,   778,
      59,   401,   402,   833,   176,   202,   304,   681,    47,    95,
     779,    85,   177,   727,   728,   729,   730,    85,   178,   273,
      97,   150,   724,    85,    60,   212,   274,   275,   535,   294,
     309,   776,   725,   726,   310,   780,   685,   679,   784,    63,
     102,   351,   867,   311,   312,   365,   454,   669,   102,   441,
     276,   127,   668,   731,   322,   323,   295,    66,   647,    72,
     649,   606,   607,   482,   606,   607,   324,    78,   102,   689,
     691,    67,   613,   497,   694,   722,   177,   265,   325,   326,
     609,   610,   178,   266,   267,   268,   269,   270,   271,   512,
     327,   514,   611,   473,   516,   526,    30,   474,   497,    34,
      79,    31,    80,   475,   837,   529,   530,   531,   834,   835,
     639,   328,   641,   612,   102,   838,   272,    82,   177,   177,
     177,   177,    79,   126,   178,   178,   178,   178,    83,   135,
     176,   176,   176,   176,   715,   731,   717,   606,   607,   177,
     195,   606,   607,   476,    61,   178,   862,   477,   318,   777,
     865,   618,   810,   128,    62,   329,   478,   479,   480,   131,
     473,   619,   177,    79,   587,    47,   577,   258,   178,   987,
     475,   319,   695,   320,   176,   444,   321,   706,   707,   464,
     177,    35,   292,   293,   113,    36,   178,   169,   170,   171,
     172,   465,   176,   444,   856,   104,   124,   129,   473,   110,
     130,   125,   817,   482,   588,   177,   590,   115,   475,   592,
     476,   178,   365,   132,   477,   177,   133,   911,   912,   606,
     607,   178,     1,   478,   479,   480,   134,   365,   365,     2,
     816,   344,   365,   606,   607,   740,   527,   473,   389,    79,
     138,   881,   608,   177,   139,   750,    79,   475,   476,   178,
     839,   637,   477,   177,   187,   176,   935,    70,   936,   178,
     458,   478,   479,   480,    71,   176,   177,   459,   177,   188,
     177,   562,   178,   538,   178,   339,   178,   944,   569,    79,
     539,   444,   177,   769,   360,   361,    79,   476,   178,   790,
     246,   477,   247,    84,   238,   248,   677,   678,   570,   600,
     478,   479,   480,   597,   687,   306,    79,   542,   249,   796,
     190,   692,   542,   806,   250,   307,   349,   177,   251,   538,
     350,   252,   193,   178,    14,    15,   552,    16,   538,   176,
     263,   572,   227,   228,   177,   557,   177,  -259,   177,  -259,
     178,  -331,   178,    17,   178,   527,   218,   239,   219,    18,
     585,   454,    19,    20,   573,   309,    22,   586,  -303,   310,
    -303,   457,  -331,   585,  -331,  -331,   302,   558,   311,   312,
     614,  -331,  -331,  -331,  -331,   177,   538,    87,   285,   209,
     286,   178,   177,   686,   858,   859,   511,   176,   178,   263,
     177,   227,   540,    88,    89,    90,   178,    23,    91,    92,
     199,   524,   585,   277,   278,   279,    24,    25,   654,   696,
      26,    93,   411,   538,    94,   280,   281,   412,   177,   177,
     791,   177,   774,   775,   178,   178,   215,   178,   197,   177,
     976,   330,   198,   177,   977,   178,   282,   345,    95,   178,
      79,   283,   494,   555,   216,   177,    96,   177,   556,    97,
      98,   178,   538,   178,   237,   567,   568,   538,    99,   792,
     240,   100,   101,   759,   793,  -140,  -140,   905,    79,   102,
     519,   241,   899,   458,   444,  -140,  -140,  -140,   910,   900,
     933,   242,   243,   773,   654,   399,   400,  -140,   221,   482,
     222,   223,   854,   224,   244,   245,   339,   177,  -140,   920,
     921,   555,   217,   178,   177,   284,   559,   497,   497,   771,
     178,   604,   177,   177,   288,   296,   605,   265,   178,   178,
     802,   803,   848,   266,   267,   268,   269,   270,   271,   939,
    -304,  -305,  -304,  -305,   317,   654,   217,   654,   340,   654,
     945,   863,   864,   948,    79,   347,   595,   177,   371,   963,
     964,   527,   372,   178,   755,   375,   272,   177,   177,   756,
     355,   393,   440,   178,   178,   377,   644,   967,   482,   176,
     176,   383,   455,   510,   265,   395,   841,   973,   396,   661,
     266,   267,   268,   269,   270,   271,   406,    79,   523,   623,
     408,   980,   414,   461,   982,   983,    79,   984,   646,   906,
     456,   471,   855,   654,    79,   654,   648,   654,    79,   844,
     650,   993,    79,   272,   714,   869,   490,   850,   872,   698,
     699,   700,   701,    79,   483,   716,   879,   265,    79,   484,
     718,   710,   711,   266,   267,   268,   269,   270,   271,   563,
     564,   565,   566,   165,   166,   870,   871,   220,   873,   485,
     575,   527,   576,   168,   487,   497,   880,   497,   488,   527,
     882,   169,   170,   171,   172,   173,   272,   489,   221,   908,
     222,   223,   500,   224,   891,   169,   170,   171,   172,   291,
     741,   742,   501,    87,   102,   746,   747,   527,   527,   782,
     527,   783,   513,   782,   922,   932,   515,   522,   527,    88,
      89,    90,   527,   544,    91,    92,   415,   416,   417,   418,
     419,   420,   421,   422,   444,   553,   527,    93,   560,   554,
      94,   558,   584,   589,   923,   941,   591,   624,   625,   596,
     638,   925,   629,   630,   946,   947,   598,   949,   615,   929,
     930,   626,   954,   616,    95,   620,   621,   622,   632,   645,
     659,   660,   662,   663,   675,    97,   359,   665,   680,   697,
     804,   805,   702,   703,    99,   708,   527,   100,   101,   285,
     721,   974,   723,   527,   956,   102,   734,   735,   738,   752,
     161,   527,   527,   757,   758,   766,   762,   225,   818,   819,
     764,   767,   822,   823,   162,   163,    14,    15,   768,    16,
     829,   830,   794,   666,   164,   772,   795,   667,   226,   797,
     227,   228,    85,   704,   705,    17,   527,   229,   230,   231,
     232,    18,   165,   166,    19,    20,    21,   265,    22,   167,
     770,   234,   168,   266,   267,   268,   269,   270,   271,   807,
     169,   170,   171,   172,   173,   162,   163,   798,   174,   808,
     799,   175,   809,   815,   813,   164,   825,   826,   827,   732,
     733,   828,   840,   102,   736,   737,   272,   657,   843,    23,
     849,   744,   745,   165,   166,   847,   885,   886,    24,    25,
     167,   851,    26,   168,   860,  -338,   866,   868,   876,   877,
     883,   169,   170,   171,   172,   173,   878,   887,  -338,   235,
     884,   888,   175,   892,   902,   893,  -338,   210,  -338,  -338,
     901,   211,   907,   909,   102,  -338,  -338,  -338,  -338,  -338,
     914,  -338,  -338,   916,   211,   898,   265,   917,  -338,  -338,
    -338,  -338,   266,   267,   268,   269,   270,   271,   140,   919,
     924,   926,   931,   212,   541,   937,   927,   940,   943,   141,
     142,   950,   951,   953,   955,   960,   212,   143,   257,   144,
     961,   965,   145,   140,   968,   272,   971,   146,   147,   148,
     149,   150,   975,   981,   141,   142,   985,   989,   820,   821,
     990,   991,   143,   151,   144,   362,   992,   145,   140,   994,
     102,   995,   146,   147,   148,   149,   150,   403,   713,   141,
     142,   959,   405,   404,   824,   407,   719,   143,   151,   144,
     373,   409,   145,   140,   651,   102,   897,   146,   147,   148,
     149,   150,   599,   789,   141,   142,   676,   861,   378,   857,
     578,     0,   143,   151,   144,   380,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     549,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   688,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
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
       0,   102,     0,     0,   357,     0,     0,     0,     0,     0,
       0,   151,     0,   265,     0,     0,     0,     0,   102,   266,
     267,   268,   269,   270,   271,   265,     0,     0,     0,     0,
       0,   266,   267,   268,   269,   270,   271,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   358,     0,
       0,     0,   272,   266,   267,   268,   269,   270,   271,   265,
       0,     0,     0,     0,   272,   266,   267,   268,   269,   270,
     271,   410,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   265,   413,     0,     0,     0,   272,   266,   267,   268,
     269,   270,   271,   265,     0,     0,     0,     0,   272,   266,
     267,   268,   269,   270,   271,   785,   265,     0,     0,     0,
       0,   786,   266,   267,   268,   269,   270,   271,     0,     0,
     272,     0,     0,     0,     0,     0,     0,   913,   265,     0,
     462,   463,   272,     0,   266,   267,   268,   269,   270,   271,
     464,     0,     0,     0,     0,   272,     0,     0,   169,   170,
     171,   172,   465,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   272
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   502,    89,    39,    54,    71,    93,   342,
      80,   536,    51,    71,    99,    45,    80,   212,   585,   164,
     196,   309,    80,    51,   304,     5,   353,    51,   301,   302,
     357,     5,   140,     5,     5,    51,   287,   145,   299,   325,
     326,   327,   328,   217,     5,   934,     5,   125,    48,     2,
     224,   324,    91,     5,    17,   125,    64,     8,   322,   323,
     898,   125,     6,     9,     9,     8,     5,   125,    59,     5,
      42,    30,   141,   142,    76,    91,    48,    49,    50,    51,
      52,    53,     5,    26,     5,     5,    18,    19,    96,    32,
     979,    76,    63,    36,   595,    58,    96,   167,    59,    31,
       5,    53,    76,   167,   299,    85,    86,   165,   166,    81,
       8,   372,   392,    45,   277,   278,   279,   280,   281,   282,
     283,    74,    65,   961,    76,    76,    65,   626,    26,    65,
      76,    76,    76,    76,    32,    28,    70,     5,    36,    62,
      60,    64,   212,    75,    65,   646,    17,   648,   212,   650,
      82,    95,    48,     0,   212,     8,    90,   196,    92,    64,
      94,    95,   495,    59,    28,     5,   100,    65,   196,    62,
     209,   210,   196,    26,     5,     5,   284,   372,    76,    32,
     196,   209,   210,    36,    76,   209,   210,    76,    28,    60,
      58,   464,   124,   209,   210,   211,    60,   371,   462,   463,
      96,   217,     5,   491,   273,   274,   275,   276,   224,   143,
     144,    26,    17,   714,     8,   716,   286,   718,     5,     5,
      58,    36,   286,    76,    64,    48,     5,    58,    58,   299,
      60,    38,    26,     5,   304,   299,    59,    42,    32,    25,
     304,   299,    36,    48,    49,    50,    51,    52,    53,    64,
      38,    30,    48,   778,     5,    58,   188,    26,   825,    97,
      76,    76,    41,    59,   427,    80,   429,    36,   431,    76,
     433,    58,   435,    96,    89,    90,    81,   347,    94,    30,
      28,   346,    76,   347,   369,   370,   220,    76,    76,   223,
      26,   225,     5,    76,   228,   229,   230,   231,   232,    60,
      36,    64,   372,    64,   499,   237,   501,    76,   372,     5,
      76,   243,   244,    76,   372,   354,    64,    30,   250,    64,
      16,     5,   392,   609,   610,   611,   612,     5,   392,    47,
      75,    76,   605,     5,    76,    96,    54,    55,   354,    76,
      76,   668,   606,   607,    80,    41,    30,   542,   675,    76,
      95,    23,    30,    89,    90,   371,   288,   533,    95,    76,
      78,   409,    17,   614,    54,    55,   167,    12,   513,   403,
     515,    54,    55,   305,    54,    55,    66,   407,    95,   553,
     554,    76,    65,   317,   558,    65,   456,    42,    44,    45,
      44,    45,   456,    48,    49,    50,    51,    52,    53,   331,
      56,   333,    56,    26,   336,    76,    59,    30,   342,    59,
      58,    64,    60,    36,    65,   349,   350,   351,   751,   752,
     498,    77,   500,    77,    95,    76,    81,    48,   498,   499,
     500,   501,    58,    76,   498,   499,   500,   501,    59,    65,
     498,   499,   500,   501,   589,   696,   591,    54,    55,   519,
      84,    54,    55,    76,    26,   519,   783,    80,     6,    66,
     787,    26,   713,    76,    36,    50,    89,    90,    91,    59,
      26,    36,   542,    58,    30,   407,   410,   411,   542,   978,
      36,    29,   560,    31,   542,   286,    34,   572,   573,    64,
     560,    72,   165,   166,   533,    76,   560,    72,    73,    74,
      75,    76,   560,   304,   777,   533,    48,    76,    26,   533,
      76,    53,    30,   445,   446,   585,   448,   533,    36,   451,
      76,   585,   538,    59,    80,   595,    76,   854,   855,    54,
      55,   595,     4,    89,    90,    91,    76,   553,   554,    11,
      65,    50,   558,    54,    55,   623,   347,    26,    50,    58,
      72,    30,    63,   623,    76,   633,    58,    36,    76,   623,
     755,   495,    80,   633,    76,   623,   899,    53,   901,   633,
      58,    89,    90,    91,    60,   633,   646,    65,   648,    64,
     650,    50,   646,    58,   648,   517,   650,   914,    50,    58,
      65,   392,   662,   658,   209,   210,    58,    76,   662,   684,
       8,    80,    10,    49,    59,    13,   540,   541,    50,    50,
      89,    90,    91,   456,    50,    26,    58,    58,    26,   697,
      76,   555,    58,   708,    32,    36,    38,   697,    36,    58,
      42,    39,    76,   697,     7,     8,    65,    10,    58,   697,
      59,    14,    61,    62,   714,    65,   716,    58,   718,    60,
     714,    38,   716,    26,   718,   456,    59,    59,    61,    32,
      58,   593,    35,    36,    37,    76,    39,    65,    59,    80,
      61,    59,    59,    58,    61,    62,    64,    64,    89,    90,
      65,    68,    69,    70,    71,   755,    58,     3,    62,    60,
      64,   755,   762,    65,   779,   780,   330,   755,   762,    59,
     770,    61,    62,    19,    20,    21,   770,    80,    24,    25,
      76,   345,    58,    44,    45,    46,    89,    90,   519,    65,
      93,    37,    58,    58,    40,    56,    57,    63,   798,   799,
      65,   801,   666,   667,   798,   799,    76,   801,    72,   809,
      72,   187,    76,   813,    76,   809,    77,   193,    64,   813,
      58,    82,    60,    58,    76,   825,    72,   827,    63,    75,
      76,   825,    58,   827,    48,   399,   400,    58,    84,    65,
      59,    87,    88,   645,    65,    44,    45,   842,    58,    95,
      60,    76,    58,    58,   585,    54,    55,    56,   853,    65,
      65,    76,    48,   665,   595,   241,   242,    66,    59,   731,
      61,    62,    17,    64,    48,    59,   738,   877,    77,   874,
     875,    58,    64,   877,   884,    79,    63,   751,   752,   662,
     884,    59,   892,   893,    64,    76,    64,    42,   892,   893,
     702,   703,   766,    48,    49,    50,    51,    52,    53,   904,
      59,    59,    61,    61,    53,   646,    64,   648,     5,   650,
     915,   785,   786,   918,    58,    60,    60,   927,    64,   937,
     938,   662,    48,   927,    58,    76,    81,   937,   938,    63,
      28,     5,    75,   937,   938,    76,   510,   942,   810,   937,
     938,    76,    28,   329,    42,    76,   758,   952,    76,   523,
      48,    49,    50,    51,    52,    53,    76,    58,   344,    60,
      76,   966,    76,    76,   969,   970,    58,   972,    60,   843,
      60,    76,    17,   714,    58,   716,    60,   718,    58,   762,
      60,   986,    58,    81,    60,   797,    30,   770,   800,   563,
     564,   565,   566,    58,    76,    60,   808,    42,    58,    76,
      60,   575,   576,    48,    49,    50,    51,    52,    53,   395,
     396,   397,   398,    54,    55,   798,   799,    38,   801,    76,
     406,   762,   408,    64,    76,   899,   809,   901,    76,   770,
     813,    72,    73,    74,    75,    76,    81,    76,    59,   851,
      61,    62,    28,    64,   827,    72,    73,    74,    75,    76,
     624,   625,    28,     3,    95,   629,   630,   798,   799,    58,
     801,    60,    76,    58,   876,    60,    76,    76,   809,    19,
      20,    21,   813,    92,    24,    25,   265,   266,   267,   268,
     269,   270,   271,   272,   825,    64,   827,    37,    53,    64,
      40,    64,    63,    76,   877,   907,    76,   483,   484,    76,
       5,   884,   488,   489,   916,   917,    76,   919,    76,   892,
     893,    65,   924,    76,    64,    76,    76,    76,    76,     5,
       5,     5,    60,     5,    41,    75,    76,     5,    76,    53,
     704,   705,     5,     5,    84,    15,   877,    87,    88,    62,
      28,   953,    76,   884,   927,    95,    76,    76,    62,    58,
      12,   892,   893,    30,     5,    53,    60,    38,   732,   733,
      28,     5,   736,   737,    26,    27,     7,     8,     5,    10,
     744,   745,     5,    18,    36,    76,     5,    22,    59,     5,
      61,    62,     5,   569,   570,    26,   927,    68,    69,    70,
      71,    32,    54,    55,    35,    36,    37,    42,    39,    61,
      60,    12,    64,    48,    49,    50,    51,    52,    53,    30,
      72,    73,    74,    75,    76,    26,    27,    60,    80,     5,
      60,    83,    60,    76,    60,    36,    60,     5,    60,   615,
     616,    30,    28,    95,   620,   621,    81,    76,    53,    80,
       5,   627,   628,    54,    55,    76,   820,   821,    89,    90,
      61,     5,    93,    64,    30,    38,    76,     5,     5,    60,
       5,    72,    73,    74,    75,    76,    59,     5,    38,    80,
      60,     5,    83,    60,    63,    60,    59,    60,    61,    62,
      60,    64,     5,     5,    95,    68,    69,    70,    71,    59,
      17,    61,    62,     5,    64,    28,    42,     5,    68,    69,
      70,    71,    48,    49,    50,    51,    52,    53,    43,     5,
       5,     5,     5,    96,    60,    28,    60,     5,     5,    54,
      55,     5,     5,     5,     5,    64,    96,    62,    63,    64,
      28,     5,    67,    43,     5,    81,     5,    72,    73,    74,
      75,    76,     5,     5,    54,    55,     5,     5,   734,   735,
       5,     5,    62,    88,    64,    65,     5,    67,    43,    65,
      95,     5,    72,    73,    74,    75,    76,   246,   586,    54,
      55,   931,   248,   247,   738,   250,   593,    62,    88,    64,
      65,   252,    67,    43,   517,    95,   832,    72,    73,    74,
      75,    76,   458,   682,    54,    55,   538,   782,   223,   778,
     411,    -1,    62,    88,    64,    65,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      65,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    65,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
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
      -1,    95,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    42,    -1,    -1,    -1,    -1,    95,    48,
      49,    50,    51,    52,    53,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    65,    -1,
      -1,    -1,    81,    48,    49,    50,    51,    52,    53,    42,
      -1,    -1,    -1,    -1,    81,    48,    49,    50,    51,    52,
      53,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    65,    -1,    -1,    -1,    81,    48,    49,    50,
      51,    52,    53,    42,    -1,    -1,    -1,    -1,    81,    48,
      49,    50,    51,    52,    53,    66,    42,    -1,    -1,    -1,
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
     175,   151,    64,    76,   114,   114,   115,    65,    76,   140,
      28,   179,   165,    53,   133,     8,    76,    76,   222,     5,
     133,     5,     9,   159,    17,    17,   143,   214,   187,   187,
      30,   216,   190,   222,   222,   190,    76,    30,     5,   179,
     133,   133,   179,   133,   167,   168,     5,    60,    59,   179,
     133,    30,   133,     5,    60,   175,   175,     5,     5,    63,
     125,   133,    60,    60,   153,   154,   215,   183,    28,    58,
      65,    60,    63,   130,   163,   103,   222,     5,   179,     5,
     103,   190,   190,    63,    17,   169,     5,     5,   172,     5,
     103,   103,   179,   133,     5,   133,     5,    60,   138,   133,
     133,     5,    60,    65,   151,   114,   114,    28,   131,   103,
       5,   179,   166,     5,   190,   103,   179,   179,   103,   179,
       5,     5,   161,     5,   179,     5,   133,     5,    76,   154,
      64,    28,   153,   129,   129,     5,   164,   103,     5,   173,
     171,     5,   170,   103,   179,     5,    72,    76,   150,   151,
     103,     5,   103,   103,   103,     5,   162,   146,   153,     5,
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
     142,   142,   142,   142,   142,   142,   143,   143,   143,   143,
     143,   144,   144,   144,   144,   144,   145,   145,   145,   145,
     146,   147,   147,   149,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   150,   151,   152,   152,   152,   152,
     153,   153,   154,   154,   155,   155,   156,   156,   156,   157,
     157,   158,   157,   159,   157,   160,   157,   161,   157,   162,
     157,   163,   157,   164,   157,   165,   157,   166,   157,   167,
     157,   168,   157,   169,   157,   170,   157,   171,   157,   172,
     157,   173,   157,   174,   175,   175,   175,   176,   176,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   178,   178,
     178,   178,   179,   179,   180,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   183,   183,   184,   185,   185,
     186,   188,   187,   189,   187,   191,   190,   192,   192,   192,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   194,   195,
     195,   195,   196,   196,   197,   197,   198,   198,   198,   199,
     199,   199,   199,   200,   200,   200,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   202,   202,
     203,   203,   203,   203,   204,   204,   204,   204,   204,   204,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   206,
     206,   207,   208,   208,   209,   209,   211,   210,   212,   212,
     212,   213,   213,   214,   214,   215,   215,   216,   216,   217,
     218,   219,   219,   219,   220,   221,   221,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   223,   223,   223,   223,
     223,   223,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   225,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     227,   227,   228,   228
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
       2,     7,     5,     0,     0,     0,     7,     9,     5,     0,
       3,     1,     6,     0,     2,     1,     4,     6,     6,     8,
      10,     0,     9,     0,    11,     0,     7,     0,    13,     0,
      15,     0,    12,     0,    14,     0,    11,     0,    13,     0,
      11,     0,    11,     0,    12,     0,    14,     0,    14,     0,
      12,     0,    14,     0,     3,     2,     0,     3,     1,     3,
       5,     4,     3,     4,     5,     6,     4,     1,     3,     3,
       3,     1,     0,     3,     3,     2,     0,     3,     1,     3,
       4,     4,     4,     3,     5,     3,     1,     1,     3,     1,
       3,     0,     4,     0,     2,     0,     2,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     5,     5,     7,     5,     1,
       3,     2,     6,     4,     1,     2,     2,     4,     5,     1,
       1,     3,     1,     1,     3,     1,     4,     4,     3,     3,
       4,     3,     6,     5,     6,     5,     6,     5,     3,     1,
       1,     3,     5,     6,     3,     3,     3,     3,     3,     3,
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
#line 2312 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2318 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2324 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2330 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2336 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2342 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2348 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2354 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2360 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2366 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2372 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2378 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2384 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2390 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2396 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2402 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2408 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2414 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2420 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2426 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2432 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2438 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2444 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2450 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2456 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2462 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2468 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2474 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2480 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2486 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2492 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2498 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2504 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2510 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2516 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2522 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2528 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2534 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2540 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2546 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2552 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2558 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2564 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2570 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2576 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2582 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2588 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2594 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2600 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2606 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2612 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2618 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2624 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2630 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2636 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2642 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2648 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2654 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2660 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2666 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2672 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2678 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2684 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2690 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2696 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2708 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2714 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2720 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2726 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2732 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2738 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2744 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2750 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2756 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2762 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2768 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2780 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2786 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2792 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2804 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2810 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2816 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2822 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2828 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2834 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2840 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2846 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2852 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2858 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2864 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2870 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2876 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2882 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2888 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2894 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2900 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2906 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2912 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2918 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2924 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2930 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2936 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2942 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2948 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2954 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2960 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2966 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2972 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2978 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2984 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2990 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2996 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3002 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3008 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3014 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3020 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3026 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3032 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3038 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3044 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3050 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3056 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3062 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3068 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3074 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3080 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3086 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3092 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3098 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3104 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3110 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3116 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3122 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3128 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3134 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT PERIOD IDENT  */
#line 439 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3140 "pascal.tab.c"
    break;

  case 141: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 440 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3146 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term MUL b_factor  */
#line 441 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3152 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IDIV b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3158 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term IMOD b_factor  */
#line 443 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3164 "pascal.tab.c"
    break;

  case 145: /* lo_term: lo_term SHIFTOP b_factor  */
#line 444 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3170 "pascal.tab.c"
    break;

  case 146: /* b_expr: b_term  */
#line 447 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3176 "pascal.tab.c"
    break;

  case 147: /* b_expr: PLUS b_term  */
#line 448 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3182 "pascal.tab.c"
    break;

  case 148: /* b_expr: MINUS b_term  */
#line 449 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3188 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr PLUS b_term  */
#line 450 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3194 "pascal.tab.c"
    break;

  case 150: /* b_expr: b_expr MINUS b_term  */
#line 451 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3200 "pascal.tab.c"
    break;

  case 151: /* b_term: b_factor  */
#line 454 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3206 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term MUL b_factor  */
#line 455 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3212 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IDIV b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3218 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term IMOD b_factor  */
#line 457 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3224 "pascal.tab.c"
    break;

  case 155: /* b_term: b_term SHIFTOP b_factor  */
#line 458 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3230 "pascal.tab.c"
    break;

  case 156: /* b_factor: scalar_constant  */
#line 461 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3236 "pascal.tab.c"
    break;

  case 157: /* b_factor: IDENT PERIOD IDENT  */
#line 462 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3242 "pascal.tab.c"
    break;

  case 158: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 463 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3248 "pascal.tab.c"
    break;

  case 159: /* b_factor: LPARENT b_expr RPARENT  */
#line 464 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3254 "pascal.tab.c"
    break;

  case 160: /* record_body: record_field_list record_case_opt  */
#line 467 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3260 "pascal.tab.c"
    break;

  case 161: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 470 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3266 "pascal.tab.c"
    break;

  case 162: /* record_field_list: record_field  */
#line 471 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3272 "pascal.tab.c"
    break;

  case 163: /* $@4: %empty  */
#line 474 "pascal.y"
                  { }
#line 3278 "pascal.tab.c"
    break;

  case 164: /* record_field: id_list COLON $@4 type  */
#line 474 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3284 "pascal.tab.c"
    break;

  case 165: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3290 "pascal.tab.c"
    break;

  case 166: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 476 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3296 "pascal.tab.c"
    break;

  case 167: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 477 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3302 "pascal.tab.c"
    break;

  case 168: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 478 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3308 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3314 "pascal.tab.c"
    break;

  case 170: /* record_field: IDENT record_field  */
#line 480 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3320 "pascal.tab.c"
    break;

  case 171: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 481 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3326 "pascal.tab.c"
    break;

  case 172: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 482 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3332 "pascal.tab.c"
    break;

  case 173: /* record_field: %empty  */
#line 483 "pascal.y"
     { (yyval.node) = NULL; }
#line 3338 "pascal.tab.c"
    break;

  case 174: /* case_arm_mark: %empty  */
#line 486 "pascal.y"
    { }
#line 3344 "pascal.tab.c"
    break;

  case 175: /* case_open: %empty  */
#line 489 "pascal.y"
    { }
#line 3350 "pascal.tab.c"
    break;

  case 176: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 492 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3356 "pascal.tab.c"
    break;

  case 177: /* record_case_opt: CASESY IDENT COLON LPARENT enum_list RPARENT OFSY case_open record_case_list  */
#line 493 "pascal.y"
                                                                                  { tree_t *_e = pt_retag((yyvsp[-4].node), TT_ENUM_TYPE); for (int _i = 0; _e && _i < _e->n; _i++) cenv_int(_e->c[_i]->v.sval, _i); (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-7].node)), _e), (yyvsp[0].node)); }
#line 3362 "pascal.tab.c"
    break;

  case 178: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 494 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3368 "pascal.tab.c"
    break;

  case 179: /* record_case_opt: %empty  */
#line 495 "pascal.y"
     { (yyval.node) = NULL; }
#line 3374 "pascal.tab.c"
    break;

  case 180: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 498 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3380 "pascal.tab.c"
    break;

  case 181: /* record_case_list: record_case_arm  */
#line 499 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3386 "pascal.tab.c"
    break;

  case 182: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 502 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3392 "pascal.tab.c"
    break;

  case 183: /* record_case_arm: %empty  */
#line 503 "pascal.y"
     { (yyval.node) = NULL; }
#line 3398 "pascal.tab.c"
    break;

  case 184: /* var_decl_list: var_decl_list var_decl  */
#line 506 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3404 "pascal.tab.c"
    break;

  case 185: /* var_decl_list: var_decl  */
#line 507 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3410 "pascal.tab.c"
    break;

  case 186: /* var_decl: id_list COLON type SEMICOLON  */
#line 510 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3416 "pascal.tab.c"
    break;

  case 187: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 511 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3422 "pascal.tab.c"
    break;

  case 188: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 512 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3428 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 515 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3434 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3440 "pascal.tab.c"
    break;

  case 191: /* $@5: %empty  */
#line 517 "pascal.y"
                                                                               { }
#line 3446 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3452 "pascal.tab.c"
    break;

  case 193: /* $@6: %empty  */
#line 518 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3458 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3464 "pascal.tab.c"
    break;

  case 195: /* $@7: %empty  */
#line 519 "pascal.y"
                                        { }
#line 3470 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 519 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3476 "pascal.tab.c"
    break;

  case 197: /* $@8: %empty  */
#line 520 "pascal.y"
                                                                                                           { }
#line 3482 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3488 "pascal.tab.c"
    break;

  case 199: /* $@9: %empty  */
#line 521 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3494 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3500 "pascal.tab.c"
    break;

  case 201: /* $@10: %empty  */
#line 522 "pascal.y"
                                                                                                 { }
#line 3506 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3512 "pascal.tab.c"
    break;

  case 203: /* $@11: %empty  */
#line 523 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3518 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 523 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3524 "pascal.tab.c"
    break;

  case 205: /* $@12: %empty  */
#line 524 "pascal.y"
                                                                                            { }
#line 3530 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 524 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3536 "pascal.tab.c"
    break;

  case 207: /* $@13: %empty  */
#line 525 "pascal.y"
                                                                                                             { }
#line 3542 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 525 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3550 "pascal.tab.c"
    break;

  case 209: /* $@14: %empty  */
#line 528 "pascal.y"
                                                                                              { }
#line 3556 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 528 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3564 "pascal.tab.c"
    break;

  case 211: /* $@15: %empty  */
#line 531 "pascal.y"
                                                                                             { }
#line 3570 "pascal.tab.c"
    break;

  case 212: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 531 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3578 "pascal.tab.c"
    break;

  case 213: /* $@16: %empty  */
#line 534 "pascal.y"
                                                                                                    { }
#line 3584 "pascal.tab.c"
    break;

  case 214: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 534 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3592 "pascal.tab.c"
    break;

  case 215: /* $@17: %empty  */
#line 537 "pascal.y"
                                                                                                                { }
#line 3598 "pascal.tab.c"
    break;

  case 216: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 537 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3607 "pascal.tab.c"
    break;

  case 217: /* $@18: %empty  */
#line 541 "pascal.y"
                                                                                                                  { }
#line 3613 "pascal.tab.c"
    break;

  case 218: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3622 "pascal.tab.c"
    break;

  case 219: /* $@19: %empty  */
#line 545 "pascal.y"
                                                                                              { }
#line 3628 "pascal.tab.c"
    break;

  case 220: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 545 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3637 "pascal.tab.c"
    break;

  case 221: /* $@20: %empty  */
#line 549 "pascal.y"
                                                                                                                     { }
#line 3643 "pascal.tab.c"
    break;

  case 222: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 549 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3651 "pascal.tab.c"
    break;

  case 223: /* pv_mark: %empty  */
#line 554 "pascal.y"
    { }
#line 3657 "pascal.tab.c"
    break;

  case 224: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 557 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3663 "pascal.tab.c"
    break;

  case 225: /* parameter_list_opt: LPARENT RPARENT  */
#line 558 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3669 "pascal.tab.c"
    break;

  case 226: /* parameter_list_opt: %empty  */
#line 559 "pascal.y"
     { (yyval.node) = NULL; }
#line 3675 "pascal.tab.c"
    break;

  case 227: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 562 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3681 "pascal.tab.c"
    break;

  case 228: /* parameter_decl_list: parameter_decl  */
#line 563 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3687 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 566 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3693 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 567 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3699 "pascal.tab.c"
    break;

  case 231: /* parameter_decl: VARSY id_list COLON param_type  */
#line 568 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3705 "pascal.tab.c"
    break;

  case 232: /* parameter_decl: id_list COLON param_type  */
#line 569 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3711 "pascal.tab.c"
    break;

  case 233: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 570 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3717 "pascal.tab.c"
    break;

  case 234: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 571 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3723 "pascal.tab.c"
    break;

  case 235: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 572 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3729 "pascal.tab.c"
    break;

  case 236: /* parameter_decl: IDENT id_list COLON param_type  */
#line 573 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3735 "pascal.tab.c"
    break;

  case 237: /* param_type: IDENT  */
#line 576 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3741 "pascal.tab.c"
    break;

  case 238: /* param_type: IDENT PERIOD IDENT  */
#line 577 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3747 "pascal.tab.c"
    break;

  case 239: /* param_type: ARRAYSY OFSY IDENT  */
#line 578 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3753 "pascal.tab.c"
    break;

  case 240: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 579 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3759 "pascal.tab.c"
    break;

  case 241: /* param_type: spec_ref  */
#line 580 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3765 "pascal.tab.c"
    break;

  case 242: /* routine_directives: %empty  */
#line 583 "pascal.y"
    { (yyval.node) = NULL; }
#line 3771 "pascal.tab.c"
    break;

  case 243: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 584 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3784 "pascal.tab.c"
    break;

  case 244: /* pf_params: LPARENT pf_sections RPARENT  */
#line 594 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3790 "pascal.tab.c"
    break;

  case 245: /* pf_params: LPARENT RPARENT  */
#line 595 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3796 "pascal.tab.c"
    break;

  case 246: /* pf_params: %empty  */
#line 596 "pascal.y"
     { (yyval.node) = NULL; }
#line 3802 "pascal.tab.c"
    break;

  case 247: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 599 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3808 "pascal.tab.c"
    break;

  case 248: /* pf_sections: pf_section  */
#line 600 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3814 "pascal.tab.c"
    break;

  case 249: /* pf_section: id_list COLON param_type  */
#line 603 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3820 "pascal.tab.c"
    break;

  case 250: /* pf_section: VARSY id_list COLON param_type  */
#line 604 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3826 "pascal.tab.c"
    break;

  case 251: /* pf_section: CONSTSY id_list COLON param_type  */
#line 605 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3832 "pascal.tab.c"
    break;

  case 252: /* pf_section: IDENT id_list COLON param_type  */
#line 606 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3838 "pascal.tab.c"
    break;

  case 253: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 607 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3844 "pascal.tab.c"
    break;

  case 254: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 608 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3850 "pascal.tab.c"
    break;

  case 255: /* enum_list: enum_list COMMA enum_member  */
#line 611 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3856 "pascal.tab.c"
    break;

  case 256: /* enum_list: enum_member  */
#line 612 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3862 "pascal.tab.c"
    break;

  case 257: /* enum_member: IDENT  */
#line 615 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3868 "pascal.tab.c"
    break;

  case 258: /* id_list: id_list COMMA IDENT  */
#line 618 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3874 "pascal.tab.c"
    break;

  case 259: /* id_list: IDENT  */
#line 619 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3880 "pascal.tab.c"
    break;

  case 260: /* body: BEGINSY statement_list ENDSY  */
#line 622 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3886 "pascal.tab.c"
    break;

  case 261: /* $@21: %empty  */
#line 625 "pascal.y"
                             { pas_stmt_mark(); }
#line 3892 "pascal.tab.c"
    break;

  case 262: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 625 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 263: /* $@22: %empty  */
#line 626 "pascal.y"
     { pas_stmt_mark(); }
#line 3904 "pascal.tab.c"
    break;

  case 264: /* statement_list: $@22 statement  */
#line 626 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3910 "pascal.tab.c"
    break;

  case 265: /* $@23: %empty  */
#line 629 "pascal.y"
    { pas_stmt_mark(); }
#line 3916 "pascal.tab.c"
    break;

  case 266: /* body_stmt: $@23 statement  */
#line 629 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3922 "pascal.tab.c"
    break;

  case 267: /* statement: statement_no_label  */
#line 632 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3928 "pascal.tab.c"
    break;

  case 268: /* statement: INTCONST COLON statement_no_label  */
#line 633 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3934 "pascal.tab.c"
    break;

  case 269: /* statement: IDENT COLON statement_no_label  */
#line 634 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3940 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: assignment  */
#line 637 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3946 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: call  */
#line 638 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3952 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: compound_statement  */
#line 639 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3958 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: goto_statement  */
#line 640 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3964 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: if_statement  */
#line 641 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3970 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: case_statement  */
#line 642 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3976 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: while_statement  */
#line 643 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3982 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: repeat_statement  */
#line 644 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3988 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: for_statement  */
#line 645 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3994 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: with_statement  */
#line 646 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4000 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: try_statement  */
#line 647 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4006 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: raise_statement  */
#line 648 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4012 "pascal.tab.c"
    break;

  case 282: /* statement_no_label: inherited_call  */
#line 649 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4018 "pascal.tab.c"
    break;

  case 283: /* statement_no_label: INHERITEDSY  */
#line 650 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4024 "pascal.tab.c"
    break;

  case 284: /* statement_no_label: %empty  */
#line 651 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4030 "pascal.tab.c"
    break;

  case 285: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 654 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4036 "pascal.tab.c"
    break;

  case 286: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 655 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4042 "pascal.tab.c"
    break;

  case 287: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 656 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4048 "pascal.tab.c"
    break;

  case 288: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 657 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4054 "pascal.tab.c"
    break;

  case 289: /* handler_list: handler  */
#line 660 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4060 "pascal.tab.c"
    break;

  case 290: /* handler_list: handler_list SEMICOLON handler  */
#line 661 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4066 "pascal.tab.c"
    break;

  case 291: /* handler_list: handler_list SEMICOLON  */
#line 662 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4072 "pascal.tab.c"
    break;

  case 292: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 665 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4078 "pascal.tab.c"
    break;

  case 293: /* handler: ONSY IDENT DOSY body_stmt  */
#line 666 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4084 "pascal.tab.c"
    break;

  case 294: /* raise_statement: RAISESY  */
#line 669 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4090 "pascal.tab.c"
    break;

  case 295: /* raise_statement: RAISESY expression  */
#line 670 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4096 "pascal.tab.c"
    break;

  case 296: /* inherited_call: INHERITEDSY IDENT  */
#line 673 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4102 "pascal.tab.c"
    break;

  case 297: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 674 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4108 "pascal.tab.c"
    break;

  case 298: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 675 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4114 "pascal.tab.c"
    break;

  case 299: /* call: IDENT  */
#line 678 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4120 "pascal.tab.c"
    break;

  case 300: /* call: spec_expr  */
#line 679 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 301: /* call: selector PERIOD IDENT  */
#line 680 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4132 "pascal.tab.c"
    break;

  case 302: /* call: call_with_args  */
#line 681 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4138 "pascal.tab.c"
    break;

  case 303: /* recv_primary: STRINGCONST  */
#line 684 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4144 "pascal.tab.c"
    break;

  case 304: /* recv_primary: LPARENT expression RPARENT  */
#line 685 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4150 "pascal.tab.c"
    break;

  case 305: /* recv_primary: spec_expr  */
#line 686 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4156 "pascal.tab.c"
    break;

  case 306: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 689 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4162 "pascal.tab.c"
    break;

  case 307: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 690 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4168 "pascal.tab.c"
    break;

  case 308: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 691 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4174 "pascal.tab.c"
    break;

  case 309: /* call_with_args: IDENT LPARENT RPARENT  */
#line 692 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4180 "pascal.tab.c"
    break;

  case 310: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 693 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4186 "pascal.tab.c"
    break;

  case 311: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 694 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4192 "pascal.tab.c"
    break;

  case 312: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 695 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4198 "pascal.tab.c"
    break;

  case 313: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 696 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4204 "pascal.tab.c"
    break;

  case 314: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 697 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4210 "pascal.tab.c"
    break;

  case 315: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 698 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4216 "pascal.tab.c"
    break;

  case 316: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 699 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4222 "pascal.tab.c"
    break;

  case 317: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 700 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4228 "pascal.tab.c"
    break;

  case 318: /* argument_list: argument_list COMMA argument  */
#line 703 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4234 "pascal.tab.c"
    break;

  case 319: /* argument_list: argument  */
#line 704 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4240 "pascal.tab.c"
    break;

  case 320: /* argument: expression  */
#line 707 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4246 "pascal.tab.c"
    break;

  case 321: /* argument: expression COLON expression  */
#line 708 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4252 "pascal.tab.c"
    break;

  case 322: /* argument: expression COLON expression COLON expression  */
#line 709 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4258 "pascal.tab.c"
    break;

  case 323: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 710 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4264 "pascal.tab.c"
    break;

  case 324: /* assignment: selector BECOMES expression  */
#line 713 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4270 "pascal.tab.c"
    break;

  case 325: /* assignment: selector ADDBECOMES expression  */
#line 714 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4276 "pascal.tab.c"
    break;

  case 326: /* assignment: selector SUBBECOMES expression  */
#line 715 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4282 "pascal.tab.c"
    break;

  case 327: /* assignment: selector MULBECOMES expression  */
#line 716 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4288 "pascal.tab.c"
    break;

  case 328: /* assignment: selector DIVBECOMES expression  */
#line 717 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4294 "pascal.tab.c"
    break;

  case 329: /* assignment: call_with_args BECOMES expression  */
#line 718 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4300 "pascal.tab.c"
    break;

  case 330: /* selector: selector LBRACK expression_list RBRACK  */
#line 721 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4306 "pascal.tab.c"
    break;

  case 331: /* selector: selector PERIOD IDENT  */
#line 722 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4312 "pascal.tab.c"
    break;

  case 332: /* selector: selector ARROW  */
#line 723 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4318 "pascal.tab.c"
    break;

  case 333: /* selector: call_with_args PERIOD IDENT  */
#line 724 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 334: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 725 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4330 "pascal.tab.c"
    break;

  case 335: /* selector: call_with_args ARROW  */
#line 726 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4336 "pascal.tab.c"
    break;

  case 336: /* selector: recv_primary PERIOD IDENT  */
#line 727 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4342 "pascal.tab.c"
    break;

  case 337: /* selector: recv_primary ARROW  */
#line 728 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4348 "pascal.tab.c"
    break;

  case 338: /* selector: IDENT  */
#line 729 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4354 "pascal.tab.c"
    break;

  case 339: /* expression_list: expression_list COMMA expression  */
#line 732 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 340: /* expression_list: expression  */
#line 733 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4366 "pascal.tab.c"
    break;

  case 341: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 736 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4372 "pascal.tab.c"
    break;

  case 342: /* goto_statement: GOTOSY INTCONST  */
#line 739 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4378 "pascal.tab.c"
    break;

  case 343: /* goto_statement: GOTOSY IDENT  */
#line 740 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4384 "pascal.tab.c"
    break;

  case 344: /* if_statement: IFSY expression THENSY body_stmt  */
#line 743 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4390 "pascal.tab.c"
    break;

  case 345: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 744 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4396 "pascal.tab.c"
    break;

  case 346: /* $@24: %empty  */
#line 747 "pascal.y"
                           { }
#line 4402 "pascal.tab.c"
    break;

  case 347: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 747 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4408 "pascal.tab.c"
    break;

  case 348: /* case_tail: %empty  */
#line 750 "pascal.y"
    { (yyval.node) = NULL; }
#line 4414 "pascal.tab.c"
    break;

  case 349: /* case_tail: ELSESY statement_list  */
#line 751 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4420 "pascal.tab.c"
    break;

  case 350: /* case_tail: OTHERWISESY statement_list  */
#line 752 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4426 "pascal.tab.c"
    break;

  case 351: /* case_list: case_list SEMICOLON case_elem  */
#line 755 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4432 "pascal.tab.c"
    break;

  case 352: /* case_list: case_elem  */
#line 756 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4438 "pascal.tab.c"
    break;

  case 353: /* case_elem: constant_list COLON body_stmt  */
#line 759 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4444 "pascal.tab.c"
    break;

  case 354: /* case_elem: %empty  */
#line 760 "pascal.y"
     { (yyval.node) = NULL; }
#line 4450 "pascal.tab.c"
    break;

  case 355: /* constant_list: constant_list COMMA case_label  */
#line 763 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4456 "pascal.tab.c"
    break;

  case 356: /* constant_list: case_label  */
#line 764 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4462 "pascal.tab.c"
    break;

  case 357: /* case_label: b_expr  */
#line 767 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4468 "pascal.tab.c"
    break;

  case 358: /* case_label: b_expr DOTDOT b_expr  */
#line 768 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4474 "pascal.tab.c"
    break;

  case 359: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 771 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4480 "pascal.tab.c"
    break;

  case 360: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 774 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4486 "pascal.tab.c"
    break;

  case 361: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 777 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4492 "pascal.tab.c"
    break;

  case 362: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 778 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4498 "pascal.tab.c"
    break;

  case 363: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 779 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4504 "pascal.tab.c"
    break;

  case 364: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 782 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4510 "pascal.tab.c"
    break;

  case 365: /* with_open: with_open COMMA selector  */
#line 785 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4516 "pascal.tab.c"
    break;

  case 366: /* with_open: selector  */
#line 786 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4522 "pascal.tab.c"
    break;

  case 367: /* expression: simple_expression  */
#line 793 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4528 "pascal.tab.c"
    break;

  case 368: /* expression: expression INOP simple_expression  */
#line 794 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4534 "pascal.tab.c"
    break;

  case 369: /* expression: expression LTOP simple_expression  */
#line 795 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4540 "pascal.tab.c"
    break;

  case 370: /* expression: expression LEOP simple_expression  */
#line 796 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4546 "pascal.tab.c"
    break;

  case 371: /* expression: expression GTOP simple_expression  */
#line 797 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4552 "pascal.tab.c"
    break;

  case 372: /* expression: expression GEOP simple_expression  */
#line 798 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4558 "pascal.tab.c"
    break;

  case 373: /* expression: expression NEOP simple_expression  */
#line 799 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4564 "pascal.tab.c"
    break;

  case 374: /* expression: expression EQOP simple_expression  */
#line 800 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4570 "pascal.tab.c"
    break;

  case 375: /* expression: expression ISOP simple_expression  */
#line 801 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4576 "pascal.tab.c"
    break;

  case 376: /* simple_expression: term  */
#line 804 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4582 "pascal.tab.c"
    break;

  case 377: /* simple_expression: PLUS term  */
#line 805 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4588 "pascal.tab.c"
    break;

  case 378: /* simple_expression: MINUS term  */
#line 806 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4594 "pascal.tab.c"
    break;

  case 379: /* simple_expression: simple_expression PLUS term  */
#line 807 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4600 "pascal.tab.c"
    break;

  case 380: /* simple_expression: simple_expression MINUS term  */
#line 808 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4606 "pascal.tab.c"
    break;

  case 381: /* simple_expression: simple_expression OROP term  */
#line 809 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4612 "pascal.tab.c"
    break;

  case 382: /* simple_expression: simple_expression XOROP term  */
#line 810 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4618 "pascal.tab.c"
    break;

  case 383: /* term: power  */
#line 813 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4624 "pascal.tab.c"
    break;

  case 384: /* term: term MUL power  */
#line 814 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4630 "pascal.tab.c"
    break;

  case 385: /* term: term RDIV power  */
#line 815 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4636 "pascal.tab.c"
    break;

  case 386: /* term: term IDIV power  */
#line 816 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4642 "pascal.tab.c"
    break;

  case 387: /* term: term IMOD power  */
#line 817 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4648 "pascal.tab.c"
    break;

  case 388: /* term: term ANDOP power  */
#line 818 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4654 "pascal.tab.c"
    break;

  case 389: /* term: term MUL MINUS power  */
#line 819 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4660 "pascal.tab.c"
    break;

  case 390: /* term: term RDIV MINUS power  */
#line 820 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4666 "pascal.tab.c"
    break;

  case 391: /* term: term IDIV MINUS power  */
#line 821 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4672 "pascal.tab.c"
    break;

  case 392: /* term: term IMOD MINUS power  */
#line 822 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4678 "pascal.tab.c"
    break;

  case 393: /* term: term ANDOP MINUS power  */
#line 823 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4684 "pascal.tab.c"
    break;

  case 394: /* term: term SHIFTOP power  */
#line 824 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4690 "pascal.tab.c"
    break;

  case 395: /* term: term ASOP power  */
#line 825 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4696 "pascal.tab.c"
    break;

  case 396: /* power: factor  */
#line 828 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4702 "pascal.tab.c"
    break;

  case 397: /* power: power POWOP factor  */
#line 829 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4708 "pascal.tab.c"
    break;

  case 398: /* factor: selector  */
#line 832 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4714 "pascal.tab.c"
    break;

  case 399: /* factor: call_with_args  */
#line 833 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4720 "pascal.tab.c"
    break;

  case 400: /* factor: INTCONST  */
#line 834 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4726 "pascal.tab.c"
    break;

  case 401: /* factor: REALCONST  */
#line 835 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4732 "pascal.tab.c"
    break;

  case 402: /* factor: STRINGCONST  */
#line 836 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4738 "pascal.tab.c"
    break;

  case 403: /* factor: CHARCODE  */
#line 837 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4744 "pascal.tab.c"
    break;

  case 404: /* factor: LPARENT expression RPARENT  */
#line 838 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4750 "pascal.tab.c"
    break;

  case 405: /* factor: inherited_call  */
#line 839 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4756 "pascal.tab.c"
    break;

  case 406: /* factor: NOTSY factor  */
#line 840 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4762 "pascal.tab.c"
    break;

  case 407: /* factor: ATSIGN factor  */
#line 841 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4768 "pascal.tab.c"
    break;

  case 408: /* factor: LBRACK RBRACK  */
#line 842 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4774 "pascal.tab.c"
    break;

  case 409: /* factor: LBRACK set_member_list RBRACK  */
#line 843 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4780 "pascal.tab.c"
    break;

  case 410: /* set_member_list: set_member  */
#line 846 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4786 "pascal.tab.c"
    break;

  case 411: /* set_member_list: set_member_list COMMA set_member  */
#line 847 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4792 "pascal.tab.c"
    break;

  case 412: /* set_member: expression  */
#line 850 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4798 "pascal.tab.c"
    break;

  case 413: /* set_member: expression DOTDOT expression  */
#line 851 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4804 "pascal.tab.c"
    break;


#line 4808 "pascal.tab.c"

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

#line 857 "pascal.y"

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
