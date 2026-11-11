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
#define YYLAST   1880

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

#define YYPACT_NINF (-561)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-336)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     400,     6,   103,   152,  -561,   117,   250,  -561,  -561,   439,
    -561,    54,  -561,  -561,   465,   139,   103,   190,   198,   198,
     218,  -561,   176,   235,   326,   348,   310,  -561,  -561,  -561,
     385,   198,   480,   370,   412,  -561,  -561,   100,   463,   139,
    -561,   126,   469,   194,  -561,   198,  -561,   298,   127,   270,
     263,  1171,   644,   478,   176,  -561,   498,   509,   535,   558,
     575,   551,   584,  -561,   493,  -561,  -561,  -561,  -561,   510,
    1466,  1177,  -561,  -561,   103,   198,   585,   475,  -561,   595,
    1177,  -561,   198,   596,    46,  -561,  -561,   547,   603,  -561,
    1466,   290,  1466,  -561,  1466,  1466,   580,  -561,  1003,  -561,
    1466,   606,   614,  -561,   625,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,   414,   506,  -561,  1159,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   198,  1242,   643,  -561,   639,   640,
     661,   656,   660,   692,   695,  -561,   690,   987,  -561,  -561,
    1213,  1213,  1213,  1266,  1466,  1213,  -561,  -561,  -561,   494,
       4,   606,   686,  -561,   706,   364,    85,   225,   618,   675,
    -561,   645,   697,  -561,   697,   858,   858,   112,   698,  -561,
    -561,  -561,  -561,  1551,    37,   308,  -561,  -561,  -561,  -561,
     728,   488,  -561,   339,   306,   469,    36,  -561,   191,   772,
    -561,    35,   329,  -561,  -561,   732,  1171,  -561,  -561,   612,
     300,   325,   706,   364,   105,   895,   291,  1705,  1104,   125,
     125,  1291,   721,    32,  1799,   739,   765,  1316,   744,  -561,
    1466,   745,  -561,  1466,  1341,  1466,   746,  -561,  1466,  1466,
    1466,  1466,  1466,   395,    79,    76,   826,   198,   756,   766,
      26,  -561,  -561,   198,   198,  -561,   139,   103,  -561,   769,
     198,   773,   176,  -561,  -561,   618,   618,  -561,  1038,   615,
    -561,  1717,  -561,   774,  -561,  1466,  1466,  1466,  1466,  1466,
    1466,  1466,  1466,  1213,  1213,  1213,  1213,  1557,  1582,  1607,
    1632,  1657,  1213,  1213,  1213,   779,   305,  -561,   223,   827,
     798,   795,   306,   306,     2,  -561,  -561,   511,  -561,   721,
     792,  1219,  1219,   793,   305,   319,   799,   800,   801,   568,
     806,   807,   809,   834,    68,  -561,   648,  1491,   216,   843,
     846,  -561,   762,   762,  1219,   762,   762,   762,   762,  -561,
     475,   198,   810,   198,   811,  -561,   198,    55,  -561,   688,
    -561,  -561,  1491,   813,  -561,   475,  -561,   320,  -561,  1466,
    1466,  1466,  -561,  -561,   290,  -561,  -561,  -561,  -561,   856,
    -561,  -561,  -561,   513,  -561,   388,  1739,   111,  -561,   803,
    -561,  1366,   721,  -561,   522,   836,  1799,   837,   665,  1799,
    -561,   543,  1799,  1680,   671,  1799,  1799,  1799,  1799,   838,
    -561,  -561,   305,  -561,   429,  -561,  -561,  -561,  -561,   475,
     475,   452,   471,   139,   178,   211,  -561,   198,  -561,   176,
    1466,  1466,  -561,   699,   839,   225,   225,   225,   225,   225,
     225,   225,   225,   618,   618,   618,   618,  1213,   675,  1213,
     675,  1213,   675,  1213,   675,  1213,   675,   675,   675,  -561,
     842,     2,   545,  -561,  -561,   327,   198,   830,   198,   833,
    -561,   198,    64,  -561,   713,   835,   320,   698,  -561,   477,
    -561,   762,   762,  1219,   850,  -561,   442,   345,  -561,   402,
    -561,   549,   845,  -561,   852,   101,   448,   859,   860,   862,
    -561,   724,  -561,  -561,   854,  -561,   134,  -561,  -561,  -561,
     579,   863,  -561,  -561,  1516,   897,  1799,  1177,   721,  1177,
     721,   579,   345,   345,   255,  -561,  -561,  -561,  -561,   475,
     905,   740,   697,   741,   697,   754,    63,  -561,    20,    47,
     935,   937,   475,   890,   946,   108,  -561,   949,   996,   663,
    1799,  -561,  1171,  -561,   364,  1219,   916,  1466,  -561,  1466,
    1466,   721,  -561,   883,   301,   234,  -561,   302,  -561,   555,
     490,  -561,  1391,  1416,  1466,  -561,  -561,  1441,  -561,  1177,
     564,   909,   475,   475,   475,   475,   963,   968,  -561,  -561,
    -561,  -561,  -561,   959,   475,   475,  1799,  -561,   675,   675,
     675,   675,   675,  -561,   305,   917,  -561,   767,   697,   770,
     697,   783,   237,  -561,    20,  -561,   952,  -561,  -561,   345,
     345,   411,  1219,   762,   762,  -561,   762,   762,   762,   762,
    -561,  -561,  -561,  -561,  -561,   907,   914,  -561,  -561,   931,
    1177,   475,   475,   579,  -561,  -561,   475,   475,  -561,   205,
    1177,   142,   938,    65,  1717,  -561,  -561,   684,  -561,  -561,
     971,   999,  -561,    20,  -561,    20,   945,    20,  -561,   979,
     155,  -561,   955,  1004,  1005,  -561,  -561,  -561,   956,   320,
    -561,   939,  -561,  1466,  1466,  -561,  -561,   377,   279,  -561,
     788,  -561,  -561,  -561,  1752,  1764,  -561,    49,  -561,   803,
    -561,  -561,  -561,  -561,  -561,  -561,   567,  -561,   573,  1799,
    -561,   577,  1007,  1016,  1177,  1017,   964,   967,    87,  -561,
    -561,   475,   475,  1025,  1025,  -561,  1006,  1030,   977,  -561,
    -561,    20,  -561,    20,   983,    20,  -561,  -561,   982,  -561,
     415,   345,   345,  -561,  -561,  -561,  -561,   456,   475,   475,
    -561,  -561,   475,   475,    63,   995,  1054,  -561,  1000,  1031,
     475,   475,  -561,  -561,  -561,   994,  -561,  1491,  1491,   315,
    -561,   721,  1047,  -561,  -561,  1002,  1026,  -561,   320,  -561,
      45,  1018,  1466,  -561,  -561,  1076,   320,  1077,  -561,    48,
     738,   936,  -561,  1219,  1219,  -561,  -561,  1053,  1219,  -561,
    -561,  1466,  1466,  -561,  1020,  -561,   307,  -561,  -561,  -561,
    -561,  -561,  1088,  -561,   320,   320,  -561,   320,  1002,  1002,
    1092,  1040,  1025,  1039,  -561,   320,   576,  -561,  -561,   320,
    -561,  -561,  -561,  -561,  1096,  1042,   475,   475,  1100,  1101,
      53,   305,  -561,   320,  -561,  1050,  1051,  1219,  1079,  -561,
    -561,   581,  -561,  1052,  1057,  -561,  1002,  -561,  1466,  -561,
    -561,  -561,  -561,  1799,  -561,  1108,  -561,  1109,  -561,  -561,
    -561,   255,  -561,  1025,  1025,  -561,  -561,  -561,  1777,  1799,
    -561,  1105,  -561,  -561,  1002,  1116,  1118,  1002,  1120,  -561,
    -561,  -561,   320,  -561,  1002,  1121,  -561,  -561,  -561,   320,
    1122,  1068,  -561,  -561,  -561,  -561,  -561,   320,   320,  1124,
    -561,   805,  -561,  1491,  -561,  1491,  1103,  -561,  -561,  1129,
    1799,  -561,  1002,  -561,  1136,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  1137,  1138,  1002,  1139,  -561,  1146,
    -561,   320,    43,  -561,  -561,  1219,  1094,  1219,  -561,  -561,
    1177,  1177,  1154,  -561,  1002,  -561,  -561,  -561,  1156,  1002,
    1002,  1157,  1002,  -561,  -561,  -561,  -561,  1002,  -561,  1158,
    -561,   650,  -561,  -561,  1124,  -561,  -561,  -561,  -561,  1160,
    -561,  -561,  -561,  -561,  -561,  1163,  1002,  -561,  -561,  -561,
     579,  1166,  -561,  1167,  1168,  1170,  -561,  -561,  1099,  -561,
    -561,  -561,  -561,  1172,  -561,  -561
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
       0,     0,   243,     0,   243,     0,     0,   221,     0,   188,
       0,     0,   223,     0,     0,   104,   106,     0,     0,     0,
     357,   356,   281,   361,   362,   351,   341,     0,   303,     0,
       0,     0,    67,     0,     0,     0,   286,     0,   294,     0,
       0,   304,     0,     0,     0,   331,   307,     0,   327,   127,
       0,     0,   223,   223,   223,   223,     0,     0,   220,   220,
      23,   260,   260,    33,   223,   223,   410,   408,   388,   389,
     390,   386,   387,   108,     0,   107,   102,     0,   243,     0,
     243,     0,     0,   241,     0,    89,    90,   252,    77,   146,
     147,     0,     0,     0,     0,   130,     0,     0,     0,     0,
     140,   109,   220,   220,   112,     0,     0,   220,   220,   121,
     127,   223,   223,   171,   220,   220,   223,   223,   159,     0,
     127,   335,     0,     0,    42,    41,    87,     0,    97,    95,
       0,     0,   239,     0,   226,     0,     0,     0,   224,     0,
     234,   238,   229,     0,     0,    12,   184,   185,     0,     0,
     193,     0,   239,     0,     0,   262,   263,   354,   345,   349,
       0,   353,   262,   315,   337,   318,    79,     0,   282,   288,
     283,   260,   285,   295,    74,   314,     0,   312,     0,   336,
     310,     0,     0,   109,   127,     0,     0,     0,     0,   239,
     239,   223,   223,    29,    30,   260,     0,     0,     0,    71,
     109,     0,   250,     0,     0,     0,   244,   246,     0,   157,
       0,   148,   149,   152,   153,   151,   154,     0,   223,   223,
     220,   220,   223,   223,     0,     0,     0,   163,     0,     0,
     223,   223,   165,   166,   173,     0,   162,     0,     0,     0,
      44,     0,     0,    94,   239,   202,   230,   228,     0,   233,
       0,     0,     0,   186,   240,     0,     0,     0,   105,   190,
       0,     0,   360,     0,   351,   260,   260,     0,     0,   262,
     342,     0,     0,   262,     0,   287,     0,   313,   311,   309,
      59,    62,     0,   239,     0,     0,   239,     0,   206,   208,
       0,     0,    32,     0,   239,     0,     0,   248,   247,     0,
     249,    91,   156,   101,     0,     0,   223,   223,     0,     0,
       0,     0,   113,     0,    99,     0,   170,   180,     0,    48,
      46,     0,    45,     0,     0,    84,   198,    12,     0,   227,
     237,   236,   235,   231,   189,     0,   239,     0,    12,   262,
     262,   355,   348,   347,   346,   344,   352,   350,     0,   319,
     290,     0,   284,    58,   210,     0,     0,   216,     0,    12,
      12,   239,     0,    20,    27,     0,   103,   251,   114,     0,
       0,     0,   116,   117,   122,   123,   164,     0,     0,   175,
     178,     0,   173,     0,    43,     0,     0,    85,    12,     0,
     232,   239,   204,   187,     0,   359,   358,   320,   262,    12,
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
    -561,  -561,  -561,  -561,  -561,   -65,   928,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   932,   -34,  -334,  -561,  -561,   -58,
     927,   -48,   933,    40,   -70,   -28,  -266,  -559,   -64,   245,
    -260,   -78,  -561,  -561,  -561,   197,   598,  -273,  -561,  -561,
    -561,  -561,  -208,  -561,   341,  -283,  -287,  -282,  -498,  -561,
    -279,  -561,  -561,   292,  -561,   261,   273,   951,   -36,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,   569,   407,   468,   683,
    -560,   299,  -144,  -561,   613,  -561,   750,   433,  -561,   -72,
    -561,  -561,  -324,  -561,  -185,   354,  -561,  -561,   536,  -561,
     -23,  -561,  -561,   -39,  -193,   677,  -561,   -19,   993,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,   445,  -520,   444,  -561,
    -561,  -561,  -561,  -561,   368,  1506,   -79,  -203,   -91,  -561,
     814
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   573,   706,    37,    39,    40,   495,   831,   633,   465,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   897,   931,   501,   527,   287,   305,   480,   735,
     922,   181,   182,   183,   184,   667,   467,   468,   313,   314,
     315,   630,   970,   827,   492,   889,   890,    45,    46,    28,
     655,   848,   346,   945,   977,   898,   958,   837,   935,   869,
     870,   909,   964,   962,   912,   961,    77,   189,   337,   338,
     652,   519,   289,   452,   453,   297,   298,   316,    29,    50,
     196,    51,   531,   532,   106,   107,   108,   545,   546,   109,
     153,   111,   112,   154,   363,   364,   114,   155,   384,   116,
     117,   118,   119,   535,   777,   668,   669,   891,   671,   120,
     121,   122,   123,   204,   366,   157,   158,   159,   160,   259,
     260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,   640,   368,    72,   127,   178,   520,    78,
     177,   348,   113,   176,   445,   670,   178,   200,   466,   469,
     290,   206,   176,   104,   374,   709,   649,   213,   110,   533,
     485,   381,   115,   536,   717,   502,   503,    85,   471,   459,
     341,   504,    11,   505,   506,   507,   508,   236,   950,   254,
     299,   194,   202,   840,   262,   177,   653,   847,   516,    33,
     516,   178,   255,   256,   397,   303,   783,   176,   211,   592,
     749,   331,   203,   490,   428,   430,   432,   434,   436,   437,
     438,   391,     8,   756,   390,   757,   329,   759,   342,   332,
     264,   368,   796,   491,    79,   333,   650,   177,   212,   334,
     212,   304,   398,   178,   303,    68,   614,   176,   176,   784,
     188,   343,   550,    34,   185,   102,   884,   369,   370,   951,
     517,   841,   353,   654,   654,   739,   560,   265,    87,   593,
     750,    73,    81,   266,   267,   268,   269,   270,   271,   336,
     392,   285,   177,   286,    88,    89,    90,   797,   178,    91,
      92,   807,    12,   808,   176,   810,   299,   113,    69,  -256,
     632,  -256,    93,   354,   368,    94,   272,   661,   104,   541,
     113,   113,   624,   110,   599,   600,    13,   115,   549,    10,
     601,   104,   104,   570,    74,    79,   110,   110,   294,    95,
     115,   115,   365,   439,   423,   424,   425,   426,   365,   331,
      97,   359,   747,   299,   212,   365,   211,   102,   542,    99,
     625,   628,   100,   101,   761,    38,   177,   332,    14,    15,
     102,    16,   178,   333,   578,   571,   579,   334,   580,   177,
     581,   446,   582,   744,   177,   178,    74,    17,   212,   679,
     178,   176,    75,    18,   497,   446,    19,    20,   572,   447,
      22,   212,    52,    76,   670,   448,   335,    14,    15,   449,
      16,    56,   885,   447,   680,   745,    43,   336,    85,   448,
      53,    57,   273,   449,    44,   681,    17,   177,   498,   274,
     275,   524,    18,   178,   774,    19,    20,    21,   450,    22,
     637,    23,   639,    86,    49,   775,    85,   544,   547,   451,
      24,    25,   177,   276,    26,    85,    85,    85,   178,   603,
     604,    58,    85,   451,   176,   202,   721,   722,    82,   720,
     776,   356,   177,   351,   723,   724,   725,   726,   178,    83,
      23,   678,   682,   676,   306,   534,    61,   862,   727,    24,
      25,   772,   352,    26,   307,   472,    62,   666,   780,   473,
     325,   326,   365,   472,    95,   474,    79,   586,    80,   686,
     688,   127,   327,   474,   691,    97,   150,   265,   644,    72,
     646,    78,   308,   266,   267,   268,   269,   270,   271,   344,
     832,   441,    66,   328,   309,   102,   177,    79,   310,   606,
     607,   833,   178,   322,   323,   475,   525,   311,   312,   476,
     102,   608,    59,   475,     1,   324,   272,   476,   477,   478,
     479,     2,   295,   829,   830,   102,   477,   478,   479,   636,
     727,   638,   609,   263,    60,   227,   228,   177,   177,   177,
     177,   603,   604,   178,   178,   178,   178,   806,   156,   176,
     176,   176,   176,   773,   712,   389,   714,   263,   177,   227,
     539,    47,    48,    79,   178,   857,   603,   604,   201,   860,
     205,    63,   207,   208,    64,   603,   604,   610,   214,   603,
     604,   177,   978,   218,   615,   219,   719,   178,    47,   561,
     812,   692,   472,   176,   616,    65,   813,    79,    67,   177,
     851,   195,   474,   113,   318,   178,   603,   604,    30,   703,
     704,   176,   568,    31,   104,   605,   292,   293,   186,   110,
      79,   258,   261,   115,   177,   192,    70,   319,   365,   320,
     178,   569,   321,    71,   177,   905,   906,   598,    34,    79,
     178,   444,   475,   365,   365,   541,   476,    35,   365,   188,
     684,    36,   736,   834,   220,   477,   478,   479,   541,   444,
     177,    79,   746,  -300,   126,  -300,   178,   233,   135,   928,
     177,   929,   176,   360,   361,   221,   178,   222,   223,   457,
     224,   537,   176,   177,   128,   177,   458,   177,   538,   178,
     537,   178,   138,   178,   937,   129,   139,   551,   376,   177,
     765,   379,   526,   382,   306,   178,   379,   385,   386,   387,
     388,   537,   472,   584,   307,   306,   876,   584,   556,   786,
     585,   130,   474,   537,   611,   307,   792,   131,    84,   197,
     683,   339,   584,   198,   177,   537,  -256,   133,  -256,   693,
     178,   537,   787,   802,   132,   537,   176,   444,   788,   893,
     209,   177,   789,   177,   309,   177,   894,   178,   310,   178,
     349,   178,   475,   596,   350,   309,   476,   311,   312,   310,
     134,   187,   277,   278,   279,   477,   478,   479,   311,   312,
     394,   190,   193,   411,   280,   281,   401,   402,   412,   199,
     665,   177,   215,    47,  -302,   496,  -302,   178,   177,   217,
     216,   237,   124,   176,   178,   282,   177,   125,   238,   239,
     283,   526,   178,   853,   854,   265,    79,   285,   493,   286,
     496,   266,   267,   268,   269,   270,   271,   528,   529,   530,
     240,   454,   968,   554,   177,   177,   969,   177,   555,   554,
     178,   178,   241,   178,   558,   177,   242,   510,   481,   177,
     243,   178,   751,   244,   272,   178,    79,   752,   518,   245,
     217,   177,   523,   177,   284,   849,   330,   178,  -301,   178,
    -301,   288,   345,   651,   511,   221,   513,   222,   223,   515,
     224,    79,   899,   594,   296,   165,   166,   340,   576,   258,
     265,   317,    79,   904,   620,   168,   266,   267,   268,   269,
     270,   271,   347,   169,   170,   171,   172,   173,    79,    79,
     643,   645,   177,   371,   914,   915,   566,   567,   178,   177,
     399,   400,    79,   372,   647,   178,   102,   177,   177,   272,
     375,   377,   383,   178,   178,    79,   463,   711,    79,   444,
     713,   393,   395,   932,   169,   170,   171,   172,   464,   651,
      47,    79,   396,   715,   938,   406,   778,   941,   779,   408,
     414,   177,   955,   956,   440,   455,   767,   178,   456,   302,
     177,   177,   634,   778,   489,   926,   178,   178,   460,   470,
     959,   499,   176,   176,   500,   482,   483,   484,   481,   587,
     965,   589,   486,   487,   591,   488,   512,   514,   651,   521,
     651,   559,   651,   971,  -335,   543,   973,   974,   509,   975,
     552,   553,   635,   557,   526,   583,   588,   674,   675,   590,
     642,   595,   983,   522,   602,  -335,   641,  -335,  -335,   623,
     211,   612,   689,   355,  -335,  -335,  -335,  -335,   613,   658,
     169,   170,   171,   172,   291,   617,   618,   265,   619,   629,
     656,   755,   657,   266,   267,   268,   269,   270,   271,   339,
     659,   660,   212,   850,   662,   839,   651,   672,   651,   677,
     651,   769,   694,   845,   562,   563,   564,   565,   699,   695,
     696,   697,   698,   700,   705,   574,   272,   575,   265,   285,
     718,   707,   708,   730,   266,   267,   268,   269,   270,   271,
     731,   865,   866,   734,   868,   246,   748,   247,   798,   799,
     248,   753,   875,   526,   754,   758,   877,   760,   762,   763,
     764,   526,   790,   249,   663,   768,   766,   272,   664,   250,
     886,   791,   793,   251,   794,   454,   252,   795,   737,   738,
      85,   770,   771,   742,   743,   804,   803,   805,   265,   526,
     526,  -335,   526,   809,   266,   267,   268,   269,   270,   271,
     526,   621,   622,   836,   526,   821,   626,   627,   811,   822,
     823,   824,  -335,   210,  -335,  -335,   444,   211,   526,   917,
     828,  -335,  -335,  -335,  -335,   835,   919,   272,   654,   838,
     265,   844,   846,   855,   923,   924,   266,   267,   268,   269,
     270,   271,   864,   863,   842,   867,   861,   871,   873,   212,
     872,   878,   879,   874,   410,   882,   883,   892,   800,   801,
     887,   888,   895,   901,   903,   496,   496,   526,   949,   272,
     896,   910,   908,   911,   526,   913,   918,   920,   921,   925,
     843,   930,   526,   526,   933,   814,   815,   701,   702,   818,
     819,   936,   943,   944,   946,   902,   265,   825,   826,   858,
     859,   948,   266,   267,   268,   269,   270,   271,   953,   957,
     481,   960,   963,   967,   984,   972,   526,   339,   976,   358,
     916,   979,   980,   981,    87,   982,   405,   985,   403,   409,
     404,   728,   729,   710,   927,   272,   732,   733,   954,   161,
      88,    89,    90,   740,   741,    91,    92,   225,   952,   648,
     934,   407,   820,   162,   163,   716,   900,   597,    93,   939,
     940,    94,   942,   164,   673,   785,   378,   947,   226,   852,
     227,   228,   856,   880,   881,   577,     0,   229,   230,   231,
     232,   165,   166,     0,     0,    95,     0,     0,   167,   481,
       0,   168,     0,    96,     0,   966,    97,    98,     0,   169,
     170,   171,   172,   173,   234,    99,   140,   174,   100,   101,
     175,   496,     0,   496,     0,     0,   102,     0,   162,   163,
       0,     0,   102,   461,   462,   143,     0,   144,   164,     0,
     145,     0,     0,   463,     0,   146,   147,   148,   149,   150,
       0,   169,   170,   171,   172,   464,   165,   166,     0,   816,
     817,   151,     0,   167,     0,     0,   168,     0,   102,   140,
       0,     0,     0,     0,   169,   170,   171,   172,   173,     0,
     141,   142,   235,     0,     0,   175,     0,     0,   143,   257,
     144,     0,     0,   145,   140,     0,     0,   102,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   362,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   373,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   380,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   548,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   685,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   687,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   690,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   494,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     494,     0,     0,   145,     0,     0,   102,     0,   146,   147,
     148,   149,   631,     0,     0,   -50,   -50,     0,     0,   299,
     140,     0,     0,     0,   151,   -50,   -50,   -50,     0,     0,
     300,   102,   427,   301,     0,   302,     0,   -50,     0,   143,
       0,   144,     0,     0,   145,   140,     0,     0,   -50,   146,
     147,   148,   149,   150,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   212,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,     0,   431,     0,     0,     0,     0,     0,     0,   143,
     151,   144,     0,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,   143,   151,   144,     0,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,     0,   435,     0,     0,     0,     0,     0,  -328,   143,
     151,   144,     0,     0,   145,     0,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,     0,     0,   357,  -328,
       0,  -328,  -328,     0,   557,   151,     0,   265,  -328,  -328,
    -328,  -328,   102,   266,   267,   268,   269,   270,   271,   265,
       0,     0,     0,     0,     0,   266,   267,   268,   269,   270,
     271,   415,   416,   417,   418,   419,   420,   421,   422,     0,
       0,   265,   413,     0,     0,     0,   272,   266,   267,   268,
     269,   270,   271,     0,   265,     0,     0,     0,   272,   540,
     266,   267,   268,   269,   270,   271,   265,     0,     0,     0,
       0,     0,   266,   267,   268,   269,   270,   271,   781,   265,
     272,     0,     0,     0,   782,   266,   267,   268,   269,   270,
     271,     0,     0,   272,     0,     0,     0,     0,     0,     0,
     907,   265,     0,     0,     0,   272,     0,   266,   267,   268,
     269,   270,   271,     0,     0,     0,     0,     0,   272,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     272
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   501,   212,    39,    54,    71,   342,    45,
      80,   196,    51,    71,   287,   535,    80,    89,   301,   302,
     164,    93,    80,    51,   217,   584,     6,    99,    51,   353,
     309,   224,    51,   357,   594,   322,   323,     5,   304,   299,
       5,   324,     2,   325,   326,   327,   328,   125,     5,   140,
      48,     5,    91,     8,   145,   125,     9,     9,     5,     5,
       5,   125,   141,   142,    38,    28,    17,   125,    64,     5,
       5,     8,    91,     5,   277,   278,   279,   280,   281,   282,
     283,     5,    76,   643,     5,   645,    50,   647,    53,    26,
       5,   299,     5,    25,    58,    32,    76,   167,    96,    36,
      96,    64,    76,   167,    28,     5,     5,   165,   166,    60,
      64,    76,   372,    59,    74,    95,    63,    85,    86,    76,
      65,    76,    17,    76,    76,   623,   392,    42,     3,    65,
      65,     5,     5,    48,    49,    50,    51,    52,    53,    76,
      64,    62,   212,    64,    19,    20,    21,    60,   212,    24,
      25,   711,     0,   713,   212,   715,    48,   196,    58,    58,
     494,    60,    37,    58,   372,    40,    81,    59,   196,    58,
     209,   210,    38,   196,   461,   462,    59,   196,   371,    76,
     463,   209,   210,     5,    58,    58,   209,   210,    76,    64,
     209,   210,   211,   284,   273,   274,   275,   276,   217,     8,
      75,    76,    60,    48,    96,   224,    64,    95,    97,    84,
      76,   490,    87,    88,    59,    76,   286,    26,     7,     8,
      95,    10,   286,    32,   427,    14,   429,    36,   431,   299,
     433,     8,   435,    28,   304,   299,    58,    26,    96,     5,
     304,   299,    48,    32,    28,     8,    35,    36,    37,    26,
      39,    96,    76,    59,   774,    32,    65,     7,     8,    36,
      10,    26,   821,    26,    30,    60,    76,    76,     5,    32,
      94,    36,    47,    36,    76,    41,    26,   347,    62,    54,
      55,   346,    32,   347,     5,    35,    36,    37,    65,    39,
     498,    80,   500,    30,    76,    16,     5,   369,   370,    76,
      89,    90,   372,    78,    93,     5,     5,     5,   372,    54,
      55,    76,     5,    76,   372,   354,   603,   604,    48,   602,
      41,    30,   392,    23,   606,   607,   608,   609,   392,    59,
      80,    30,    30,   541,    26,   354,    26,    30,   611,    89,
      90,   665,    17,    93,    36,    26,    36,   532,   672,    30,
      44,    45,   371,    26,    64,    36,    58,    30,    60,   552,
     553,   409,    56,    36,   557,    75,    76,    42,   512,   403,
     514,   407,    64,    48,    49,    50,    51,    52,    53,    50,
      65,    76,    12,    77,    76,    95,   456,    58,    80,    44,
      45,    76,   456,    54,    55,    76,    76,    89,    90,    80,
      95,    56,    76,    76,     4,    66,    81,    80,    89,    90,
      91,    11,   167,   747,   748,    95,    89,    90,    91,   497,
     693,   499,    77,    59,    76,    61,    62,   497,   498,   499,
     500,    54,    55,   497,   498,   499,   500,   710,    70,   497,
     498,   499,   500,    66,   588,    50,   590,    59,   518,    61,
      62,    18,    19,    58,   518,   779,    54,    55,    90,   783,
      92,    76,    94,    95,    31,    54,    55,    65,   100,    54,
      55,   541,   970,    59,    26,    61,    65,   541,    45,    50,
      65,   559,    26,   541,    36,     5,    30,    58,    76,   559,
     773,    84,    36,   532,     6,   559,    54,    55,    59,   571,
     572,   559,    50,    64,   532,    63,   165,   166,    75,   532,
      58,   143,   144,   532,   584,    82,    53,    29,   537,    31,
     584,    50,    34,    60,   594,   849,   850,    50,    59,    58,
     594,   286,    76,   552,   553,    58,    80,    72,   557,    64,
      50,    76,   620,   751,    38,    89,    90,    91,    58,   304,
     620,    58,   630,    59,    76,    61,   620,   124,    65,   893,
     630,   895,   620,   209,   210,    59,   630,    61,    62,    58,
      64,    58,   630,   643,    76,   645,    65,   647,    65,   643,
      58,   645,    72,   647,   908,    76,    76,    65,   220,   659,
     655,   223,   347,   225,    26,   659,   228,   229,   230,   231,
     232,    58,    26,    58,    36,    26,    30,    58,    65,   681,
      65,    76,    36,    58,    65,    36,   694,    59,    49,    72,
      65,   188,    58,    76,   694,    58,    58,    76,    60,    65,
     694,    58,    65,   705,    59,    58,   694,   392,    65,    58,
      60,   711,    65,   713,    76,   715,    65,   711,    80,   713,
      38,   715,    76,   456,    42,    76,    80,    89,    90,    80,
      76,    76,    44,    45,    46,    89,    90,    91,    89,    90,
     237,    76,    76,    58,    56,    57,   243,   244,    63,    76,
      17,   751,    76,   250,    59,   317,    61,   751,   758,    64,
      76,    48,    48,   751,   758,    77,   766,    53,    59,    59,
      82,   456,   766,   775,   776,    42,    58,    62,    60,    64,
     342,    48,    49,    50,    51,    52,    53,   349,   350,   351,
      59,   288,    72,    58,   794,   795,    76,   797,    63,    58,
     794,   795,    76,   797,    63,   805,    76,   330,   305,   809,
      48,   805,    58,    48,    81,   809,    58,    63,    60,    59,
      64,   821,   345,   823,    79,    17,   187,   821,    59,   823,
      61,    64,   193,   518,   331,    59,   333,    61,    62,   336,
      64,    58,   837,    60,    76,    54,    55,     5,   410,   411,
      42,    53,    58,   848,    60,    64,    48,    49,    50,    51,
      52,    53,    60,    72,    73,    74,    75,    76,    58,    58,
      60,    60,   872,    64,   869,   870,   399,   400,   872,   879,
     241,   242,    58,    48,    60,   879,    95,   887,   888,    81,
      76,    76,    76,   887,   888,    58,    64,    60,    58,   584,
      60,     5,    76,   898,    72,    73,    74,    75,    76,   594,
     407,    58,    76,    60,   909,    76,    58,   912,    60,    76,
      76,   921,   930,   931,    75,    28,   659,   921,    60,    64,
     930,   931,   494,    58,    30,    60,   930,   931,    76,    76,
     935,    28,   930,   931,    28,    76,    76,    76,   445,   446,
     945,   448,    76,    76,   451,    76,    76,    76,   643,    76,
     645,    53,   647,   958,    38,    92,   961,   962,   329,   964,
      64,    64,     5,    64,   659,    63,    76,   539,   540,    76,
       5,    76,   977,   344,    64,    59,   509,    61,    62,    65,
      64,    76,   554,    28,    68,    69,    70,    71,    76,   522,
      72,    73,    74,    75,    76,    76,    76,    42,    76,    76,
       5,   642,     5,    48,    49,    50,    51,    52,    53,   516,
      60,     5,    96,    17,     5,   758,   711,    41,   713,    76,
     715,   662,    53,   766,   395,   396,   397,   398,     5,   562,
     563,   564,   565,     5,    15,   406,    81,   408,    42,    62,
      28,   574,   575,    76,    48,    49,    50,    51,    52,    53,
      76,   794,   795,    62,   797,     8,    58,    10,   699,   700,
      13,    30,   805,   758,     5,    60,   809,    28,    53,     5,
       5,   766,     5,    26,    18,    76,    60,    81,    22,    32,
     823,     5,     5,    36,    60,   592,    39,    60,   621,   622,
       5,   663,   664,   626,   627,     5,    30,    60,    42,   794,
     795,    38,   797,    60,    48,    49,    50,    51,    52,    53,
     805,   482,   483,   754,   809,    60,   487,   488,    76,     5,
      60,    30,    59,    60,    61,    62,   821,    64,   823,   872,
      76,    68,    69,    70,    71,    28,   879,    81,    76,    53,
      42,     5,     5,    30,   887,   888,    48,    49,    50,    51,
      52,    53,   793,     5,    76,   796,    76,     5,    59,    96,
      60,     5,    60,   804,    66,     5,     5,    28,   701,   702,
      60,    60,    60,     5,     5,   747,   748,   872,   921,    81,
      63,     5,    17,     5,   879,     5,     5,     5,    60,     5,
     762,    28,   887,   888,     5,   728,   729,   568,   569,   732,
     733,     5,     5,     5,     5,   846,    42,   740,   741,   781,
     782,     5,    48,    49,    50,    51,    52,    53,    64,     5,
     727,     5,     5,     5,    65,     5,   921,   734,     5,    65,
     871,     5,     5,     5,     3,     5,   248,     5,   246,   252,
     247,   612,   613,   585,   892,    81,   617,   618,   927,    12,
      19,    20,    21,   624,   625,    24,    25,    38,   925,   516,
     901,   250,   734,    26,    27,   592,   838,   457,    37,   910,
     911,    40,   913,    36,   537,   679,   223,   918,    59,   774,
      61,    62,   778,   816,   817,   411,    -1,    68,    69,    70,
      71,    54,    55,    -1,    -1,    64,    -1,    -1,    61,   806,
      -1,    64,    -1,    72,    -1,   946,    75,    76,    -1,    72,
      73,    74,    75,    76,    12,    84,    43,    80,    87,    88,
      83,   893,    -1,   895,    -1,    -1,    95,    -1,    26,    27,
      -1,    -1,    95,    54,    55,    62,    -1,    64,    36,    -1,
      67,    -1,    -1,    64,    -1,    72,    73,    74,    75,    76,
      -1,    72,    73,    74,    75,    76,    54,    55,    -1,   730,
     731,    88,    -1,    61,    -1,    -1,    64,    -1,    95,    43,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,
      54,    55,    80,    -1,    -1,    83,    -1,    -1,    62,    63,
      64,    -1,    -1,    67,    43,    -1,    -1,    95,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    65,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    65,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    65,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    -1,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    44,    45,    -1,    -1,    48,
      43,    -1,    -1,    -1,    88,    54,    55,    56,    -1,    -1,
      59,    95,    55,    62,    -1,    64,    -1,    66,    -1,    62,
      -1,    64,    -1,    -1,    67,    43,    -1,    -1,    77,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    96,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,
      43,    -1,    95,    -1,    72,    73,    74,    75,    76,    -1,
      -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    38,    62,
      88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    -1,    -1,    33,    59,
      -1,    61,    62,    -1,    64,    88,    -1,    42,    68,    69,
      70,    71,    95,    48,    49,    50,    51,    52,    53,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,   265,   266,   267,   268,   269,   270,   271,   272,    -1,
      -1,    42,    65,    -1,    -1,    -1,    81,    48,    49,    50,
      51,    52,    53,    -1,    42,    -1,    -1,    -1,    81,    60,
      48,    49,    50,    51,    52,    53,    42,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    66,    42,
      81,    -1,    -1,    -1,    60,    48,    49,    50,    51,    52,
      53,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    42,    -1,    -1,    -1,    81,    -1,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81
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
     175,   185,    76,   185,    76,   185,     5,    65,    60,   179,
     114,    76,   174,   175,   103,    76,   127,   133,   222,   222,
     222,   190,   191,   190,   205,   211,   190,    58,    65,    62,
      60,    58,    97,    92,   187,   195,   196,   187,    65,   202,
     128,    65,    64,    64,    58,    63,    65,    64,    63,    53,
     124,    50,   174,   174,   174,   174,   175,   175,    50,    50,
       5,    14,    37,   109,   174,   174,   222,   228,   225,   225,
     225,   225,   225,    63,    58,    65,    30,   185,    76,   185,
      76,   185,     5,    65,    60,    76,   133,   184,    50,   144,
     144,   143,    64,    54,    55,    63,    44,    45,    56,    77,
      65,    65,    76,    76,     5,    26,    36,    76,    76,    76,
      60,   174,   174,    65,    38,    76,   174,   174,   148,    76,
     149,    76,   114,   116,   222,     5,   129,   140,   129,   140,
     146,   175,     5,    60,   180,    60,   180,    60,   177,     6,
      76,   127,   178,     9,    76,   158,     5,     5,   175,    60,
       5,    59,     5,    18,    22,    17,   192,   143,   213,   214,
     215,   216,    41,   203,   222,   222,   140,    76,    30,     5,
      30,    41,    30,    65,    50,    65,   202,    65,   202,   222,
      65,   202,   129,    65,    53,   175,   175,   175,   175,     5,
       5,   174,   174,   187,   187,    15,   110,   175,   175,   125,
     134,    60,   180,    60,   180,    60,   182,   178,    28,    65,
     143,   144,   144,   145,   145,   145,   145,   135,   174,   174,
      76,    76,   174,   174,    62,   137,   129,   175,   175,   146,
     174,   174,   175,   175,    28,    60,   129,    60,    58,     5,
      65,    58,    63,    30,     5,   179,   178,   178,    60,   178,
      28,    59,    53,     5,     5,   103,    60,   133,    76,   179,
     222,   222,   190,    66,     5,    16,    41,   212,    58,    60,
     190,    66,    60,    17,    60,   196,   187,    65,    65,    65,
       5,     5,   129,     5,    60,    60,     5,    60,   179,   179,
     175,   175,   187,    30,     5,    60,   135,   178,   178,    60,
     178,    76,    65,    30,   175,   175,   174,   174,   175,   175,
     176,    60,     5,    60,    30,   175,   175,   151,    76,   114,
     114,   115,    65,    76,   140,    28,   179,   165,    53,   133,
       8,    76,    76,   222,     5,   133,     5,     9,   159,    17,
      17,   143,   214,   187,   187,    30,   216,   190,   222,   222,
     190,    76,    30,     5,   179,   133,   133,   179,   133,   167,
     168,     5,    60,    59,   179,   133,    30,   133,     5,    60,
     175,   175,     5,     5,    63,   125,   133,    60,    60,   153,
     154,   215,    28,    58,    65,    60,    63,   130,   163,   103,
     222,     5,   179,     5,   103,   190,   190,    63,    17,   169,
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
       0,     2,     1,     4,     6,     6,     8,    10,     0,     9,
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
#line 2315 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2321 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2327 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2333 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2339 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2345 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2351 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2357 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2363 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2369 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2375 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2381 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2387 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2393 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2399 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2405 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2423 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2429 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2435 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2441 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2447 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2453 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2459 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2477 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2489 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2495 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2501 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2507 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2525 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2531 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2543 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2549 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2555 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2561 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2567 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2573 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2579 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2585 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2591 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2597 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2603 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2609 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2615 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2621 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2627 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2639 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2645 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2651 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2657 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2663 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2669 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2675 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2681 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2687 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2693 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2705 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2711 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2717 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2729 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2735 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2741 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2753 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2759 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2783 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2795 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2801 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2807 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2813 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2837 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2843 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2849 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2855 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2861 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2885 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2897 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2903 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2909 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2915 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2921 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2927 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2933 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2939 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2945 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2951 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2957 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2963 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2969 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2975 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2981 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2987 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2993 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2999 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3005 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3011 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3017 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3023 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3029 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3035 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3041 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3047 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3053 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3059 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3065 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3071 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3077 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3089 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3095 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3101 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3107 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3119 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3125 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3131 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3137 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 439 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3143 "pascal.tab.c"
    break;

  case 141: /* lo_term: lo_term MUL b_factor  */
#line 440 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term IDIV b_factor  */
#line 441 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3155 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IMOD b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term SHIFTOP b_factor  */
#line 443 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 145: /* b_expr: b_term  */
#line 446 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3173 "pascal.tab.c"
    break;

  case 146: /* b_expr: PLUS b_term  */
#line 447 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 147: /* b_expr: MINUS b_term  */
#line 448 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 148: /* b_expr: b_expr PLUS b_term  */
#line 449 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3191 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr MINUS b_term  */
#line 450 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3197 "pascal.tab.c"
    break;

  case 150: /* b_term: b_factor  */
#line 453 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3203 "pascal.tab.c"
    break;

  case 151: /* b_term: b_term MUL b_factor  */
#line 454 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term IDIV b_factor  */
#line 455 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3215 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IMOD b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3221 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term SHIFTOP b_factor  */
#line 457 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3227 "pascal.tab.c"
    break;

  case 155: /* b_factor: scalar_constant  */
#line 460 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3233 "pascal.tab.c"
    break;

  case 156: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 461 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 157: /* b_factor: LPARENT b_expr RPARENT  */
#line 462 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3245 "pascal.tab.c"
    break;

  case 158: /* record_body: record_field_list record_case_opt  */
#line 465 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3251 "pascal.tab.c"
    break;

  case 159: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 468 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 160: /* record_field_list: record_field  */
#line 469 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 161: /* $@4: %empty  */
#line 472 "pascal.y"
                  { }
#line 3269 "pascal.tab.c"
    break;

  case 162: /* record_field: id_list COLON $@4 type  */
#line 472 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3275 "pascal.tab.c"
    break;

  case 163: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 473 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3281 "pascal.tab.c"
    break;

  case 164: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 474 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3287 "pascal.tab.c"
    break;

  case 165: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3293 "pascal.tab.c"
    break;

  case 166: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 476 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3299 "pascal.tab.c"
    break;

  case 167: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 477 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3305 "pascal.tab.c"
    break;

  case 168: /* record_field: IDENT record_field  */
#line 478 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3311 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3317 "pascal.tab.c"
    break;

  case 170: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 480 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3323 "pascal.tab.c"
    break;

  case 171: /* record_field: %empty  */
#line 481 "pascal.y"
     { (yyval.node) = NULL; }
#line 3329 "pascal.tab.c"
    break;

  case 172: /* case_arm_mark: %empty  */
#line 484 "pascal.y"
    { }
#line 3335 "pascal.tab.c"
    break;

  case 173: /* case_open: %empty  */
#line 487 "pascal.y"
    { }
#line 3341 "pascal.tab.c"
    break;

  case 174: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 490 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3347 "pascal.tab.c"
    break;

  case 175: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 491 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3353 "pascal.tab.c"
    break;

  case 176: /* record_case_opt: %empty  */
#line 492 "pascal.y"
     { (yyval.node) = NULL; }
#line 3359 "pascal.tab.c"
    break;

  case 177: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 495 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 178: /* record_case_list: record_case_arm  */
#line 496 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3371 "pascal.tab.c"
    break;

  case 179: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 499 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3377 "pascal.tab.c"
    break;

  case 180: /* record_case_arm: %empty  */
#line 500 "pascal.y"
     { (yyval.node) = NULL; }
#line 3383 "pascal.tab.c"
    break;

  case 181: /* var_decl_list: var_decl_list var_decl  */
#line 503 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3389 "pascal.tab.c"
    break;

  case 182: /* var_decl_list: var_decl  */
#line 504 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3395 "pascal.tab.c"
    break;

  case 183: /* var_decl: id_list COLON type SEMICOLON  */
#line 507 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3401 "pascal.tab.c"
    break;

  case 184: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 508 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3407 "pascal.tab.c"
    break;

  case 185: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 509 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3413 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 512 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3419 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3425 "pascal.tab.c"
    break;

  case 188: /* $@5: %empty  */
#line 514 "pascal.y"
                                                                               { }
#line 3431 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3437 "pascal.tab.c"
    break;

  case 190: /* $@6: %empty  */
#line 515 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3443 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 515 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3449 "pascal.tab.c"
    break;

  case 192: /* $@7: %empty  */
#line 516 "pascal.y"
                                        { }
#line 3455 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 516 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3461 "pascal.tab.c"
    break;

  case 194: /* $@8: %empty  */
#line 517 "pascal.y"
                                                                                                           { }
#line 3467 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3473 "pascal.tab.c"
    break;

  case 196: /* $@9: %empty  */
#line 518 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3479 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3485 "pascal.tab.c"
    break;

  case 198: /* $@10: %empty  */
#line 519 "pascal.y"
                                                                                                 { }
#line 3491 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3497 "pascal.tab.c"
    break;

  case 200: /* $@11: %empty  */
#line 520 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3503 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3509 "pascal.tab.c"
    break;

  case 202: /* $@12: %empty  */
#line 521 "pascal.y"
                                                                                            { }
#line 3515 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3521 "pascal.tab.c"
    break;

  case 204: /* $@13: %empty  */
#line 522 "pascal.y"
                                                                                                             { }
#line 3527 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3535 "pascal.tab.c"
    break;

  case 206: /* $@14: %empty  */
#line 525 "pascal.y"
                                                                                              { }
#line 3541 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 525 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3549 "pascal.tab.c"
    break;

  case 208: /* $@15: %empty  */
#line 528 "pascal.y"
                                                                                             { }
#line 3555 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 528 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3563 "pascal.tab.c"
    break;

  case 210: /* $@16: %empty  */
#line 531 "pascal.y"
                                                                                                    { }
#line 3569 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 531 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3577 "pascal.tab.c"
    break;

  case 212: /* $@17: %empty  */
#line 534 "pascal.y"
                                                                                                                { }
#line 3583 "pascal.tab.c"
    break;

  case 213: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 534 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3592 "pascal.tab.c"
    break;

  case 214: /* $@18: %empty  */
#line 538 "pascal.y"
                                                                                                                  { }
#line 3598 "pascal.tab.c"
    break;

  case 215: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 538 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3607 "pascal.tab.c"
    break;

  case 216: /* $@19: %empty  */
#line 542 "pascal.y"
                                                                                              { }
#line 3613 "pascal.tab.c"
    break;

  case 217: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 542 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3622 "pascal.tab.c"
    break;

  case 218: /* $@20: %empty  */
#line 546 "pascal.y"
                                                                                                                     { }
#line 3628 "pascal.tab.c"
    break;

  case 219: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 546 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3636 "pascal.tab.c"
    break;

  case 220: /* pv_mark: %empty  */
#line 551 "pascal.y"
    { }
#line 3642 "pascal.tab.c"
    break;

  case 221: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 554 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3648 "pascal.tab.c"
    break;

  case 222: /* parameter_list_opt: LPARENT RPARENT  */
#line 555 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3654 "pascal.tab.c"
    break;

  case 223: /* parameter_list_opt: %empty  */
#line 556 "pascal.y"
     { (yyval.node) = NULL; }
#line 3660 "pascal.tab.c"
    break;

  case 224: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 559 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3666 "pascal.tab.c"
    break;

  case 225: /* parameter_decl_list: parameter_decl  */
#line 560 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3672 "pascal.tab.c"
    break;

  case 226: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 563 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3678 "pascal.tab.c"
    break;

  case 227: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 564 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3684 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: VARSY id_list COLON param_type  */
#line 565 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3690 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: id_list COLON param_type  */
#line 566 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3696 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 567 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3702 "pascal.tab.c"
    break;

  case 231: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 568 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3708 "pascal.tab.c"
    break;

  case 232: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 569 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3714 "pascal.tab.c"
    break;

  case 233: /* parameter_decl: IDENT id_list COLON param_type  */
#line 570 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3720 "pascal.tab.c"
    break;

  case 234: /* param_type: IDENT  */
#line 573 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3726 "pascal.tab.c"
    break;

  case 235: /* param_type: IDENT PERIOD IDENT  */
#line 574 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3732 "pascal.tab.c"
    break;

  case 236: /* param_type: ARRAYSY OFSY IDENT  */
#line 575 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3738 "pascal.tab.c"
    break;

  case 237: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 576 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3744 "pascal.tab.c"
    break;

  case 238: /* param_type: spec_ref  */
#line 577 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3750 "pascal.tab.c"
    break;

  case 239: /* routine_directives: %empty  */
#line 580 "pascal.y"
    { (yyval.node) = NULL; }
#line 3756 "pascal.tab.c"
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
#line 3769 "pascal.tab.c"
    break;

  case 241: /* pf_params: LPARENT pf_sections RPARENT  */
#line 591 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3775 "pascal.tab.c"
    break;

  case 242: /* pf_params: LPARENT RPARENT  */
#line 592 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3781 "pascal.tab.c"
    break;

  case 243: /* pf_params: %empty  */
#line 593 "pascal.y"
     { (yyval.node) = NULL; }
#line 3787 "pascal.tab.c"
    break;

  case 244: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 596 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3793 "pascal.tab.c"
    break;

  case 245: /* pf_sections: pf_section  */
#line 597 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3799 "pascal.tab.c"
    break;

  case 246: /* pf_section: id_list COLON param_type  */
#line 600 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3805 "pascal.tab.c"
    break;

  case 247: /* pf_section: VARSY id_list COLON param_type  */
#line 601 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3811 "pascal.tab.c"
    break;

  case 248: /* pf_section: CONSTSY id_list COLON param_type  */
#line 602 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3817 "pascal.tab.c"
    break;

  case 249: /* pf_section: IDENT id_list COLON param_type  */
#line 603 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3823 "pascal.tab.c"
    break;

  case 250: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 604 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3829 "pascal.tab.c"
    break;

  case 251: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 605 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3835 "pascal.tab.c"
    break;

  case 252: /* enum_list: enum_list COMMA enum_member  */
#line 608 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3841 "pascal.tab.c"
    break;

  case 253: /* enum_list: enum_member  */
#line 609 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3847 "pascal.tab.c"
    break;

  case 254: /* enum_member: IDENT  */
#line 612 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3853 "pascal.tab.c"
    break;

  case 255: /* id_list: id_list COMMA IDENT  */
#line 615 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3859 "pascal.tab.c"
    break;

  case 256: /* id_list: IDENT  */
#line 616 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3865 "pascal.tab.c"
    break;

  case 257: /* body: BEGINSY statement_list ENDSY  */
#line 619 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3871 "pascal.tab.c"
    break;

  case 258: /* $@21: %empty  */
#line 622 "pascal.y"
                             { pas_stmt_mark(); }
#line 3877 "pascal.tab.c"
    break;

  case 259: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 622 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3883 "pascal.tab.c"
    break;

  case 260: /* $@22: %empty  */
#line 623 "pascal.y"
     { pas_stmt_mark(); }
#line 3889 "pascal.tab.c"
    break;

  case 261: /* statement_list: $@22 statement  */
#line 623 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3895 "pascal.tab.c"
    break;

  case 262: /* $@23: %empty  */
#line 626 "pascal.y"
    { pas_stmt_mark(); }
#line 3901 "pascal.tab.c"
    break;

  case 263: /* body_stmt: $@23 statement  */
#line 626 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3907 "pascal.tab.c"
    break;

  case 264: /* statement: statement_no_label  */
#line 629 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3913 "pascal.tab.c"
    break;

  case 265: /* statement: INTCONST COLON statement_no_label  */
#line 630 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3919 "pascal.tab.c"
    break;

  case 266: /* statement: IDENT COLON statement_no_label  */
#line 631 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3925 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: assignment  */
#line 634 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3931 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: call  */
#line 635 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3937 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: compound_statement  */
#line 636 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3943 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: goto_statement  */
#line 637 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3949 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: if_statement  */
#line 638 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3955 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: case_statement  */
#line 639 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3961 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: while_statement  */
#line 640 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3967 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: repeat_statement  */
#line 641 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3973 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: for_statement  */
#line 642 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3979 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: with_statement  */
#line 643 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3985 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: try_statement  */
#line 644 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3991 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: raise_statement  */
#line 645 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3997 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: inherited_call  */
#line 646 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4003 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: INHERITEDSY  */
#line 647 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4009 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: %empty  */
#line 648 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4015 "pascal.tab.c"
    break;

  case 282: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 651 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4021 "pascal.tab.c"
    break;

  case 283: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 652 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4027 "pascal.tab.c"
    break;

  case 284: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 653 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4033 "pascal.tab.c"
    break;

  case 285: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 654 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4039 "pascal.tab.c"
    break;

  case 286: /* handler_list: handler  */
#line 657 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4045 "pascal.tab.c"
    break;

  case 287: /* handler_list: handler_list SEMICOLON handler  */
#line 658 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4051 "pascal.tab.c"
    break;

  case 288: /* handler_list: handler_list SEMICOLON  */
#line 659 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4057 "pascal.tab.c"
    break;

  case 289: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 662 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4063 "pascal.tab.c"
    break;

  case 290: /* handler: ONSY IDENT DOSY body_stmt  */
#line 663 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4069 "pascal.tab.c"
    break;

  case 291: /* raise_statement: RAISESY  */
#line 666 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4075 "pascal.tab.c"
    break;

  case 292: /* raise_statement: RAISESY expression  */
#line 667 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4081 "pascal.tab.c"
    break;

  case 293: /* inherited_call: INHERITEDSY IDENT  */
#line 670 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4087 "pascal.tab.c"
    break;

  case 294: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 671 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4093 "pascal.tab.c"
    break;

  case 295: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 672 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4099 "pascal.tab.c"
    break;

  case 296: /* call: IDENT  */
#line 675 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4105 "pascal.tab.c"
    break;

  case 297: /* call: spec_expr  */
#line 676 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4111 "pascal.tab.c"
    break;

  case 298: /* call: selector PERIOD IDENT  */
#line 677 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4117 "pascal.tab.c"
    break;

  case 299: /* call: call_with_args  */
#line 678 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4123 "pascal.tab.c"
    break;

  case 300: /* recv_primary: STRINGCONST  */
#line 681 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4129 "pascal.tab.c"
    break;

  case 301: /* recv_primary: LPARENT expression RPARENT  */
#line 682 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4135 "pascal.tab.c"
    break;

  case 302: /* recv_primary: spec_expr  */
#line 683 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4141 "pascal.tab.c"
    break;

  case 303: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 686 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4147 "pascal.tab.c"
    break;

  case 304: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 687 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4153 "pascal.tab.c"
    break;

  case 305: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 688 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4159 "pascal.tab.c"
    break;

  case 306: /* call_with_args: IDENT LPARENT RPARENT  */
#line 689 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4165 "pascal.tab.c"
    break;

  case 307: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 690 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4171 "pascal.tab.c"
    break;

  case 308: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 691 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4177 "pascal.tab.c"
    break;

  case 309: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 692 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4183 "pascal.tab.c"
    break;

  case 310: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 693 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4189 "pascal.tab.c"
    break;

  case 311: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 694 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4195 "pascal.tab.c"
    break;

  case 312: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 695 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4201 "pascal.tab.c"
    break;

  case 313: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 696 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4207 "pascal.tab.c"
    break;

  case 314: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 697 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4213 "pascal.tab.c"
    break;

  case 315: /* argument_list: argument_list COMMA argument  */
#line 700 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4219 "pascal.tab.c"
    break;

  case 316: /* argument_list: argument  */
#line 701 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4225 "pascal.tab.c"
    break;

  case 317: /* argument: expression  */
#line 704 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4231 "pascal.tab.c"
    break;

  case 318: /* argument: expression COLON expression  */
#line 705 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4237 "pascal.tab.c"
    break;

  case 319: /* argument: expression COLON expression COLON expression  */
#line 706 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4243 "pascal.tab.c"
    break;

  case 320: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 707 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4249 "pascal.tab.c"
    break;

  case 321: /* assignment: selector BECOMES expression  */
#line 710 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4255 "pascal.tab.c"
    break;

  case 322: /* assignment: selector ADDBECOMES expression  */
#line 711 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4261 "pascal.tab.c"
    break;

  case 323: /* assignment: selector SUBBECOMES expression  */
#line 712 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4267 "pascal.tab.c"
    break;

  case 324: /* assignment: selector MULBECOMES expression  */
#line 713 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4273 "pascal.tab.c"
    break;

  case 325: /* assignment: selector DIVBECOMES expression  */
#line 714 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4279 "pascal.tab.c"
    break;

  case 326: /* assignment: call_with_args BECOMES expression  */
#line 715 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4285 "pascal.tab.c"
    break;

  case 327: /* selector: selector LBRACK expression_list RBRACK  */
#line 718 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4291 "pascal.tab.c"
    break;

  case 328: /* selector: selector PERIOD IDENT  */
#line 719 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4297 "pascal.tab.c"
    break;

  case 329: /* selector: selector ARROW  */
#line 720 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4303 "pascal.tab.c"
    break;

  case 330: /* selector: call_with_args PERIOD IDENT  */
#line 721 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4309 "pascal.tab.c"
    break;

  case 331: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 722 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4315 "pascal.tab.c"
    break;

  case 332: /* selector: call_with_args ARROW  */
#line 723 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4321 "pascal.tab.c"
    break;

  case 333: /* selector: recv_primary PERIOD IDENT  */
#line 724 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4327 "pascal.tab.c"
    break;

  case 334: /* selector: recv_primary ARROW  */
#line 725 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4333 "pascal.tab.c"
    break;

  case 335: /* selector: IDENT  */
#line 726 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4339 "pascal.tab.c"
    break;

  case 336: /* expression_list: expression_list COMMA expression  */
#line 729 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4345 "pascal.tab.c"
    break;

  case 337: /* expression_list: expression  */
#line 730 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4351 "pascal.tab.c"
    break;

  case 338: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 733 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4357 "pascal.tab.c"
    break;

  case 339: /* goto_statement: GOTOSY INTCONST  */
#line 736 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4363 "pascal.tab.c"
    break;

  case 340: /* goto_statement: GOTOSY IDENT  */
#line 737 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4369 "pascal.tab.c"
    break;

  case 341: /* if_statement: IFSY expression THENSY body_stmt  */
#line 740 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4375 "pascal.tab.c"
    break;

  case 342: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 741 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4381 "pascal.tab.c"
    break;

  case 343: /* $@24: %empty  */
#line 744 "pascal.y"
                           { }
#line 4387 "pascal.tab.c"
    break;

  case 344: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 744 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4393 "pascal.tab.c"
    break;

  case 345: /* case_tail: %empty  */
#line 747 "pascal.y"
    { (yyval.node) = NULL; }
#line 4399 "pascal.tab.c"
    break;

  case 346: /* case_tail: ELSESY statement_list  */
#line 748 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4405 "pascal.tab.c"
    break;

  case 347: /* case_tail: OTHERWISESY statement_list  */
#line 749 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4411 "pascal.tab.c"
    break;

  case 348: /* case_list: case_list SEMICOLON case_elem  */
#line 752 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4417 "pascal.tab.c"
    break;

  case 349: /* case_list: case_elem  */
#line 753 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4423 "pascal.tab.c"
    break;

  case 350: /* case_elem: constant_list COLON body_stmt  */
#line 756 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4429 "pascal.tab.c"
    break;

  case 351: /* case_elem: %empty  */
#line 757 "pascal.y"
     { (yyval.node) = NULL; }
#line 4435 "pascal.tab.c"
    break;

  case 352: /* constant_list: constant_list COMMA case_label  */
#line 760 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4441 "pascal.tab.c"
    break;

  case 353: /* constant_list: case_label  */
#line 761 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4447 "pascal.tab.c"
    break;

  case 354: /* case_label: b_expr  */
#line 764 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4453 "pascal.tab.c"
    break;

  case 355: /* case_label: b_expr DOTDOT b_expr  */
#line 765 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4459 "pascal.tab.c"
    break;

  case 356: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 768 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4465 "pascal.tab.c"
    break;

  case 357: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 771 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4471 "pascal.tab.c"
    break;

  case 358: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 774 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4477 "pascal.tab.c"
    break;

  case 359: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 775 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4483 "pascal.tab.c"
    break;

  case 360: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 776 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4489 "pascal.tab.c"
    break;

  case 361: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 779 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4495 "pascal.tab.c"
    break;

  case 362: /* with_open: with_open COMMA selector  */
#line 782 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4501 "pascal.tab.c"
    break;

  case 363: /* with_open: selector  */
#line 783 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4507 "pascal.tab.c"
    break;

  case 364: /* expression: simple_expression  */
#line 790 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4513 "pascal.tab.c"
    break;

  case 365: /* expression: expression INOP simple_expression  */
#line 791 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4519 "pascal.tab.c"
    break;

  case 366: /* expression: expression LTOP simple_expression  */
#line 792 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4525 "pascal.tab.c"
    break;

  case 367: /* expression: expression LEOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4531 "pascal.tab.c"
    break;

  case 368: /* expression: expression GTOP simple_expression  */
#line 794 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4537 "pascal.tab.c"
    break;

  case 369: /* expression: expression GEOP simple_expression  */
#line 795 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4543 "pascal.tab.c"
    break;

  case 370: /* expression: expression NEOP simple_expression  */
#line 796 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4549 "pascal.tab.c"
    break;

  case 371: /* expression: expression EQOP simple_expression  */
#line 797 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4555 "pascal.tab.c"
    break;

  case 372: /* expression: expression ISOP simple_expression  */
#line 798 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4561 "pascal.tab.c"
    break;

  case 373: /* simple_expression: term  */
#line 801 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4567 "pascal.tab.c"
    break;

  case 374: /* simple_expression: PLUS term  */
#line 802 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4573 "pascal.tab.c"
    break;

  case 375: /* simple_expression: MINUS term  */
#line 803 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4579 "pascal.tab.c"
    break;

  case 376: /* simple_expression: simple_expression PLUS term  */
#line 804 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4585 "pascal.tab.c"
    break;

  case 377: /* simple_expression: simple_expression MINUS term  */
#line 805 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4591 "pascal.tab.c"
    break;

  case 378: /* simple_expression: simple_expression OROP term  */
#line 806 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4597 "pascal.tab.c"
    break;

  case 379: /* simple_expression: simple_expression XOROP term  */
#line 807 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4603 "pascal.tab.c"
    break;

  case 380: /* term: power  */
#line 810 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4609 "pascal.tab.c"
    break;

  case 381: /* term: term MUL power  */
#line 811 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4615 "pascal.tab.c"
    break;

  case 382: /* term: term RDIV power  */
#line 812 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4621 "pascal.tab.c"
    break;

  case 383: /* term: term IDIV power  */
#line 813 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4627 "pascal.tab.c"
    break;

  case 384: /* term: term IMOD power  */
#line 814 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4633 "pascal.tab.c"
    break;

  case 385: /* term: term ANDOP power  */
#line 815 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4639 "pascal.tab.c"
    break;

  case 386: /* term: term MUL MINUS power  */
#line 816 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4645 "pascal.tab.c"
    break;

  case 387: /* term: term RDIV MINUS power  */
#line 817 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4651 "pascal.tab.c"
    break;

  case 388: /* term: term IDIV MINUS power  */
#line 818 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4657 "pascal.tab.c"
    break;

  case 389: /* term: term IMOD MINUS power  */
#line 819 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4663 "pascal.tab.c"
    break;

  case 390: /* term: term ANDOP MINUS power  */
#line 820 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4669 "pascal.tab.c"
    break;

  case 391: /* term: term SHIFTOP power  */
#line 821 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4675 "pascal.tab.c"
    break;

  case 392: /* term: term ASOP power  */
#line 822 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4681 "pascal.tab.c"
    break;

  case 393: /* power: factor  */
#line 825 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4687 "pascal.tab.c"
    break;

  case 394: /* power: power POWOP factor  */
#line 826 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4693 "pascal.tab.c"
    break;

  case 395: /* factor: selector  */
#line 829 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4699 "pascal.tab.c"
    break;

  case 396: /* factor: call_with_args  */
#line 830 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4705 "pascal.tab.c"
    break;

  case 397: /* factor: INTCONST  */
#line 831 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4711 "pascal.tab.c"
    break;

  case 398: /* factor: REALCONST  */
#line 832 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4717 "pascal.tab.c"
    break;

  case 399: /* factor: STRINGCONST  */
#line 833 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4723 "pascal.tab.c"
    break;

  case 400: /* factor: CHARCODE  */
#line 834 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4729 "pascal.tab.c"
    break;

  case 401: /* factor: LPARENT expression RPARENT  */
#line 835 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4735 "pascal.tab.c"
    break;

  case 402: /* factor: inherited_call  */
#line 836 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4741 "pascal.tab.c"
    break;

  case 403: /* factor: NOTSY factor  */
#line 837 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4747 "pascal.tab.c"
    break;

  case 404: /* factor: ATSIGN factor  */
#line 838 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4753 "pascal.tab.c"
    break;

  case 405: /* factor: LBRACK RBRACK  */
#line 839 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4759 "pascal.tab.c"
    break;

  case 406: /* factor: LBRACK set_member_list RBRACK  */
#line 840 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4765 "pascal.tab.c"
    break;

  case 407: /* set_member_list: set_member  */
#line 843 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4771 "pascal.tab.c"
    break;

  case 408: /* set_member_list: set_member_list COMMA set_member  */
#line 844 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4777 "pascal.tab.c"
    break;

  case 409: /* set_member: expression  */
#line 847 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4783 "pascal.tab.c"
    break;

  case 410: /* set_member: expression DOTDOT expression  */
#line 848 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4789 "pascal.tab.c"
    break;


#line 4793 "pascal.tab.c"

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
