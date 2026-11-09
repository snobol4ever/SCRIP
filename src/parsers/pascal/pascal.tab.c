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
  YYSYMBOL_pv_mark = 171,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 172,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 173,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 174,           /* parameter_decl  */
  YYSYMBOL_param_type = 175,               /* param_type  */
  YYSYMBOL_routine_directives = 176,       /* routine_directives  */
  YYSYMBOL_pf_params = 177,                /* pf_params  */
  YYSYMBOL_pf_sections = 178,              /* pf_sections  */
  YYSYMBOL_pf_section = 179,               /* pf_section  */
  YYSYMBOL_enum_list = 180,                /* enum_list  */
  YYSYMBOL_enum_member = 181,              /* enum_member  */
  YYSYMBOL_id_list = 182,                  /* id_list  */
  YYSYMBOL_body = 183,                     /* body  */
  YYSYMBOL_statement_list = 184,           /* statement_list  */
  YYSYMBOL_185_19 = 185,                   /* $@19  */
  YYSYMBOL_186_20 = 186,                   /* $@20  */
  YYSYMBOL_body_stmt = 187,                /* body_stmt  */
  YYSYMBOL_188_21 = 188,                   /* $@21  */
  YYSYMBOL_statement = 189,                /* statement  */
  YYSYMBOL_statement_no_label = 190,       /* statement_no_label  */
  YYSYMBOL_try_statement = 191,            /* try_statement  */
  YYSYMBOL_handler_list = 192,             /* handler_list  */
  YYSYMBOL_handler = 193,                  /* handler  */
  YYSYMBOL_raise_statement = 194,          /* raise_statement  */
  YYSYMBOL_inherited_call = 195,           /* inherited_call  */
  YYSYMBOL_call = 196,                     /* call  */
  YYSYMBOL_recv_primary = 197,             /* recv_primary  */
  YYSYMBOL_call_with_args = 198,           /* call_with_args  */
  YYSYMBOL_argument_list = 199,            /* argument_list  */
  YYSYMBOL_argument = 200,                 /* argument  */
  YYSYMBOL_assignment = 201,               /* assignment  */
  YYSYMBOL_selector = 202,                 /* selector  */
  YYSYMBOL_expression_list = 203,          /* expression_list  */
  YYSYMBOL_compound_statement = 204,       /* compound_statement  */
  YYSYMBOL_goto_statement = 205,           /* goto_statement  */
  YYSYMBOL_if_statement = 206,             /* if_statement  */
  YYSYMBOL_case_statement = 207,           /* case_statement  */
  YYSYMBOL_208_22 = 208,                   /* $@22  */
  YYSYMBOL_case_tail = 209,                /* case_tail  */
  YYSYMBOL_case_list = 210,                /* case_list  */
  YYSYMBOL_case_elem = 211,                /* case_elem  */
  YYSYMBOL_constant_list = 212,            /* constant_list  */
  YYSYMBOL_case_label = 213,               /* case_label  */
  YYSYMBOL_while_statement = 214,          /* while_statement  */
  YYSYMBOL_repeat_statement = 215,         /* repeat_statement  */
  YYSYMBOL_for_statement = 216,            /* for_statement  */
  YYSYMBOL_with_statement = 217,           /* with_statement  */
  YYSYMBOL_with_open = 218,                /* with_open  */
  YYSYMBOL_expression = 219,               /* expression  */
  YYSYMBOL_simple_expression = 220,        /* simple_expression  */
  YYSYMBOL_term = 221,                     /* term  */
  YYSYMBOL_power = 222,                    /* power  */
  YYSYMBOL_factor = 223,                   /* factor  */
  YYSYMBOL_set_member_list = 224,          /* set_member_list  */
  YYSYMBOL_set_member = 225                /* set_member  */
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
#define YYLAST   1799

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  128
/* YYNRULES -- Number of rules.  */
#define YYNRULES  397
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  947

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
     468,   469,   470,   473,   476,   479,   480,   481,   484,   485,
     488,   489,   492,   493,   496,   497,   498,   501,   502,   503,
     503,   504,   504,   505,   505,   506,   506,   507,   507,   508,
     508,   509,   509,   510,   510,   511,   511,   514,   514,   517,
     517,   520,   520,   523,   523,   527,   527,   532,   535,   536,
     537,   540,   541,   544,   545,   546,   547,   548,   549,   550,
     551,   554,   555,   556,   557,   558,   561,   562,   572,   573,
     574,   577,   578,   581,   582,   583,   584,   585,   586,   589,
     590,   593,   596,   597,   600,   603,   603,   604,   604,   607,
     607,   610,   611,   612,   615,   616,   617,   618,   619,   620,
     621,   622,   623,   624,   625,   626,   627,   628,   629,   632,
     633,   634,   635,   638,   639,   640,   643,   644,   647,   648,
     651,   652,   653,   656,   657,   658,   659,   662,   663,   664,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,   681,   682,   685,   686,   687,   688,   691,   692,
     693,   694,   695,   696,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   710,   711,   714,   717,   718,   721,   722,
     725,   725,   728,   729,   730,   733,   734,   737,   738,   741,
     742,   745,   746,   749,   752,   755,   756,   757,   760,   763,
     764,   771,   772,   773,   774,   775,   776,   777,   778,   779,
     782,   783,   784,   785,   786,   787,   788,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   801,   802,   803,
     806,   807,   810,   811,   812,   813,   814,   815,   816,   817,
     818,   819,   820,   821,   824,   825,   828,   829
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
  "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "pv_mark",
  "parameter_list_opt", "parameter_decl_list", "parameter_decl",
  "param_type", "routine_directives", "pf_params", "pf_sections",
  "pf_section", "enum_list", "enum_member", "id_list", "body",
  "statement_list", "$@19", "$@20", "body_stmt", "$@21", "statement",
  "statement_no_label", "try_statement", "handler_list", "handler",
  "raise_statement", "inherited_call", "call", "recv_primary",
  "call_with_args", "argument_list", "argument", "assignment", "selector",
  "expression_list", "compound_statement", "goto_statement",
  "if_statement", "case_statement", "$@22", "case_tail", "case_list",
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

#define YYPACT_NINF (-575)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-323)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     313,   -23,     8,   177,  -575,   195,   160,  -575,  -575,   342,
    -575,    43,  -575,  -575,   438,   143,     8,   165,   179,   179,
     215,  -575,   -16,    49,   255,   261,   273,  -575,  -575,  -575,
     296,   179,   344,   372,   329,  -575,  -575,    84,   443,   143,
    -575,    85,   376,   188,  -575,   179,  -575,    96,    86,   248,
      46,   915,   252,   393,   -16,  -575,   437,   453,   468,   404,
     499,   510,   522,  -575,   392,  -575,  -575,  -575,  -575,   543,
    1337,  1030,  -575,  -575,     8,   179,   555,   486,  -575,   577,
    1030,  -575,   179,   589,    45,  -575,  -575,   568,   597,  -575,
    1337,   163,  1337,  -575,  1337,  1337,   535,  -575,   637,  -575,
    1337,   606,   611,  -575,   420,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,   419,   413,  -575,  1250,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,   179,  1081,   669,  -575,   516,   549,
     671,   660,   661,   693,   697,  -575,   710,   619,  -575,  -575,
    1152,  1152,  1152,  1108,  1337,  1152,  -575,  -575,  -575,   548,
       7,   606,   685,  -575,   552,   709,   449,   378,   284,   677,
    -575,   708,  -575,   708,   687,   687,   125,   704,  -575,  -575,
    -575,  -575,  1422,   728,   502,  -575,  -575,  -575,  -575,   754,
      48,  -575,   357,   302,   376,   100,  -575,    90,   796,  -575,
      34,   312,  -575,  -575,   750,   915,  -575,  -575,   474,   228,
     343,   552,   709,    30,   839,   141,  1576,  1593,   197,   197,
    1135,  1558,    20,  1718,   747,   772,  1177,   755,  -575,  1337,
     756,  -575,  1337,  1202,  1337,   759,  -575,  1337,  1337,  1337,
    1337,  1337,   352,    51,   819,   179,   760,   764,   776,  -575,
    -575,   179,   179,  -575,   143,     8,  -575,   778,   179,   781,
     -16,  -575,  -575,   284,   284,  -575,  1612,   560,  -575,  1631,
    -575,   789,  -575,  1337,  1337,  1337,  1337,  1337,  1337,  1337,
    1337,  1152,  1152,  1152,  1152,  1428,  1453,  1478,  1503,  1528,
    1152,  1152,  1152,   251,   798,   788,   777,   302,   302,    -2,
    -575,  -575,   487,  -575,  1558,   792,  1025,  1025,   169,   199,
     794,   795,   797,   807,   808,   809,   842,   281,  -575,   634,
    1362,   132,   858,   865,  -575,   529,   529,  1025,   529,   529,
     529,   529,  -575,   486,   179,   818,   179,   820,  -575,   179,
      32,  -575,   658,   889,  -575,  1362,   824,  -575,   486,  -575,
     226,  -575,  1337,  1337,  1337,  -575,  -575,   163,  -575,  -575,
    -575,  -575,   977,  -575,  -575,  -575,   489,  -575,   729,  1649,
      82,  -575,   811,  -575,  1227,  1558,  -575,   495,   841,  1718,
     843,   570,  1718,  -575,   498,  1718,   600,   572,  1718,  1718,
    1718,  1718,   853,  -575,   169,  -575,   387,  -575,  -575,  -575,
     486,   486,   397,   409,   143,    87,  1051,  -575,   179,  -575,
     -16,  1337,  1337,  -575,   617,   845,   378,   378,   378,   378,
     378,   378,   378,   378,   284,   284,   284,   284,  1152,   677,
    1152,   677,  1152,   677,  1152,   677,  1152,   677,   677,   677,
    -575,   179,   834,   179,   836,  -575,   179,    39,  -575,   668,
     837,   226,   704,  -575,   459,  -575,   529,   529,  1025,   855,
    -575,   314,   463,  -575,   279,    -2,   504,  -575,  -575,   849,
    -575,   851,    76,   317,   852,   856,   857,  -575,   686,  -575,
    -575,   864,   861,  -575,  -575,  -575,   328,   862,  -575,  -575,
    1387,   926,  1718,  1030,  1558,  1030,  1558,   328,   463,   463,
     148,  -575,  -575,  -575,  -575,   486,   938,   690,   708,   694,
     708,   736,   172,  -575,    17,   941,   872,   944,   945,   486,
     891,   948,   167,  -575,   949,   923,   672,  1718,  -575,   915,
    -575,   709,  1025,   916,  1337,  -575,  1337,  1337,  1558,  -575,
     880,   180,   241,  -575,   182,  -575,   509,   491,  -575,  1262,
    1287,  1337,  -575,  -575,  1312,  -575,  1030,   515,   905,   486,
     486,   486,   954,   956,  -575,  -575,  -575,  -575,  -575,   951,
     486,   486,  1718,  -575,   677,   677,   677,   677,   677,   737,
     708,   742,   708,   748,   300,  -575,    17,  -575,   935,  -575,
    -575,   463,   463,   373,  1025,   529,   529,  -575,   529,   529,
     529,   529,  -575,   169,  -575,  -575,  -575,  -575,   892,   893,
    -575,  -575,   918,  1030,   486,   486,   328,  -575,   486,   486,
    -575,    67,  1030,    88,   909,    64,  1631,  -575,  -575,   614,
    -575,  -575,   947,   973,  -575,    17,  -575,    17,   921,    17,
    -575,   955,   184,  -575,   929,  -575,   979,  -575,  -575,  -575,
     925,   226,  -575,   910,   980,  1337,  1337,  -575,  -575,   477,
     362,  -575,   758,  -575,  -575,  -575,  1665,  1684,  -575,    63,
    -575,   811,  -575,  -575,  -575,  -575,  -575,  -575,   519,  -575,
     523,  1718,  -575,   531,   983,   987,  1030,   989,   946,   952,
    -575,  -575,   486,   486,   990,   990,  -575,   968,  1000,   953,
      17,  -575,    17,   957,    17,  -575,  -575,   932,  -575,   440,
     463,   463,  -575,  -575,  -575,  -575,  -575,   435,   486,   486,
    -575,  -575,   486,   486,   172,   958,  1004,  -575,   959,   981,
     486,  -575,  -575,  -575,   950,  -575,  1362,  1362,   274,  -575,
    1558,   992,  -575,  -575,   872,   969,  -575,   226,  -575,    37,
     961,  1337,  -575,  1018,   226,  1019,  -575,  1020,   872,   734,
     827,  -575,  1025,  1025,  -575,  -575,   997,  1025,  -575,  -575,
    1337,  1337,  -575,   964,  -575,   201,  -575,  -575,  -575,  -575,
    -575,  1023,  -575,   226,   226,   872,   872,  1026,   972,   990,
     974,  -575,   226,  -575,  -575,   226,  -575,  -575,  -575,  -575,
    1029,   982,   486,   486,  1045,  1048,    68,   169,  -575,   226,
    -575,  1002,  1025,  1035,  -575,  -575,   532,  -575,  1008,  1001,
    -575,   872,  -575,  1337,  -575,  -575,  -575,  -575,  1718,  -575,
    1065,  -575,  -575,  -575,  -575,  -575,   148,  -575,   990,   990,
    -575,  -575,  -575,  1699,  1718,  -575,  1054,  -575,  -575,   872,
    1067,  1069,  -575,  -575,  -575,   226,  -575,   872,  1070,  -575,
    -575,   226,  1071,  1032,  -575,  -575,  -575,  -575,  -575,   226,
    1073,  -575,   765,  -575,  1362,  -575,  1362,  1068,  -575,  -575,
    1088,  1718,  -575,   872,  1090,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  1104,  1106,   872,  1107,  -575,  1109,  -575,   226,
      35,  -575,  1025,  1052,  1025,  -575,  -575,  1030,  1030,  1113,
    -575,   872,  -575,  -575,  -575,  1114,   872,   872,  -575,  -575,
    -575,  -575,   872,  -575,  1119,  -575,   571,  -575,  -575,  1073,
    -575,  -575,  -575,  -575,  1121,  -575,  -575,  -575,  1125,   872,
    -575,  -575,  -575,   328,  1127,  -575,  1128,  1132,  -575,  -575,
    1074,  -575,  -575,  -575,  1133,  -575,  -575
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   207,   243,    16,   173,     0,     0,   207,
       0,   268,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    65,    13,     0,
       0,   121,    38,    18,     0,     0,     0,   210,   172,     0,
     121,    19,     0,     0,   210,   245,   244,     0,     0,   247,
       0,     0,     0,   247,     0,     0,     0,   287,   283,   247,
     278,   267,     0,    68,   284,    67,   248,   251,   264,   265,
     266,   255,     0,   286,   254,     0,   256,   257,   258,   259,
     260,   261,   262,   263,     0,   121,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   384,   387,   385,   386,
     322,     0,   289,   389,   383,   382,     0,   351,   360,   367,
     380,   230,   120,   230,     0,     0,     0,     0,    51,    54,
      52,    53,   123,   103,   162,   133,    75,    74,   126,     0,
       0,    79,     0,   128,    63,     0,   207,     0,     0,   242,
       0,     0,   207,   183,     0,   268,   326,   327,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,   268,   268,
       0,     0,     0,   279,   280,     0,     0,     0,   321,     0,
       0,   319,     0,     0,     0,     0,   316,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,     0,   207,
     207,     0,     0,     4,     0,     0,    12,     0,     0,     0,
       0,    21,   390,   361,   362,   392,   396,     0,   394,     0,
     391,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,     0,    50,   129,   130,    80,
      81,   241,     0,   240,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,   154,     0,
       0,     0,    94,     0,    91,     0,     0,     0,     0,     0,
       0,     0,   207,   210,     0,     0,     0,     0,   209,   243,
       0,   212,     0,   226,   174,     0,     0,   207,   210,    12,
       0,   246,     0,     0,     0,   249,   249,     0,   330,   325,
     249,   288,   283,   252,   253,   293,     0,   303,   382,   304,
       0,    77,   247,   247,     0,     0,   292,     0,   320,   313,
     317,     0,   324,   295,     0,   308,   285,     0,   309,   310,
     311,   312,     0,    60,     0,    57,     0,   207,   207,   207,
     210,   210,     0,     0,    24,     0,    31,   207,    26,   207,
      25,     0,     0,   393,   388,   315,   352,   353,   354,   355,
     356,   357,   358,   359,   365,   363,   364,   366,     0,   370,
       0,   371,     0,   372,     0,   368,     0,   369,   378,   379,
     381,     0,     0,     0,     0,   229,   243,     0,   232,     0,
       0,     0,     0,   122,     0,   125,     0,     0,     0,    50,
     149,     0,   139,   144,     0,    71,     0,    69,    72,     0,
      98,     0,   105,     0,     0,     0,     0,   104,     0,   207,
     207,     0,     0,   207,   207,    96,   162,     0,   152,   155,
       0,     0,    42,   121,     0,   121,     0,   162,   131,   132,
     127,   136,   137,   135,   138,   210,     0,     0,   230,     0,
     230,     0,     0,   208,     0,     0,   179,     0,     0,   210,
       0,     0,   100,   102,     0,     0,     0,   344,   343,   268,
     348,   349,   338,   328,     0,   290,     0,     0,     0,    66,
       0,     0,     0,   273,     0,   281,     0,     0,   291,     0,
       0,     0,   318,   294,     0,   314,   121,     0,     0,   210,
     210,   210,     0,     0,   207,   207,    23,   247,   247,    33,
     210,   210,   397,   395,   375,   376,   377,   373,   374,     0,
     230,     0,   230,     0,     0,   228,     0,    87,    88,   239,
      76,   140,   141,     0,     0,     0,     0,   124,     0,     0,
       0,     0,   134,     0,   103,   207,   207,   106,     0,     0,
     207,   207,   115,   121,   210,   210,   162,   207,   210,   210,
     153,     0,   121,   322,     0,     0,    42,    41,    85,     0,
      95,    93,     0,     0,   226,     0,   213,     0,     0,     0,
     211,     0,   221,   225,   216,   177,     0,    12,   175,   176,
       0,     0,   184,     0,   226,     0,     0,   249,   250,   341,
     332,   336,     0,   340,   249,   302,   324,   305,    78,     0,
     269,   275,   270,   247,   272,   282,    73,   301,     0,   299,
       0,   323,   297,     0,     0,   103,   121,     0,     0,     0,
     226,   226,   210,   210,    29,    30,   247,     0,     0,     0,
       0,   237,     0,     0,     0,   231,   233,     0,   151,     0,
     142,   143,   146,   147,   145,   148,    70,     0,   210,   210,
     207,   207,   210,   210,     0,     0,     0,   157,     0,     0,
     210,   159,   160,   164,     0,   156,     0,     0,     0,    44,
       0,     0,    92,   226,   193,   217,   215,     0,   220,     0,
       0,     0,   227,     0,     0,     0,   101,     0,   181,     0,
       0,   347,     0,   338,   247,   247,     0,     0,   249,   329,
       0,     0,   249,     0,   274,     0,   300,   298,   296,    59,
      61,     0,   226,     0,     0,   197,   199,     0,     0,    32,
       0,   226,     0,   235,   234,     0,   236,    89,   150,    99,
       0,     0,   210,   210,     0,     0,     0,     0,   107,     0,
      97,     0,   171,     0,    48,    46,     0,    45,     0,     0,
      82,   189,    12,     0,   214,   224,   223,   222,   218,   180,
       0,   226,   178,    12,   249,   249,   342,   335,   334,   333,
     331,   339,   337,     0,   306,   277,     0,   271,    58,   201,
       0,     0,    12,    12,   226,     0,    20,    27,     0,   238,
     108,     0,     0,     0,   110,   111,   116,   117,   158,     0,
     166,   169,     0,   164,     0,    43,     0,     0,    83,    12,
       0,   219,   226,   195,     0,   346,   345,   307,   249,    12,
     226,   226,     0,     0,   185,     0,   226,     0,   112,     0,
       0,   161,   171,     0,   171,    47,    49,   121,   121,     0,
     194,   191,    12,   182,   276,     0,   205,   203,   198,   200,
      12,   226,    28,   109,     0,   114,     0,   168,   163,   165,
      90,    84,   190,    12,     0,   202,    12,    12,     0,   187,
     113,   119,   118,   162,     0,   196,     0,     0,   186,    12,
       0,   192,   206,   204,     0,   170,   188
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -575,  -575,  -575,  -575,  -575,   -65,   897,  -575,  -575,  -575,
    -575,  -575,  -575,  -575,   902,   -31,  -329,  -575,  -575,   -64,
     898,   -45,   904,    25,   -70,   -46,   763,  -574,   -42,   270,
    -251,   -76,  -575,  -575,  -575,  -399,  -568,  -575,  -575,  -575,
    -575,  -208,  -575,   492,  -265,  -282,  -201,  -485,  -575,   674,
    -575,  -575,   289,  -575,   264,   268,   917,   -33,  -575,  -575,
    -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,   527,   130,   452,   665,  -555,   149,  -141,
    -575,   594,  -575,   727,   618,  -575,   -69,  -575,  -575,  -332,
    -575,  -184,   501,  -575,  -575,   512,  -575,   -36,  -575,  -575,
     -34,  -188,   650,  -575,   102,   963,  -575,  -575,  -575,  -575,
    -575,  -575,  -575,   424,  -509,   422,  -575,  -575,  -575,  -575,
    -575,    38,  1509,   -83,  -213,  -104,  -575,   784
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     251,   559,   687,    37,    39,    40,   481,   806,   615,   450,
      54,    55,    41,    42,   103,   152,   456,   457,   105,   178,
     360,   179,   868,   898,   487,   514,   299,   467,   715,   890,
     180,   181,   182,   183,   649,   452,   453,   306,   307,   308,
     612,   933,   802,   478,   860,   861,    45,    46,    28,   637,
     823,   339,   910,   939,   869,   923,   812,   902,   842,   843,
     879,   927,   926,    77,   188,   330,   331,   634,   506,   284,
     437,   438,   292,   293,   309,    29,    50,   195,    51,   518,
     519,   106,   107,   108,   532,   533,   109,   153,   111,   112,
     154,   356,   357,   114,   155,   377,   116,   117,   118,   119,
     522,   756,   650,   651,   862,   653,   120,   121,   122,   123,
     203,   359,   157,   158,   159,   160,   257,   258
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   176,   622,   361,   190,   104,   507,   175,    72,   127,
     176,   341,    78,   652,   520,   110,   175,   113,   523,   706,
     199,   696,   285,   631,   205,    85,   707,    11,   367,   177,
     212,   451,   454,   488,   489,   374,   252,   502,   177,   334,
     915,   260,   578,   444,   574,   815,   294,   346,    33,   234,
     193,    85,   490,     8,   311,   176,   383,   201,   253,   254,
      52,   175,   419,   421,   423,   425,   427,   428,   429,   728,
     735,   210,   736,   502,   738,    56,    86,   312,    53,   313,
     762,   597,   314,   177,    10,    57,   361,   335,   347,    68,
      73,    81,   556,   632,   211,   723,   176,   503,   324,   184,
     175,   175,    34,   211,   575,   362,   363,   707,   156,   187,
     336,   916,   102,   816,   537,   384,   325,   491,   492,   493,
     494,   719,   326,   763,   177,    58,   327,   724,   200,   729,
     204,   856,   206,   207,  -243,   783,  -243,   784,   213,   786,
     528,   176,    69,    74,    79,    74,    85,   175,   726,   104,
     322,   614,   210,   115,    79,   328,    80,   361,    79,   110,
     483,   113,   104,   104,   581,   582,   329,    14,    15,   177,
      16,   349,   110,   110,   113,   113,   536,    12,   430,   529,
     324,   256,   259,   583,   211,    85,    17,    85,   414,   415,
     416,   417,    18,   202,   484,    19,    20,    21,   325,    22,
      87,   289,   585,   586,   326,   564,    85,   565,   327,   566,
     660,   567,   664,   568,   194,   294,    88,    89,    90,    38,
     102,    91,    92,   857,   176,   459,   643,    95,   176,   460,
     175,   837,   294,    85,    93,   461,    75,    94,    97,   150,
      23,    43,   745,   740,   652,   455,   661,    76,   329,    24,
      25,   344,   177,    26,    13,    44,   177,   369,   102,   431,
     372,    95,   375,   211,   102,   372,   378,   379,   380,   381,
     176,   662,    97,   352,   511,   462,   619,   432,   621,   463,
     211,    99,   663,   433,   100,   101,   476,   434,   464,   465,
     466,    49,   102,   531,   534,   176,    82,   115,   177,    61,
     124,   175,   512,   700,   701,   125,   477,    83,   431,    62,
     115,   115,   358,   201,   176,   751,   435,     1,   358,   699,
     658,   102,   759,   177,     2,   358,   432,   436,   275,   276,
     277,    59,   433,   585,   586,   648,   434,    60,   814,   807,
     278,   279,   177,   598,   592,   820,   318,   319,   482,    65,
     808,   668,   670,   599,   300,   127,   673,   626,   320,   628,
     345,   280,   337,    72,   301,    78,   281,   753,   585,   586,
      79,   176,    63,   482,   840,   841,   436,   587,   754,   321,
     515,   516,   517,   848,    66,   263,   849,   702,   703,   704,
     705,   264,   265,   266,   267,   268,   269,   804,   805,   177,
     858,    30,   382,   755,    44,    67,    31,   618,   303,   620,
      79,   315,   316,   176,   176,   176,   176,   304,   305,   175,
     175,   175,   175,   317,   270,   271,   832,   585,   586,   691,
     835,   693,   272,   273,   176,    34,   290,   548,   698,   562,
     256,   177,   177,   177,   177,    79,   885,   554,   940,   521,
      79,   219,   887,   496,   262,    79,   274,   135,   176,   555,
     891,   459,   177,   131,   175,   789,   358,    79,   510,   126,
     674,   461,   220,   104,   221,   222,   176,   223,   217,  -289,
     218,  -289,   175,   110,   216,   113,   177,   826,   684,   685,
     914,   263,   875,   876,   585,   586,    70,   264,   265,   266,
     267,   268,   269,    71,   177,   788,   176,   588,   589,   580,
      35,   462,   342,   128,    36,   463,   343,   528,   616,   590,
     552,   553,   809,   176,   464,   465,   466,   716,   300,   129,
     270,   585,   586,   176,   177,   895,   725,   896,   301,   175,
     591,   666,   176,   752,   130,   442,   904,   524,   175,   528,
     187,   177,   443,   524,   525,   176,   524,   176,   132,   176,
     538,   177,   593,   543,   656,   657,   302,   524,   458,   594,
     177,   176,   743,   593,   665,   236,    84,   524,    44,   671,
     675,   524,   303,   177,   766,   177,   133,   177,   767,   524,
     864,   304,   305,   448,   765,   208,   768,   865,   134,   177,
     771,   168,   169,   170,   171,   449,   176,  -287,   237,  -287,
     513,   220,   175,   221,   222,   138,   223,   779,   402,   139,
     176,   115,   176,   403,   176,   623,   358,   244,   541,   245,
     541,   186,   246,   542,   177,   545,    47,    48,  -315,   640,
     196,   358,   358,   931,   197,   247,   358,   932,   177,    64,
     177,   248,   177,   189,   458,   249,   287,   288,   250,  -315,
     176,  -315,  -315,    47,   544,   192,   175,   176,  -315,  -315,
    -315,  -315,   730,   198,   176,  -322,  -288,   731,  -288,   677,
     678,   679,   214,   749,   750,   828,   829,   215,   177,   647,
     688,   689,    79,   185,   479,   177,  -322,   209,  -322,  -322,
     191,   210,   177,   176,   176,  -322,  -322,  -322,  -322,   353,
     354,   513,   176,   323,   263,   176,    79,   235,   504,   338,
     264,   265,   266,   267,   268,   269,    79,   176,   576,   176,
     238,   177,   177,   211,   717,   718,   239,   240,   721,   722,
     177,   241,   232,   177,    79,   242,   603,   870,    79,   216,
     625,   824,    79,   270,   627,   177,   282,   177,   874,   168,
     169,   170,   171,   286,   482,   482,   390,   391,   261,   243,
     226,   227,   283,   734,   633,   176,   263,   882,   883,   818,
     291,   176,   264,   265,   266,   267,   268,   269,   261,   176,
     226,   526,   298,   748,    79,    79,   629,   690,   833,   834,
      79,   333,   692,   177,   899,   332,    79,   310,   694,   177,
     340,   364,   777,   778,   905,   270,   757,   177,   758,   176,
     365,   920,   921,   757,   385,   893,   440,   176,   176,   775,
     776,   368,   370,   175,   175,   376,   387,   924,   790,   791,
     388,   297,   794,   795,   825,   928,   633,   177,   441,   495,
     801,   871,   389,   386,   397,   177,   177,   399,   934,   392,
     393,   936,   937,   458,   509,   405,    47,   348,   445,   263,
     469,   470,   475,   471,   944,   264,   265,   266,   267,   268,
     269,   263,   811,   472,   473,   474,   485,   264,   265,   266,
     267,   268,   269,   486,   498,   633,   500,   633,   505,   633,
     508,   439,   482,   530,   482,   539,   546,   540,   270,   544,
     570,   513,   572,   577,   549,   550,   551,   468,    87,   584,
     270,   839,   852,   853,   560,   595,   561,   596,   600,   606,
     847,   617,   601,   602,    88,    89,    90,   607,   611,    91,
      92,   645,   497,   624,   499,   646,   635,   501,   636,   638,
     639,   641,    93,   642,   644,    94,   659,   654,   676,   680,
     633,   681,   633,   697,   633,   263,   686,   727,   710,   711,
     873,   264,   265,   266,   267,   268,   269,   732,   733,    95,
     714,   737,   741,   739,   742,   744,   746,    96,   769,   747,
      97,    98,   770,   884,   772,    85,   604,   605,   780,    99,
     608,   609,   100,   101,   270,   781,   773,   513,   787,   798,
     102,   800,   774,   782,   513,  -322,    47,   785,   797,   799,
     810,   901,   813,   819,   821,   822,   803,   830,   838,   906,
     907,   844,   845,   846,   850,   912,  -322,   817,  -322,  -322,
     836,   210,   851,   513,   513,  -322,  -322,  -322,  -322,   569,
     854,   571,   513,   855,   573,   513,   161,   162,    14,    15,
     929,    16,   859,   863,   867,   557,   163,   458,   866,   513,
     872,   878,   880,   211,   881,   886,   888,    17,   892,   446,
     447,   682,   683,    18,   164,   165,    19,    20,   558,   448,
      22,   166,   889,   900,   167,   903,   897,   168,   169,   170,
     171,   449,   168,   169,   170,   171,   172,   161,   162,   908,
     173,   909,   911,   174,   913,   513,   918,   163,   922,   925,
     332,   513,   708,   709,   930,   102,   935,   712,   713,   513,
     938,    23,   941,   942,   720,   164,   165,   943,   946,   945,
      24,    25,   166,   396,    26,   167,   394,   547,   400,   395,
     610,   140,   894,   168,   169,   170,   171,   172,   919,   513,
     917,   233,   141,   142,   174,   398,   796,   630,   695,   579,
     143,   255,   144,   764,   655,   145,   102,   827,   140,   831,
     146,   147,   148,   149,   150,   371,   563,     0,     0,   141,
     142,     0,   439,     0,     0,   140,   151,   143,     0,   144,
     355,     0,   145,   102,     0,     0,     0,   146,   147,   148,
     149,   150,     0,     0,   143,     0,   144,     0,     0,   145,
     140,     0,     0,   151,   146,   147,   148,   149,   150,     0,
     102,   141,   142,     0,     0,     0,     0,   792,   793,   143,
     151,   144,   366,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   373,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,   224,   143,
     151,   144,   535,     0,   145,     0,     0,   102,     0,   146,
     147,   148,   149,   150,     0,   140,     0,     0,     0,   225,
       0,   226,   227,     0,     0,   151,   141,   142,   228,   229,
     230,   231,   102,     0,   143,   468,   144,   667,     0,   145,
     140,     0,   332,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   669,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   672,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,     0,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   480,     0,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   480,     0,     0,   145,     0,     0,   102,     0,   146,
     147,   148,   149,   613,     0,     0,   -50,   -50,     0,     0,
     294,   140,     0,     0,     0,   151,   -50,   -50,   -50,     0,
       0,   295,   102,   418,   296,     0,   297,     0,   -50,     0,
     143,     0,   144,     0,     0,   145,   140,     0,     0,   -50,
     146,   147,   148,   149,   150,     0,     0,     0,   420,     0,
       0,     0,     0,     0,     0,   143,   151,   144,   211,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   422,     0,     0,     0,     0,     0,     0,
     143,   151,   144,     0,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   424,     0,
       0,     0,     0,     0,     0,   143,   151,   144,     0,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   426,     0,     0,     0,     0,     0,     0,
     143,   151,   144,     0,     0,   145,     0,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,     0,   350,
       0,     0,   164,   165,     0,     0,   151,     0,   263,     0,
       0,     0,   167,   102,   264,   265,   266,   267,   268,   269,
     168,   169,   170,   171,   172,   263,     0,     0,     0,     0,
       0,   264,   265,   266,   267,   268,   269,     0,     0,     0,
       0,     0,     0,   102,   263,     0,     0,   270,   351,     0,
     264,   265,   266,   267,   268,   269,     0,     0,     0,     0,
       0,     0,     0,   263,   270,     0,     0,     0,   401,   264,
     265,   266,   267,   268,   269,     0,     0,     0,     0,     0,
       0,   263,     0,   270,     0,     0,   404,   264,   265,   266,
     267,   268,   269,     0,     0,     0,     0,   263,     0,   527,
       0,     0,   270,   264,   265,   266,   267,   268,   269,     0,
       0,     0,     0,     0,     0,     0,   263,     0,     0,     0,
     270,   760,   264,   265,   266,   267,   268,   269,     0,     0,
       0,   263,     0,     0,   761,     0,   270,   264,   265,   266,
     267,   268,   269,     0,     0,     0,     0,     0,     0,     0,
     263,     0,   877,     0,     0,   270,   264,   265,   266,   267,
     268,   269,   406,   407,   408,   409,   410,   411,   412,   413,
     270,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270
};

static const yytype_int16 yycheck[] =
{
      65,    71,   487,   211,    80,    51,   335,    71,    39,    54,
      80,   195,    45,   522,   346,    51,    80,    51,   350,   593,
      89,   576,   163,     6,    93,     5,   594,     2,   216,    71,
      99,   296,   297,   315,   316,   223,   140,     5,    80,     5,
       5,   145,   441,   294,     5,     8,    48,    17,     5,   125,
       5,     5,   317,    76,     6,   125,     5,    91,   141,   142,
      76,   125,   275,   276,   277,   278,   279,   280,   281,     5,
     625,    64,   627,     5,   629,    26,    30,    29,    94,    31,
      17,     5,    34,   125,    76,    36,   294,    53,    58,     5,
       5,     5,     5,    76,    96,    28,   166,    65,     8,    74,
     164,   165,    59,    96,    65,    85,    86,   675,    70,    64,
      76,    76,    95,    76,   365,    64,    26,   318,   319,   320,
     321,   606,    32,    60,   166,    76,    36,    60,    90,    65,
      92,    63,    94,    95,    58,   690,    60,   692,   100,   694,
      58,   211,    58,    58,    58,    58,     5,   211,    60,   195,
      50,   480,    64,    51,    58,    65,    60,   365,    58,   195,
      28,   195,   208,   209,   446,   447,    76,     7,     8,   211,
      10,    30,   208,   209,   208,   209,   364,     0,   282,    97,
       8,   143,   144,   448,    96,     5,    26,     5,   271,   272,
     273,   274,    32,    91,    62,    35,    36,    37,    26,    39,
       3,    76,    54,    55,    32,   418,     5,   420,    36,   422,
      30,   424,    30,   426,    84,    48,    19,    20,    21,    76,
      95,    24,    25,   797,   294,    26,    59,    64,   298,    30,
     294,    30,    48,     5,    37,    36,    48,    40,    75,    76,
      80,    76,   641,    59,   753,    76,     5,    59,    76,    89,
      90,    23,   294,    93,    59,    76,   298,   219,    95,     8,
     222,    64,   224,    96,    95,   227,   228,   229,   230,   231,
     340,    30,    75,    76,   339,    76,   484,    26,   486,    80,
      96,    84,    41,    32,    87,    88,     5,    36,    89,    90,
      91,    76,    95,   362,   363,   365,    48,   195,   340,    26,
      48,   365,    76,   585,   586,    53,    25,    59,     8,    36,
     208,   209,   210,   347,   384,   647,    65,     4,   216,   584,
     528,    95,   654,   365,    11,   223,    26,    76,    44,    45,
      46,    76,    32,    54,    55,   519,    36,    76,   737,    65,
      56,    57,   384,    26,    65,   744,    44,    45,   310,     5,
      76,   539,   540,    36,    26,   400,   544,   498,    56,   500,
      17,    77,    50,   394,    36,   398,    82,     5,    54,    55,
      58,   441,    76,   335,   773,   774,    76,    63,    16,    77,
     342,   343,   344,   782,    12,    42,   785,   588,   589,   590,
     591,    48,    49,    50,    51,    52,    53,   726,   727,   441,
     799,    59,    50,    41,    76,    76,    64,   483,    80,   485,
      58,    54,    55,   483,   484,   485,   486,    89,    90,   483,
     484,   485,   486,    66,    81,    47,   758,    54,    55,   570,
     762,   572,    54,    55,   504,    59,   166,    50,    65,   401,
     402,   483,   484,   485,   486,    58,   845,    50,   933,   347,
      58,    38,   851,   323,     5,    58,    78,    65,   528,    50,
     859,    26,   504,    59,   528,    30,   364,    58,   338,    76,
     546,    36,    59,   519,    61,    62,   546,    64,    59,    59,
      61,    61,   546,   519,    64,   519,   528,   752,   557,   558,
     889,    42,   824,   825,    54,    55,    53,    48,    49,    50,
      51,    52,    53,    60,   546,    65,   576,    44,    45,    50,
      72,    76,    38,    76,    76,    80,    42,    58,   480,    56,
     390,   391,   730,   593,    89,    90,    91,   603,    26,    76,
      81,    54,    55,   603,   576,   864,   612,   866,    36,   603,
      77,    50,   612,    66,    76,    58,   878,    58,   612,    58,
      64,   593,    65,    58,    65,   625,    58,   627,    59,   629,
      65,   603,    58,    65,   526,   527,    64,    58,   298,    65,
     612,   641,   637,    58,    65,    59,    49,    58,    76,   541,
      65,    58,    80,   625,    65,   627,    76,   629,    65,    58,
      58,    89,    90,    64,   663,    60,    65,    65,    76,   641,
     676,    72,    73,    74,    75,    76,   676,    59,    59,    61,
     340,    59,   676,    61,    62,    72,    64,   686,    58,    76,
     690,   519,   692,    63,   694,   495,   524,     8,    58,    10,
      58,    76,    13,    63,   676,    63,    18,    19,    38,   509,
      72,   539,   540,    72,    76,    26,   544,    76,   690,    31,
     692,    32,   694,    76,   384,    36,   164,   165,    39,    59,
     730,    61,    62,    45,    64,    76,   730,   737,    68,    69,
      70,    71,    58,    76,   744,    38,    59,    63,    61,   549,
     550,   551,    76,   645,   646,   754,   755,    76,   730,    17,
     560,   561,    58,    75,    60,   737,    59,    60,    61,    62,
      82,    64,   744,   773,   774,    68,    69,    70,    71,   208,
     209,   441,   782,   186,    42,   785,    58,    48,    60,   192,
      48,    49,    50,    51,    52,    53,    58,   797,    60,   799,
      59,   773,   774,    96,   604,   605,    76,    76,   608,   609,
     782,    48,   124,   785,    58,    48,    60,   812,    58,    64,
      60,    17,    58,    81,    60,   797,    79,   799,   823,    72,
      73,    74,    75,    76,   726,   727,   239,   240,    59,    59,
      61,    62,    64,   624,   504,   845,    42,   842,   843,   741,
      76,   851,    48,    49,    50,    51,    52,    53,    59,   859,
      61,    62,    64,   644,    58,    58,    60,    60,   760,   761,
      58,     5,    60,   845,   869,   187,    58,    53,    60,   851,
      60,    64,   682,   683,   879,    81,    58,   859,    60,   889,
      48,   897,   898,    58,     5,    60,    28,   897,   898,   680,
     681,    76,    76,   897,   898,    76,    76,   902,   708,   709,
      76,    64,   712,   713,    17,   910,   576,   889,    60,   322,
     720,   813,    76,   235,    76,   897,   898,    76,   923,   241,
     242,   926,   927,   593,   337,    76,   248,    28,    76,    42,
      76,    76,    30,    76,   939,    48,    49,    50,    51,    52,
      53,    42,   733,    76,    76,    76,    28,    48,    49,    50,
      51,    52,    53,    28,    76,   625,    76,   627,     9,   629,
      76,   283,   864,    92,   866,    64,    53,    64,    81,    64,
      76,   641,    76,    76,   387,   388,   389,   299,     3,    64,
      81,   772,   792,   793,   397,    76,   399,    76,    76,    65,
     781,     5,    76,    76,    19,    20,    21,    76,    76,    24,
      25,    18,   324,     5,   326,    22,     5,   329,    76,     5,
       5,    60,    37,     5,     5,    40,    76,    41,    53,     5,
     690,     5,   692,    28,   694,    42,    15,    58,    76,    76,
     821,    48,    49,    50,    51,    52,    53,    30,     5,    64,
      62,    60,    53,    28,     5,    60,    76,    72,     5,     9,
      75,    76,     5,   844,     5,     5,   469,   470,    30,    84,
     473,   474,    87,    88,    81,     5,    60,   737,    76,     5,
      95,    30,    60,    60,   744,    38,   398,    60,    60,    60,
      28,   872,    53,     5,     5,     5,    76,    30,     5,   880,
     881,     5,    60,    59,     5,   886,    59,    76,    61,    62,
      76,    64,    60,   773,   774,    68,    69,    70,    71,   431,
       5,   433,   782,     5,   436,   785,    26,    27,     7,     8,
     911,    10,    60,    28,    63,    14,    36,   797,    60,   799,
       5,    17,     5,    96,     5,     5,     5,    26,     5,    54,
      55,   554,   555,    32,    54,    55,    35,    36,    37,    64,
      39,    61,    60,     5,    64,     5,    28,    72,    73,    74,
      75,    76,    72,    73,    74,    75,    76,    26,    27,     5,
      80,     5,     5,    83,     5,   845,    64,    36,     5,     5,
     502,   851,   595,   596,     5,    95,     5,   600,   601,   859,
       5,    80,     5,     5,   607,    54,    55,     5,     5,    65,
      89,    90,    61,   246,    93,    64,   244,   384,   250,   245,
     476,    43,   863,    72,    73,    74,    75,    76,   894,   889,
     892,    80,    54,    55,    83,   248,   714,   502,   574,   442,
      62,    63,    64,   661,   524,    67,    95,   753,    43,   757,
      72,    73,    74,    75,    76,   222,   402,    -1,    -1,    54,
      55,    -1,   574,    -1,    -1,    43,    88,    62,    -1,    64,
      65,    -1,    67,    95,    -1,    -1,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    62,    -1,    64,    -1,    -1,    67,
      43,    -1,    -1,    88,    72,    73,    74,    75,    76,    -1,
      95,    54,    55,    -1,    -1,    -1,    -1,   710,   711,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    38,    62,
      88,    64,    65,    -1,    67,    -1,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    43,    -1,    -1,    -1,    59,
      -1,    61,    62,    -1,    -1,    88,    54,    55,    68,    69,
      70,    71,    95,    -1,    62,   707,    64,    65,    -1,    67,
      43,    -1,   714,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    65,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,    72,
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
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,    33,
      -1,    -1,    54,    55,    -1,    -1,    88,    -1,    42,    -1,
      -1,    -1,    64,    95,    48,    49,    50,    51,    52,    53,
      72,    73,    74,    75,    76,    42,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    42,    -1,    -1,    81,    65,    -1,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    81,    -1,    -1,    -1,    66,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,
      -1,    -1,    81,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      81,    66,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    42,    -1,    -1,    60,    -1,    81,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    63,    -1,    -1,    81,    48,    49,    50,    51,
      52,    53,   263,   264,   265,   266,   267,   268,   269,   270,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    99,   100,   103,   104,   106,    76,   101,
      76,   121,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    89,    90,    93,   105,   156,   183,
      59,    64,   102,     5,    59,    72,    76,   111,    76,   112,
     113,   120,   121,    76,    76,   154,   155,   182,   182,    76,
     184,   186,    76,    94,   118,   119,    26,    36,    76,    76,
      76,    26,    36,    76,   182,     5,    12,    76,     5,    58,
      53,    60,   113,     5,    58,    48,    59,   171,   155,    58,
      60,     5,    48,    59,   171,     5,    30,     3,    19,    20,
      21,    24,    25,    37,    40,    64,    72,    75,    76,    84,
      87,    88,    95,   122,   123,   126,   189,   190,   191,   194,
     195,   196,   197,   198,   201,   202,   204,   205,   206,   207,
     214,   215,   216,   217,    48,    53,    76,   119,    76,    76,
      76,    59,    59,    76,    76,    65,   103,   107,    72,    76,
      43,    54,    55,    62,    64,    67,    72,    73,    74,    75,
      76,    88,   123,   195,   198,   202,   219,   220,   221,   222,
     223,    26,    27,    36,    54,    55,    61,    64,    72,    73,
      74,    75,    76,    80,    83,   117,   122,   126,   127,   129,
     138,   139,   140,   141,   121,   182,    76,    64,   172,    76,
     129,   182,    76,     5,   172,   185,    72,    76,    76,   184,
     219,   198,   202,   218,   219,   184,   219,   219,    60,    60,
      64,    96,   184,   219,    76,    76,    64,    59,    61,    38,
      59,    61,    62,    64,    38,    59,    61,    62,    68,    69,
      70,    71,   182,    80,   129,    48,    59,    59,    59,    76,
      76,    48,    48,    59,     8,    10,    13,    26,    32,    36,
      39,   108,   223,   221,   221,    63,   219,   224,   225,   219,
     223,    59,     5,    42,    48,    49,    50,    51,    52,    53,
      81,    47,    54,    55,    78,    44,    45,    46,    56,    57,
      77,    82,    79,    64,   177,   177,    76,   141,   141,    76,
     127,    76,   180,   181,    48,    59,    62,    64,    64,   134,
      26,    36,    64,    80,    89,    90,   145,   146,   147,   182,
      53,     6,    29,    31,    34,    54,    55,    66,    44,    45,
      56,    77,    50,   171,     8,    26,    32,    36,    65,    76,
     173,   174,   182,     5,     5,    53,    76,    50,   171,   159,
      60,   189,    38,    42,    23,    17,    17,    58,    28,    30,
      33,    65,    76,   190,   190,    65,   199,   200,   202,   219,
     128,   139,    85,    86,    64,    48,    65,   199,    76,   219,
      76,   203,   219,    65,   199,   219,    76,   203,   219,   219,
     219,   219,    50,     5,    64,     5,   182,    76,    76,    76,
     171,   171,   182,   182,   112,   120,   104,    76,   154,    76,
     118,    66,    58,    63,    65,    76,   220,   220,   220,   220,
     220,   220,   220,   220,   221,   221,   221,   221,    55,   222,
      55,   222,    55,   222,    55,   222,    55,   222,   222,   222,
     223,     8,    26,    32,    36,    65,    76,   178,   179,   182,
      28,    60,    58,    65,   128,    76,    54,    55,    64,    76,
     117,   142,   143,   144,   142,    76,   124,   125,   127,    26,
      30,    36,    76,    80,    89,    90,    91,   135,   182,    76,
      76,    76,    76,    76,    76,    30,     5,    25,   151,    60,
      64,   114,   219,    28,    62,    28,    28,   132,   143,   143,
     142,   144,   144,   144,   144,   171,   172,   182,    76,   182,
      76,   182,     5,    65,    60,     9,   176,   114,    76,   171,
     172,   103,    76,   127,   133,   219,   219,   219,   187,   188,
     187,   202,   208,   187,    58,    65,    62,    60,    58,    97,
      92,   184,   192,   193,   184,    65,   199,   128,    65,    64,
      64,    58,    63,    65,    64,    63,    53,   124,    50,   171,
     171,   171,   172,   172,    50,    50,     5,    14,    37,   109,
     171,   171,   219,   225,   222,   222,   222,   222,   222,   182,
      76,   182,    76,   182,     5,    65,    60,    76,   133,   181,
      50,   143,   143,   142,    64,    54,    55,    63,    44,    45,
      56,    77,    65,    58,    65,    76,    76,     5,    26,    36,
      76,    76,    76,    60,   171,   171,    65,    76,   171,   171,
     147,    76,   148,    76,   114,   116,   219,     5,   129,   139,
     129,   139,   145,   172,     5,    60,   177,    60,   177,    60,
     174,     6,    76,   127,   175,     5,    76,   157,     5,     5,
     172,    60,     5,    59,     5,    18,    22,    17,   189,   142,
     210,   211,   212,   213,    41,   200,   219,   219,   139,    76,
      30,     5,    30,    41,    30,    65,    50,    65,   199,    65,
     199,   219,    65,   199,   129,    65,    53,   172,   172,   172,
       5,     5,   171,   171,   184,   184,    15,   110,   172,   172,
      60,   177,    60,   177,    60,   179,   175,    28,    65,   142,
     143,   143,   144,   144,   144,   144,   125,   134,   171,   171,
      76,    76,   171,   171,    62,   136,   129,   172,   172,   145,
     171,   172,   172,    28,    60,   129,    60,    58,     5,    65,
      58,    63,    30,     5,   176,   175,   175,    60,   175,    28,
      59,    53,     5,   103,    60,   133,    76,     9,   176,   219,
     219,   187,    66,     5,    16,    41,   209,    58,    60,   187,
      66,    60,    17,    60,   193,   184,    65,    65,    65,     5,
       5,   129,     5,    60,    60,   176,   176,   172,   172,   184,
      30,     5,    60,   175,   175,    60,   175,    76,    65,    30,
     172,   172,   171,   171,   172,   172,   173,    60,     5,    60,
      30,   172,   150,    76,   114,   114,   115,    65,    76,   139,
      28,   176,   164,    53,   133,     8,    76,    76,   219,     5,
     133,     5,     5,   158,    17,    17,   142,   211,   184,   184,
      30,   213,   187,   219,   219,   187,    76,    30,     5,   176,
     133,   133,   166,   167,     5,    60,    59,   176,   133,   133,
       5,    60,   172,   172,     5,     5,    63,   125,   133,    60,
     152,   153,   212,    28,    58,    65,    60,    63,   130,   162,
     103,   219,     5,   176,   103,   187,   187,    63,    17,   168,
       5,     5,   103,   103,   176,   133,     5,   133,     5,    60,
     137,   133,     5,    60,   150,   114,   114,    28,   131,   103,
       5,   176,   165,     5,   187,   103,   176,   176,     5,     5,
     160,     5,   176,     5,   133,     5,    76,   153,    64,   152,
     129,   129,     5,   163,   103,     5,   170,   169,   103,   176,
       5,    72,    76,   149,   103,     5,   103,   103,     5,   161,
     145,     5,     5,     5,   103,    65,     5
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
     147,   147,   147,   149,   150,   151,   151,   151,   152,   152,
     153,   153,   154,   154,   155,   155,   155,   156,   156,   157,
     156,   158,   156,   159,   156,   160,   156,   161,   156,   162,
     156,   163,   156,   164,   156,   165,   156,   166,   156,   167,
     156,   168,   156,   169,   156,   170,   156,   171,   172,   172,
     172,   173,   173,   174,   174,   174,   174,   174,   174,   174,
     174,   175,   175,   175,   175,   175,   176,   176,   177,   177,
     177,   178,   178,   179,   179,   179,   179,   179,   179,   180,
     180,   181,   182,   182,   183,   185,   184,   186,   184,   188,
     187,   189,   189,   189,   190,   190,   190,   190,   190,   190,
     190,   190,   190,   190,   190,   190,   190,   190,   190,   191,
     191,   191,   191,   192,   192,   192,   193,   193,   194,   194,
     195,   195,   195,   196,   196,   196,   196,   197,   197,   197,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   198,   199,   199,   200,   200,   200,   200,   201,   201,
     201,   201,   201,   201,   202,   202,   202,   202,   202,   202,
     202,   202,   202,   203,   203,   204,   205,   205,   206,   206,
     208,   207,   209,   209,   209,   210,   210,   211,   211,   212,
     212,   213,   213,   214,   215,   216,   216,   216,   217,   218,
     218,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     220,   220,   220,   220,   220,   220,   220,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   224,   224,   225,   225
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
       4,     7,     0,     0,     0,     7,     5,     0,     3,     1,
       6,     0,     2,     1,     4,     6,     6,     7,     9,     0,
       9,     0,    11,     0,     7,     0,    13,     0,    15,     0,
      12,     0,    14,     0,    11,     0,    13,     0,    11,     0,
      11,     0,    12,     0,    14,     0,    14,     0,     3,     2,
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
#line 2274 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2280 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2286 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2292 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2298 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2304 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2310 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2316 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2322 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2328 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2334 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2340 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2346 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2352 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2358 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2364 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2370 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2376 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2382 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2388 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2394 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2400 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2406 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2412 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2418 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2424 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2430 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2436 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2442 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2448 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2454 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2460 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2466 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2472 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2478 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2484 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2490 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2496 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2502 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2508 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2514 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2520 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2526 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2532 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2538 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2544 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2550 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2556 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2562 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2568 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2574 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2580 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2586 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2592 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2598 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2604 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2610 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2616 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2622 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2628 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2634 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2640 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2646 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2652 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2658 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2664 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2670 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2676 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2682 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2688 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2694 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2700 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2706 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2712 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2718 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2724 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2730 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2736 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2742 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2748 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2754 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2760 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2766 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2772 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2778 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2784 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2790 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2796 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2802 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2808 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2814 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2820 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2826 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2832 "pascal.tab.c"
    break;

  case 96: /* type: OBJECTSY record_body ENDSY  */
#line 375 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2838 "pascal.tab.c"
    break;

  case 97: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 376 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2844 "pascal.tab.c"
    break;

  case 98: /* type: CLASSSY class_members ENDSY  */
#line 377 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2850 "pascal.tab.c"
    break;

  case 99: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 378 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2856 "pascal.tab.c"
    break;

  case 100: /* result_type: IDENT  */
#line 381 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2862 "pascal.tab.c"
    break;

  case 101: /* result_type: IDENT PERIOD IDENT  */
#line 382 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2868 "pascal.tab.c"
    break;

  case 102: /* result_type: spec_ref  */
#line 383 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2874 "pascal.tab.c"
    break;

  case 103: /* class_members: %empty  */
#line 386 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2880 "pascal.tab.c"
    break;

  case 104: /* class_members: class_members class_member  */
#line 387 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2886 "pascal.tab.c"
    break;

  case 105: /* class_member: IDENT  */
#line 390 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2892 "pascal.tab.c"
    break;

  case 106: /* class_member: IDENT SEMICOLON  */
#line 391 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2898 "pascal.tab.c"
    break;

  case 107: /* class_member: id_list COLON type SEMICOLON  */
#line 392 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2904 "pascal.tab.c"
    break;

  case 108: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2910 "pascal.tab.c"
    break;

  case 109: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 394 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2916 "pascal.tab.c"
    break;

  case 110: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2922 "pascal.tab.c"
    break;

  case 111: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 396 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2928 "pascal.tab.c"
    break;

  case 112: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 397 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2934 "pascal.tab.c"
    break;

  case 113: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 398 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2940 "pascal.tab.c"
    break;

  case 114: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 399 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2946 "pascal.tab.c"
    break;

  case 115: /* prop_index_opt: %empty  */
#line 402 "pascal.y"
    { (yyval.node) = NULL; }
#line 2952 "pascal.tab.c"
    break;

  case 116: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 403 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2958 "pascal.tab.c"
    break;

  case 117: /* prop_specs: %empty  */
#line 406 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2964 "pascal.tab.c"
    break;

  case 118: /* prop_specs: prop_specs IDENT IDENT  */
#line 407 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2970 "pascal.tab.c"
    break;

  case 119: /* prop_specs: prop_specs IDENT INTCONST  */
#line 408 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2976 "pascal.tab.c"
    break;

  case 120: /* packed_opt: PACKEDSY  */
#line 411 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2982 "pascal.tab.c"
    break;

  case 121: /* packed_opt: %empty  */
#line 412 "pascal.y"
     { (yyval.node) = NULL; }
#line 2988 "pascal.tab.c"
    break;

  case 122: /* simple_type: LPARENT enum_list RPARENT  */
#line 415 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2994 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT  */
#line 416 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3000 "pascal.tab.c"
    break;

  case 124: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 417 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3006 "pascal.tab.c"
    break;

  case 125: /* simple_type: IDENT PERIOD IDENT  */
#line 418 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3012 "pascal.tab.c"
    break;

  case 126: /* simple_type: spec_ref  */
#line 419 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3018 "pascal.tab.c"
    break;

  case 127: /* simple_type: lo_bound DOTDOT b_expr  */
#line 420 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3024 "pascal.tab.c"
    break;

  case 128: /* lo_bound: lo_term  */
#line 423 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3030 "pascal.tab.c"
    break;

  case 129: /* lo_bound: PLUS lo_term  */
#line 424 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3036 "pascal.tab.c"
    break;

  case 130: /* lo_bound: MINUS lo_term  */
#line 425 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3042 "pascal.tab.c"
    break;

  case 131: /* lo_bound: lo_bound PLUS b_term  */
#line 426 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3048 "pascal.tab.c"
    break;

  case 132: /* lo_bound: lo_bound MINUS b_term  */
#line 427 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3054 "pascal.tab.c"
    break;

  case 133: /* lo_term: scalar_constant  */
#line 430 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3060 "pascal.tab.c"
    break;

  case 134: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 431 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3066 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term MUL b_factor  */
#line 432 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3072 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term IDIV b_factor  */
#line 433 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3078 "pascal.tab.c"
    break;

  case 137: /* lo_term: lo_term IMOD b_factor  */
#line 434 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3084 "pascal.tab.c"
    break;

  case 138: /* lo_term: lo_term SHIFTOP b_factor  */
#line 435 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3090 "pascal.tab.c"
    break;

  case 139: /* b_expr: b_term  */
#line 438 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3096 "pascal.tab.c"
    break;

  case 140: /* b_expr: PLUS b_term  */
#line 439 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3102 "pascal.tab.c"
    break;

  case 141: /* b_expr: MINUS b_term  */
#line 440 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3108 "pascal.tab.c"
    break;

  case 142: /* b_expr: b_expr PLUS b_term  */
#line 441 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3114 "pascal.tab.c"
    break;

  case 143: /* b_expr: b_expr MINUS b_term  */
#line 442 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3120 "pascal.tab.c"
    break;

  case 144: /* b_term: b_factor  */
#line 445 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3126 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term MUL b_factor  */
#line 446 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3132 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term IDIV b_factor  */
#line 447 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3138 "pascal.tab.c"
    break;

  case 147: /* b_term: b_term IMOD b_factor  */
#line 448 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3144 "pascal.tab.c"
    break;

  case 148: /* b_term: b_term SHIFTOP b_factor  */
#line 449 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3150 "pascal.tab.c"
    break;

  case 149: /* b_factor: scalar_constant  */
#line 452 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3156 "pascal.tab.c"
    break;

  case 150: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 453 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3162 "pascal.tab.c"
    break;

  case 151: /* b_factor: LPARENT b_expr RPARENT  */
#line 454 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3168 "pascal.tab.c"
    break;

  case 152: /* record_body: record_field_list record_case_opt  */
#line 457 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3174 "pascal.tab.c"
    break;

  case 153: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 460 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3180 "pascal.tab.c"
    break;

  case 154: /* record_field_list: record_field  */
#line 461 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3186 "pascal.tab.c"
    break;

  case 155: /* $@4: %empty  */
#line 464 "pascal.y"
                  { }
#line 3192 "pascal.tab.c"
    break;

  case 156: /* record_field: id_list COLON $@4 type  */
#line 464 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3198 "pascal.tab.c"
    break;

  case 157: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 465 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3204 "pascal.tab.c"
    break;

  case 158: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 466 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3210 "pascal.tab.c"
    break;

  case 159: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 467 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3216 "pascal.tab.c"
    break;

  case 160: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 468 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3222 "pascal.tab.c"
    break;

  case 161: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 469 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3228 "pascal.tab.c"
    break;

  case 162: /* record_field: %empty  */
#line 470 "pascal.y"
     { (yyval.node) = NULL; }
#line 3234 "pascal.tab.c"
    break;

  case 163: /* case_arm_mark: %empty  */
#line 473 "pascal.y"
    { }
#line 3240 "pascal.tab.c"
    break;

  case 164: /* case_open: %empty  */
#line 476 "pascal.y"
    { }
#line 3246 "pascal.tab.c"
    break;

  case 165: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 479 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3252 "pascal.tab.c"
    break;

  case 166: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 480 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3258 "pascal.tab.c"
    break;

  case 167: /* record_case_opt: %empty  */
#line 481 "pascal.y"
     { (yyval.node) = NULL; }
#line 3264 "pascal.tab.c"
    break;

  case 168: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 484 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3270 "pascal.tab.c"
    break;

  case 169: /* record_case_list: record_case_arm  */
#line 485 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3276 "pascal.tab.c"
    break;

  case 170: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 488 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3282 "pascal.tab.c"
    break;

  case 171: /* record_case_arm: %empty  */
#line 489 "pascal.y"
     { (yyval.node) = NULL; }
#line 3288 "pascal.tab.c"
    break;

  case 172: /* var_decl_list: var_decl_list var_decl  */
#line 492 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3294 "pascal.tab.c"
    break;

  case 173: /* var_decl_list: var_decl  */
#line 493 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3300 "pascal.tab.c"
    break;

  case 174: /* var_decl: id_list COLON type SEMICOLON  */
#line 496 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3306 "pascal.tab.c"
    break;

  case 175: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 497 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3312 "pascal.tab.c"
    break;

  case 176: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 498 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3318 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 501 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3324 "pascal.tab.c"
    break;

  case 178: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 502 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3330 "pascal.tab.c"
    break;

  case 179: /* $@5: %empty  */
#line 503 "pascal.y"
                                                                               { }
#line 3336 "pascal.tab.c"
    break;

  case 180: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 503 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3342 "pascal.tab.c"
    break;

  case 181: /* $@6: %empty  */
#line 504 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3348 "pascal.tab.c"
    break;

  case 182: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 504 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3354 "pascal.tab.c"
    break;

  case 183: /* $@7: %empty  */
#line 505 "pascal.y"
                                        { }
#line 3360 "pascal.tab.c"
    break;

  case 184: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 505 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3366 "pascal.tab.c"
    break;

  case 185: /* $@8: %empty  */
#line 506 "pascal.y"
                                                                                                           { }
#line 3372 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 506 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3378 "pascal.tab.c"
    break;

  case 187: /* $@9: %empty  */
#line 507 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3384 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3390 "pascal.tab.c"
    break;

  case 189: /* $@10: %empty  */
#line 508 "pascal.y"
                                                                                                 { }
#line 3396 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 508 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3402 "pascal.tab.c"
    break;

  case 191: /* $@11: %empty  */
#line 509 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3408 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 509 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3414 "pascal.tab.c"
    break;

  case 193: /* $@12: %empty  */
#line 510 "pascal.y"
                                                                                            { }
#line 3420 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3426 "pascal.tab.c"
    break;

  case 195: /* $@13: %empty  */
#line 511 "pascal.y"
                                                                                                             { }
#line 3432 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 511 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3440 "pascal.tab.c"
    break;

  case 197: /* $@14: %empty  */
#line 514 "pascal.y"
                                                                                              { }
#line 3446 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3454 "pascal.tab.c"
    break;

  case 199: /* $@15: %empty  */
#line 517 "pascal.y"
                                                                                             { }
#line 3460 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3468 "pascal.tab.c"
    break;

  case 201: /* $@16: %empty  */
#line 520 "pascal.y"
                                                                                                    { }
#line 3474 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3482 "pascal.tab.c"
    break;

  case 203: /* $@17: %empty  */
#line 523 "pascal.y"
                                                                                                                { }
#line 3488 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 523 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3497 "pascal.tab.c"
    break;

  case 205: /* $@18: %empty  */
#line 527 "pascal.y"
                                                                                                                     { }
#line 3503 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 527 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3511 "pascal.tab.c"
    break;

  case 207: /* pv_mark: %empty  */
#line 532 "pascal.y"
    { }
#line 3517 "pascal.tab.c"
    break;

  case 208: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 535 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3523 "pascal.tab.c"
    break;

  case 209: /* parameter_list_opt: LPARENT RPARENT  */
#line 536 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3529 "pascal.tab.c"
    break;

  case 210: /* parameter_list_opt: %empty  */
#line 537 "pascal.y"
     { (yyval.node) = NULL; }
#line 3535 "pascal.tab.c"
    break;

  case 211: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 540 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3541 "pascal.tab.c"
    break;

  case 212: /* parameter_decl_list: parameter_decl  */
#line 541 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3547 "pascal.tab.c"
    break;

  case 213: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 544 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3553 "pascal.tab.c"
    break;

  case 214: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 545 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3559 "pascal.tab.c"
    break;

  case 215: /* parameter_decl: VARSY id_list COLON param_type  */
#line 546 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3565 "pascal.tab.c"
    break;

  case 216: /* parameter_decl: id_list COLON param_type  */
#line 547 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3571 "pascal.tab.c"
    break;

  case 217: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 548 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3577 "pascal.tab.c"
    break;

  case 218: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 549 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3583 "pascal.tab.c"
    break;

  case 219: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 550 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3589 "pascal.tab.c"
    break;

  case 220: /* parameter_decl: IDENT id_list COLON param_type  */
#line 551 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3595 "pascal.tab.c"
    break;

  case 221: /* param_type: IDENT  */
#line 554 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3601 "pascal.tab.c"
    break;

  case 222: /* param_type: IDENT PERIOD IDENT  */
#line 555 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3607 "pascal.tab.c"
    break;

  case 223: /* param_type: ARRAYSY OFSY IDENT  */
#line 556 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3613 "pascal.tab.c"
    break;

  case 224: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 557 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3619 "pascal.tab.c"
    break;

  case 225: /* param_type: spec_ref  */
#line 558 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3625 "pascal.tab.c"
    break;

  case 226: /* routine_directives: %empty  */
#line 561 "pascal.y"
    { (yyval.node) = NULL; }
#line 3631 "pascal.tab.c"
    break;

  case 227: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 562 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3644 "pascal.tab.c"
    break;

  case 228: /* pf_params: LPARENT pf_sections RPARENT  */
#line 572 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3650 "pascal.tab.c"
    break;

  case 229: /* pf_params: LPARENT RPARENT  */
#line 573 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3656 "pascal.tab.c"
    break;

  case 230: /* pf_params: %empty  */
#line 574 "pascal.y"
     { (yyval.node) = NULL; }
#line 3662 "pascal.tab.c"
    break;

  case 231: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 577 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3668 "pascal.tab.c"
    break;

  case 232: /* pf_sections: pf_section  */
#line 578 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3674 "pascal.tab.c"
    break;

  case 233: /* pf_section: id_list COLON param_type  */
#line 581 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3680 "pascal.tab.c"
    break;

  case 234: /* pf_section: VARSY id_list COLON param_type  */
#line 582 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3686 "pascal.tab.c"
    break;

  case 235: /* pf_section: CONSTSY id_list COLON param_type  */
#line 583 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3692 "pascal.tab.c"
    break;

  case 236: /* pf_section: IDENT id_list COLON param_type  */
#line 584 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3698 "pascal.tab.c"
    break;

  case 237: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 585 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3704 "pascal.tab.c"
    break;

  case 238: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 586 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3710 "pascal.tab.c"
    break;

  case 239: /* enum_list: enum_list COMMA enum_member  */
#line 589 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3716 "pascal.tab.c"
    break;

  case 240: /* enum_list: enum_member  */
#line 590 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3722 "pascal.tab.c"
    break;

  case 241: /* enum_member: IDENT  */
#line 593 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3728 "pascal.tab.c"
    break;

  case 242: /* id_list: id_list COMMA IDENT  */
#line 596 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3734 "pascal.tab.c"
    break;

  case 243: /* id_list: IDENT  */
#line 597 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3740 "pascal.tab.c"
    break;

  case 244: /* body: BEGINSY statement_list ENDSY  */
#line 600 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3746 "pascal.tab.c"
    break;

  case 245: /* $@19: %empty  */
#line 603 "pascal.y"
                             { pas_stmt_mark(); }
#line 3752 "pascal.tab.c"
    break;

  case 246: /* statement_list: statement_list SEMICOLON $@19 statement  */
#line 603 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3758 "pascal.tab.c"
    break;

  case 247: /* $@20: %empty  */
#line 604 "pascal.y"
     { pas_stmt_mark(); }
#line 3764 "pascal.tab.c"
    break;

  case 248: /* statement_list: $@20 statement  */
#line 604 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3770 "pascal.tab.c"
    break;

  case 249: /* $@21: %empty  */
#line 607 "pascal.y"
    { pas_stmt_mark(); }
#line 3776 "pascal.tab.c"
    break;

  case 250: /* body_stmt: $@21 statement  */
#line 607 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3782 "pascal.tab.c"
    break;

  case 251: /* statement: statement_no_label  */
#line 610 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3788 "pascal.tab.c"
    break;

  case 252: /* statement: INTCONST COLON statement_no_label  */
#line 611 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3794 "pascal.tab.c"
    break;

  case 253: /* statement: IDENT COLON statement_no_label  */
#line 612 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3800 "pascal.tab.c"
    break;

  case 254: /* statement_no_label: assignment  */
#line 615 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3806 "pascal.tab.c"
    break;

  case 255: /* statement_no_label: call  */
#line 616 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3812 "pascal.tab.c"
    break;

  case 256: /* statement_no_label: compound_statement  */
#line 617 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3818 "pascal.tab.c"
    break;

  case 257: /* statement_no_label: goto_statement  */
#line 618 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3824 "pascal.tab.c"
    break;

  case 258: /* statement_no_label: if_statement  */
#line 619 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3830 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: case_statement  */
#line 620 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3836 "pascal.tab.c"
    break;

  case 260: /* statement_no_label: while_statement  */
#line 621 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3842 "pascal.tab.c"
    break;

  case 261: /* statement_no_label: repeat_statement  */
#line 622 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3848 "pascal.tab.c"
    break;

  case 262: /* statement_no_label: for_statement  */
#line 623 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3854 "pascal.tab.c"
    break;

  case 263: /* statement_no_label: with_statement  */
#line 624 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3860 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: try_statement  */
#line 625 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3866 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: raise_statement  */
#line 626 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3872 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: inherited_call  */
#line 627 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3878 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: INHERITEDSY  */
#line 628 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3884 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: %empty  */
#line 629 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3890 "pascal.tab.c"
    break;

  case 269: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 632 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3896 "pascal.tab.c"
    break;

  case 270: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 633 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3902 "pascal.tab.c"
    break;

  case 271: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 634 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3908 "pascal.tab.c"
    break;

  case 272: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 635 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3914 "pascal.tab.c"
    break;

  case 273: /* handler_list: handler  */
#line 638 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3920 "pascal.tab.c"
    break;

  case 274: /* handler_list: handler_list SEMICOLON handler  */
#line 639 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3926 "pascal.tab.c"
    break;

  case 275: /* handler_list: handler_list SEMICOLON  */
#line 640 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3932 "pascal.tab.c"
    break;

  case 276: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 643 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3938 "pascal.tab.c"
    break;

  case 277: /* handler: ONSY IDENT DOSY body_stmt  */
#line 644 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3944 "pascal.tab.c"
    break;

  case 278: /* raise_statement: RAISESY  */
#line 647 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3950 "pascal.tab.c"
    break;

  case 279: /* raise_statement: RAISESY expression  */
#line 648 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3956 "pascal.tab.c"
    break;

  case 280: /* inherited_call: INHERITEDSY IDENT  */
#line 651 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3962 "pascal.tab.c"
    break;

  case 281: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 652 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3968 "pascal.tab.c"
    break;

  case 282: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 653 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3974 "pascal.tab.c"
    break;

  case 283: /* call: IDENT  */
#line 656 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3980 "pascal.tab.c"
    break;

  case 284: /* call: spec_expr  */
#line 657 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3986 "pascal.tab.c"
    break;

  case 285: /* call: selector PERIOD IDENT  */
#line 658 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3992 "pascal.tab.c"
    break;

  case 286: /* call: call_with_args  */
#line 659 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3998 "pascal.tab.c"
    break;

  case 287: /* recv_primary: STRINGCONST  */
#line 662 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4004 "pascal.tab.c"
    break;

  case 288: /* recv_primary: LPARENT expression RPARENT  */
#line 663 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4010 "pascal.tab.c"
    break;

  case 289: /* recv_primary: spec_expr  */
#line 664 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4016 "pascal.tab.c"
    break;

  case 290: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 667 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4022 "pascal.tab.c"
    break;

  case 291: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 668 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4028 "pascal.tab.c"
    break;

  case 292: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 669 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4034 "pascal.tab.c"
    break;

  case 293: /* call_with_args: IDENT LPARENT RPARENT  */
#line 670 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4040 "pascal.tab.c"
    break;

  case 294: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 671 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4046 "pascal.tab.c"
    break;

  case 295: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 672 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4052 "pascal.tab.c"
    break;

  case 296: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 673 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4058 "pascal.tab.c"
    break;

  case 297: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 674 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4064 "pascal.tab.c"
    break;

  case 298: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 675 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4070 "pascal.tab.c"
    break;

  case 299: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 676 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4076 "pascal.tab.c"
    break;

  case 300: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 677 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4082 "pascal.tab.c"
    break;

  case 301: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 678 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4088 "pascal.tab.c"
    break;

  case 302: /* argument_list: argument_list COMMA argument  */
#line 681 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4094 "pascal.tab.c"
    break;

  case 303: /* argument_list: argument  */
#line 682 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4100 "pascal.tab.c"
    break;

  case 304: /* argument: expression  */
#line 685 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4106 "pascal.tab.c"
    break;

  case 305: /* argument: expression COLON expression  */
#line 686 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4112 "pascal.tab.c"
    break;

  case 306: /* argument: expression COLON expression COLON expression  */
#line 687 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4118 "pascal.tab.c"
    break;

  case 307: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 688 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4124 "pascal.tab.c"
    break;

  case 308: /* assignment: selector BECOMES expression  */
#line 691 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4130 "pascal.tab.c"
    break;

  case 309: /* assignment: selector ADDBECOMES expression  */
#line 692 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4136 "pascal.tab.c"
    break;

  case 310: /* assignment: selector SUBBECOMES expression  */
#line 693 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4142 "pascal.tab.c"
    break;

  case 311: /* assignment: selector MULBECOMES expression  */
#line 694 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4148 "pascal.tab.c"
    break;

  case 312: /* assignment: selector DIVBECOMES expression  */
#line 695 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4154 "pascal.tab.c"
    break;

  case 313: /* assignment: call_with_args BECOMES expression  */
#line 696 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4160 "pascal.tab.c"
    break;

  case 314: /* selector: selector LBRACK expression_list RBRACK  */
#line 699 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4166 "pascal.tab.c"
    break;

  case 315: /* selector: selector PERIOD IDENT  */
#line 700 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4172 "pascal.tab.c"
    break;

  case 316: /* selector: selector ARROW  */
#line 701 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4178 "pascal.tab.c"
    break;

  case 317: /* selector: call_with_args PERIOD IDENT  */
#line 702 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4184 "pascal.tab.c"
    break;

  case 318: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 703 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4190 "pascal.tab.c"
    break;

  case 319: /* selector: call_with_args ARROW  */
#line 704 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4196 "pascal.tab.c"
    break;

  case 320: /* selector: recv_primary PERIOD IDENT  */
#line 705 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4202 "pascal.tab.c"
    break;

  case 321: /* selector: recv_primary ARROW  */
#line 706 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4208 "pascal.tab.c"
    break;

  case 322: /* selector: IDENT  */
#line 707 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4214 "pascal.tab.c"
    break;

  case 323: /* expression_list: expression_list COMMA expression  */
#line 710 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4220 "pascal.tab.c"
    break;

  case 324: /* expression_list: expression  */
#line 711 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4226 "pascal.tab.c"
    break;

  case 325: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 714 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4232 "pascal.tab.c"
    break;

  case 326: /* goto_statement: GOTOSY INTCONST  */
#line 717 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4238 "pascal.tab.c"
    break;

  case 327: /* goto_statement: GOTOSY IDENT  */
#line 718 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4244 "pascal.tab.c"
    break;

  case 328: /* if_statement: IFSY expression THENSY body_stmt  */
#line 721 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4250 "pascal.tab.c"
    break;

  case 329: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 722 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4256 "pascal.tab.c"
    break;

  case 330: /* $@22: %empty  */
#line 725 "pascal.y"
                           { }
#line 4262 "pascal.tab.c"
    break;

  case 331: /* case_statement: CASESY expression OFSY $@22 case_list case_tail ENDSY  */
#line 725 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4268 "pascal.tab.c"
    break;

  case 332: /* case_tail: %empty  */
#line 728 "pascal.y"
    { (yyval.node) = NULL; }
#line 4274 "pascal.tab.c"
    break;

  case 333: /* case_tail: ELSESY statement_list  */
#line 729 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4280 "pascal.tab.c"
    break;

  case 334: /* case_tail: OTHERWISESY statement_list  */
#line 730 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4286 "pascal.tab.c"
    break;

  case 335: /* case_list: case_list SEMICOLON case_elem  */
#line 733 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4292 "pascal.tab.c"
    break;

  case 336: /* case_list: case_elem  */
#line 734 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4298 "pascal.tab.c"
    break;

  case 337: /* case_elem: constant_list COLON body_stmt  */
#line 737 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4304 "pascal.tab.c"
    break;

  case 338: /* case_elem: %empty  */
#line 738 "pascal.y"
     { (yyval.node) = NULL; }
#line 4310 "pascal.tab.c"
    break;

  case 339: /* constant_list: constant_list COMMA case_label  */
#line 741 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4316 "pascal.tab.c"
    break;

  case 340: /* constant_list: case_label  */
#line 742 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4322 "pascal.tab.c"
    break;

  case 341: /* case_label: b_expr  */
#line 745 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4328 "pascal.tab.c"
    break;

  case 342: /* case_label: b_expr DOTDOT b_expr  */
#line 746 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4334 "pascal.tab.c"
    break;

  case 343: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 749 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4340 "pascal.tab.c"
    break;

  case 344: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 752 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4346 "pascal.tab.c"
    break;

  case 345: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 755 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4352 "pascal.tab.c"
    break;

  case 346: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 756 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4358 "pascal.tab.c"
    break;

  case 347: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 757 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4364 "pascal.tab.c"
    break;

  case 348: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 760 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4370 "pascal.tab.c"
    break;

  case 349: /* with_open: with_open COMMA selector  */
#line 763 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4376 "pascal.tab.c"
    break;

  case 350: /* with_open: selector  */
#line 764 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4382 "pascal.tab.c"
    break;

  case 351: /* expression: simple_expression  */
#line 771 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4388 "pascal.tab.c"
    break;

  case 352: /* expression: expression INOP simple_expression  */
#line 772 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4394 "pascal.tab.c"
    break;

  case 353: /* expression: expression LTOP simple_expression  */
#line 773 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4400 "pascal.tab.c"
    break;

  case 354: /* expression: expression LEOP simple_expression  */
#line 774 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4406 "pascal.tab.c"
    break;

  case 355: /* expression: expression GTOP simple_expression  */
#line 775 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4412 "pascal.tab.c"
    break;

  case 356: /* expression: expression GEOP simple_expression  */
#line 776 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4418 "pascal.tab.c"
    break;

  case 357: /* expression: expression NEOP simple_expression  */
#line 777 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4424 "pascal.tab.c"
    break;

  case 358: /* expression: expression EQOP simple_expression  */
#line 778 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4430 "pascal.tab.c"
    break;

  case 359: /* expression: expression ISOP simple_expression  */
#line 779 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4436 "pascal.tab.c"
    break;

  case 360: /* simple_expression: term  */
#line 782 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4442 "pascal.tab.c"
    break;

  case 361: /* simple_expression: PLUS term  */
#line 783 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4448 "pascal.tab.c"
    break;

  case 362: /* simple_expression: MINUS term  */
#line 784 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4454 "pascal.tab.c"
    break;

  case 363: /* simple_expression: simple_expression PLUS term  */
#line 785 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4460 "pascal.tab.c"
    break;

  case 364: /* simple_expression: simple_expression MINUS term  */
#line 786 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4466 "pascal.tab.c"
    break;

  case 365: /* simple_expression: simple_expression OROP term  */
#line 787 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4472 "pascal.tab.c"
    break;

  case 366: /* simple_expression: simple_expression XOROP term  */
#line 788 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4478 "pascal.tab.c"
    break;

  case 367: /* term: power  */
#line 791 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4484 "pascal.tab.c"
    break;

  case 368: /* term: term MUL power  */
#line 792 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4490 "pascal.tab.c"
    break;

  case 369: /* term: term RDIV power  */
#line 793 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4496 "pascal.tab.c"
    break;

  case 370: /* term: term IDIV power  */
#line 794 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4502 "pascal.tab.c"
    break;

  case 371: /* term: term IMOD power  */
#line 795 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4508 "pascal.tab.c"
    break;

  case 372: /* term: term ANDOP power  */
#line 796 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4514 "pascal.tab.c"
    break;

  case 373: /* term: term MUL MINUS power  */
#line 797 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4520 "pascal.tab.c"
    break;

  case 374: /* term: term RDIV MINUS power  */
#line 798 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4526 "pascal.tab.c"
    break;

  case 375: /* term: term IDIV MINUS power  */
#line 799 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4532 "pascal.tab.c"
    break;

  case 376: /* term: term IMOD MINUS power  */
#line 800 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4538 "pascal.tab.c"
    break;

  case 377: /* term: term ANDOP MINUS power  */
#line 801 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4544 "pascal.tab.c"
    break;

  case 378: /* term: term SHIFTOP power  */
#line 802 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4550 "pascal.tab.c"
    break;

  case 379: /* term: term ASOP power  */
#line 803 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4556 "pascal.tab.c"
    break;

  case 380: /* power: factor  */
#line 806 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4562 "pascal.tab.c"
    break;

  case 381: /* power: power POWOP factor  */
#line 807 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4568 "pascal.tab.c"
    break;

  case 382: /* factor: selector  */
#line 810 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4574 "pascal.tab.c"
    break;

  case 383: /* factor: call_with_args  */
#line 811 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4580 "pascal.tab.c"
    break;

  case 384: /* factor: INTCONST  */
#line 812 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4586 "pascal.tab.c"
    break;

  case 385: /* factor: REALCONST  */
#line 813 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4592 "pascal.tab.c"
    break;

  case 386: /* factor: STRINGCONST  */
#line 814 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4598 "pascal.tab.c"
    break;

  case 387: /* factor: CHARCODE  */
#line 815 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4604 "pascal.tab.c"
    break;

  case 388: /* factor: LPARENT expression RPARENT  */
#line 816 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4610 "pascal.tab.c"
    break;

  case 389: /* factor: inherited_call  */
#line 817 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4616 "pascal.tab.c"
    break;

  case 390: /* factor: NOTSY factor  */
#line 818 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4622 "pascal.tab.c"
    break;

  case 391: /* factor: ATSIGN factor  */
#line 819 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4628 "pascal.tab.c"
    break;

  case 392: /* factor: LBRACK RBRACK  */
#line 820 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4634 "pascal.tab.c"
    break;

  case 393: /* factor: LBRACK set_member_list RBRACK  */
#line 821 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4640 "pascal.tab.c"
    break;

  case 394: /* set_member_list: set_member  */
#line 824 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4646 "pascal.tab.c"
    break;

  case 395: /* set_member_list: set_member_list COMMA set_member  */
#line 825 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4652 "pascal.tab.c"
    break;

  case 396: /* set_member: expression  */
#line 828 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4658 "pascal.tab.c"
    break;

  case 397: /* set_member: expression DOTDOT expression  */
#line 829 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4664 "pascal.tab.c"
    break;


#line 4668 "pascal.tab.c"

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

#line 835 "pascal.y"

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
