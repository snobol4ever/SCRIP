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
  YYSYMBOL_pv_mark = 170,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 171,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 172,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 173,           /* parameter_decl  */
  YYSYMBOL_param_type = 174,               /* param_type  */
  YYSYMBOL_routine_directives = 175,       /* routine_directives  */
  YYSYMBOL_pf_params = 176,                /* pf_params  */
  YYSYMBOL_pf_sections = 177,              /* pf_sections  */
  YYSYMBOL_pf_section = 178,               /* pf_section  */
  YYSYMBOL_enum_list = 179,                /* enum_list  */
  YYSYMBOL_enum_member = 180,              /* enum_member  */
  YYSYMBOL_id_list = 181,                  /* id_list  */
  YYSYMBOL_body = 182,                     /* body  */
  YYSYMBOL_statement_list = 183,           /* statement_list  */
  YYSYMBOL_184_18 = 184,                   /* $@18  */
  YYSYMBOL_185_19 = 185,                   /* $@19  */
  YYSYMBOL_body_stmt = 186,                /* body_stmt  */
  YYSYMBOL_187_20 = 187,                   /* $@20  */
  YYSYMBOL_statement = 188,                /* statement  */
  YYSYMBOL_statement_no_label = 189,       /* statement_no_label  */
  YYSYMBOL_try_statement = 190,            /* try_statement  */
  YYSYMBOL_handler_list = 191,             /* handler_list  */
  YYSYMBOL_handler = 192,                  /* handler  */
  YYSYMBOL_raise_statement = 193,          /* raise_statement  */
  YYSYMBOL_inherited_call = 194,           /* inherited_call  */
  YYSYMBOL_call = 195,                     /* call  */
  YYSYMBOL_recv_primary = 196,             /* recv_primary  */
  YYSYMBOL_call_with_args = 197,           /* call_with_args  */
  YYSYMBOL_argument_list = 198,            /* argument_list  */
  YYSYMBOL_argument = 199,                 /* argument  */
  YYSYMBOL_assignment = 200,               /* assignment  */
  YYSYMBOL_selector = 201,                 /* selector  */
  YYSYMBOL_expression_list = 202,          /* expression_list  */
  YYSYMBOL_compound_statement = 203,       /* compound_statement  */
  YYSYMBOL_goto_statement = 204,           /* goto_statement  */
  YYSYMBOL_if_statement = 205,             /* if_statement  */
  YYSYMBOL_case_statement = 206,           /* case_statement  */
  YYSYMBOL_207_21 = 207,                   /* $@21  */
  YYSYMBOL_case_tail = 208,                /* case_tail  */
  YYSYMBOL_case_list = 209,                /* case_list  */
  YYSYMBOL_case_elem = 210,                /* case_elem  */
  YYSYMBOL_constant_list = 211,            /* constant_list  */
  YYSYMBOL_case_label = 212,               /* case_label  */
  YYSYMBOL_while_statement = 213,          /* while_statement  */
  YYSYMBOL_repeat_statement = 214,         /* repeat_statement  */
  YYSYMBOL_for_statement = 215,            /* for_statement  */
  YYSYMBOL_with_statement = 216,           /* with_statement  */
  YYSYMBOL_with_open = 217,                /* with_open  */
  YYSYMBOL_expression = 218,               /* expression  */
  YYSYMBOL_simple_expression = 219,        /* simple_expression  */
  YYSYMBOL_term = 220,                     /* term  */
  YYSYMBOL_power = 221,                    /* power  */
  YYSYMBOL_factor = 222,                   /* factor  */
  YYSYMBOL_set_member_list = 223,          /* set_member_list  */
  YYSYMBOL_set_member = 224                /* set_member  */
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
#define YYLAST   1709

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  127
/* YYNRULES -- Number of rules.  */
#define YYNRULES  394
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  927

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
     468,   469,   472,   475,   478,   479,   480,   483,   484,   487,
     488,   491,   492,   495,   496,   497,   500,   501,   502,   502,
     503,   503,   504,   504,   505,   505,   506,   506,   507,   507,
     508,   508,   509,   509,   510,   510,   513,   513,   516,   516,
     519,   519,   522,   522,   527,   530,   531,   532,   535,   536,
     539,   540,   541,   542,   543,   544,   545,   546,   549,   550,
     551,   552,   553,   556,   557,   567,   568,   569,   572,   573,
     576,   577,   578,   579,   580,   581,   584,   585,   588,   591,
     592,   595,   598,   598,   599,   599,   602,   602,   605,   606,
     607,   610,   611,   612,   613,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   627,   628,   629,   630,
     633,   634,   635,   638,   639,   642,   643,   646,   647,   648,
     651,   652,   653,   654,   657,   658,   659,   662,   663,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   676,
     677,   680,   681,   682,   683,   686,   687,   688,   689,   690,
     691,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     705,   706,   709,   712,   713,   716,   717,   720,   720,   723,
     724,   725,   728,   729,   732,   733,   736,   737,   740,   741,
     744,   747,   750,   751,   752,   755,   758,   759,   766,   767,
     768,   769,   770,   771,   772,   773,   774,   777,   778,   779,
     780,   781,   782,   783,   786,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   801,   802,   805,
     806,   807,   808,   809,   810,   811,   812,   813,   814,   815,
     816,   819,   820,   823,   824
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

#define YYPACT_NINF (-557)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-320)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     336,    -5,    72,   183,  -557,   162,   982,  -557,  -557,    11,
    -557,    59,  -557,  -557,   151,   202,    72,   337,   394,   394,
     397,  -557,   -29,   264,   411,   426,   355,  -557,  -557,  -557,
     429,   394,   443,   546,   537,  -557,  -557,    73,   372,   202,
    -557,   114,   475,   313,  -557,   394,  -557,   616,   117,   330,
      57,   863,   401,   551,   -29,  -557,   556,   560,   586,   597,
     587,   588,  -557,   387,  -557,  -557,  -557,  -557,   255,  1268,
     961,  -557,  -557,    72,   394,   595,   555,  -557,   619,   961,
    -557,   394,   625,    79,  -557,  -557,   292,   628,  -557,  1268,
     190,  1268,  -557,  1268,  1268,   655,  -557,  1040,  -557,  1268,
     650,   691,  -557,   591,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,   631,   585,  -557,  1097,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,  -557,   394,  1012,   705,  -557,   680,   703,   694,
     698,   737,   742,  -557,   745,   430,  -557,  -557,  1476,  1476,
    1476,  1051,  1268,  1476,  -557,  -557,  -557,   637,   140,   650,
     751,  -557,   569,   621,   165,   252,   467,   735,  -557,   752,
    -557,   752,   845,   845,   125,   741,  -557,  -557,  -557,  -557,
    1353,   756,    27,  -557,  -557,  -557,  -557,   768,   127,  -557,
     316,   329,   475,    91,  -557,   221,   818,  -557,    26,   237,
    -557,  -557,   764,   863,  -557,  -557,   356,   129,   189,   569,
     621,    77,  1001,   134,  1524,  1541,   891,   891,  1076,  1506,
      21,  1628,   761,   779,  1118,   758,  -557,  1268,   765,  -557,
    1268,  1143,  1268,   772,  -557,  1268,  1268,  1268,  1268,  1268,
     449,    81,   827,   394,   775,   776,  -557,  -557,   394,   394,
    -557,   202,    72,  -557,   778,   394,   781,   -29,  -557,  -557,
     467,   467,  -557,   730,   504,  -557,  1560,  -557,   782,  -557,
    1268,  1268,  1268,  1268,  1268,  1268,  1268,  1268,  1476,  1476,
    1476,  1476,  1359,  1384,  1409,  1434,  1459,  1476,  1476,  1476,
     312,   809,   780,   791,   329,   329,    42,  -557,  -557,   457,
    -557,  1506,   793,   789,   789,   281,   480,   795,   796,   797,
     801,   803,   851,   244,  -557,   651,  1293,   171,   857,   858,
    -557,   908,   908,   789,   908,   908,   908,   908,  -557,   555,
     394,   816,   394,   817,  -557,   394,    39,  -557,   658,   886,
    -557,  1293,   820,  -557,   555,  -557,   289,  -557,  1268,  1268,
    1268,  -557,  -557,   190,  -557,  -557,  -557,  -557,   932,  -557,
    -557,  -557,   519,  -557,   627,  1578,    84,  -557,   805,  -557,
    1168,  1506,  -557,   521,   835,  1628,   837,   596,  1628,  -557,
     522,  1628,  1629,   609,  1628,  1628,  1628,  1628,   849,  -557,
     281,  -557,   470,  -557,  -557,   555,   555,   481,   487,   202,
     122,   671,  -557,   394,  -557,   -29,  1268,  1268,  -557,   663,
     860,   252,   252,   252,   252,   252,   252,   252,   252,   467,
     467,   467,   467,  1476,   735,  1476,   735,  1476,   735,  1476,
     735,  1476,   735,   735,   735,  -557,   394,   850,   394,   853,
    -557,   394,    45,  -557,   667,   854,   289,   741,  -557,   501,
    -557,   908,   908,   789,   861,  -557,   500,   383,  -557,   427,
      42,   527,  -557,  -557,   866,  -557,   869,    38,   458,   870,
     878,   880,  -557,   672,  -557,  -557,   848,  -557,  -557,  -557,
      47,   881,  -557,  -557,  1318,   927,  1628,   961,  1506,   961,
    1506,    47,   383,   383,   547,  -557,  -557,  -557,  -557,   555,
     928,   677,   752,   682,   752,   686,   121,  -557,    16,   929,
     883,   955,   956,   555,   902,   959,    -3,  -557,   960,   856,
     370,  1628,  -557,   863,  -557,   621,   789,   933,  1268,  -557,
    1268,  1268,  1506,  -557,   892,   272,   154,  -557,   283,  -557,
     530,   523,  -557,  1193,  1218,  1268,  -557,  -557,  1243,  -557,
     961,   533,   916,   555,   555,   968,   972,  -557,  -557,  -557,
    -557,  -557,   970,   555,   555,  1628,  -557,   735,   735,   735,
     735,   735,   713,   752,   728,   752,   739,   293,  -557,    16,
    -557,   967,  -557,  -557,   383,   383,   454,   789,   908,   908,
    -557,   908,   908,   908,   908,  -557,   281,  -557,  -557,  -557,
    -557,   922,   930,  -557,  -557,   942,   961,   555,   555,    47,
     555,   555,  -557,   196,   961,   138,   947,    55,  1560,  -557,
    -557,   612,  -557,  -557,   977,  1004,  -557,    16,  -557,    16,
     950,    16,  -557,   983,   123,  -557,   973,  -557,  1007,  -557,
    -557,  -557,   953,   289,  -557,   944,  1018,  1268,  1268,  -557,
    -557,   392,   227,  -557,   740,  -557,  -557,  -557,  1594,  1613,
    -557,    37,  -557,   805,  -557,  -557,  -557,  -557,  -557,  -557,
     538,  -557,   539,  1628,  -557,   557,  1027,  1035,   961,  1037,
     985,  -557,  -557,   555,   555,  1041,  1041,  -557,  1017,  1050,
     997,    16,  -557,    16,   998,    16,  -557,  -557,   984,  -557,
     492,   383,   383,  -557,  -557,  -557,  -557,  -557,   800,   555,
     555,  -557,  -557,   555,   555,   121,   999,  1056,  -557,  1003,
    1038,  -557,  -557,  -557,  1005,  -557,  1293,  1293,   399,  -557,
    1506,  1046,  -557,  -557,   883,  1024,  -557,   289,  -557,    32,
    1013,  1268,  -557,  1074,   289,  1075,  -557,  1078,   883,   558,
     706,  -557,   789,   789,  -557,  -557,  1060,   789,  -557,  -557,
    1268,  1268,  -557,  1015,  -557,   321,  -557,  -557,  -557,  -557,
    -557,  1088,  -557,   289,   883,   883,  1091,  1043,  1041,  1039,
    -557,   289,  -557,  -557,   289,  -557,  -557,  -557,  -557,  1092,
    1052,   555,   555,  1111,  1112,    46,   281,  -557,   289,  -557,
     789,  1093,  -557,  -557,   559,  -557,  1062,  1057,  -557,   883,
    -557,  1268,  -557,  -557,  -557,  -557,  1628,  -557,  1123,  -557,
    -557,  -557,  -557,  -557,   547,  -557,  1041,  1041,  -557,  -557,
    -557,   231,  1628,  -557,  1115,  -557,  -557,   883,  1124,  -557,
    -557,  -557,   289,  -557,   883,  1128,  -557,  -557,   289,  1129,
    1077,  -557,  -557,  -557,  -557,  -557,  1137,  -557,   750,  -557,
    1293,  -557,  1293,  1116,  -557,  -557,  1140,  1628,  -557,   883,
    1142,  -557,  -557,  -557,  -557,  -557,  -557,  1148,  1149,   883,
    1150,  -557,  1152,  -557,   289,    36,   789,  1096,   789,  -557,
    -557,   961,   961,  1157,  -557,   883,  -557,  -557,  -557,  1158,
     883,  -557,  -557,  -557,  -557,   883,  -557,  1164,  -557,   417,
    -557,  -557,  1137,  -557,  -557,  -557,  -557,  1165,  -557,  -557,
    1169,   883,  -557,  -557,  -557,    47,  1170,  -557,  1171,  -557,
    -557,  1114,  -557,  -557,  1176,  -557,  -557
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   204,   240,    16,   172,     0,     0,   204,
       0,   265,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     7,     0,    12,    22,    65,    13,     0,     0,
     121,    38,    18,     0,     0,     0,   207,   171,     0,   121,
      19,     0,     0,   207,   242,   241,     0,     0,   244,     0,
       0,     0,   244,     0,     0,     0,   284,   280,   244,   275,
     264,     0,    68,   281,    67,   245,   248,   261,   262,   263,
     252,     0,   283,   251,     0,   253,   254,   255,   256,   257,
     258,   259,   260,     0,   121,     0,    55,     0,     0,     0,
       0,     0,     0,     8,     0,     0,    34,    35,     0,     0,
       0,     0,     0,     0,   381,   384,   382,   383,   319,     0,
     286,   386,   380,   379,     0,   348,   357,   364,   377,   227,
     120,   227,     0,     0,     0,     0,    51,    54,    52,    53,
     123,   103,   161,   133,    75,    74,   126,     0,     0,    79,
       0,   128,    63,     0,   204,     0,     0,   239,     0,     0,
     204,   182,     0,   265,   323,   324,     0,     0,     0,     0,
     347,     0,     0,     0,     0,     0,   265,   265,     0,     0,
       0,   276,   277,     0,     0,     0,   318,     0,     0,   316,
       0,     0,     0,     0,   313,     0,     0,     0,     0,     0,
       0,   103,     0,     0,     0,     0,   204,   204,     0,     0,
       4,     0,     0,    12,     0,     0,     0,     0,    21,   387,
     358,   359,   389,   393,     0,   391,     0,   388,     0,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    86,     0,    50,   129,   130,    80,    81,   238,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   166,   154,     0,     0,     0,    94,     0,
      91,     0,     0,     0,     0,     0,     0,     0,   204,   207,
       0,     0,     0,     0,   206,   240,     0,   209,     0,   223,
     173,     0,     0,   204,   207,    12,     0,   243,     0,     0,
       0,   246,   246,     0,   327,   322,   246,   285,   280,   249,
     250,   290,     0,   300,   379,   301,     0,    77,   244,   244,
       0,     0,   289,     0,   317,   310,   314,     0,   321,   292,
       0,   305,   282,     0,   306,   307,   308,   309,     0,    60,
       0,    57,     0,   204,   204,   207,   207,     0,     0,    24,
       0,    31,   204,    26,   204,    25,     0,     0,   390,   385,
     312,   349,   350,   351,   352,   353,   354,   355,   356,   362,
     360,   361,   363,     0,   367,     0,   368,     0,   369,     0,
     365,     0,   366,   375,   376,   378,     0,     0,     0,     0,
     226,   240,     0,   229,     0,     0,     0,     0,   122,     0,
     125,     0,     0,     0,    50,   149,     0,   139,   144,     0,
      71,     0,    69,    72,     0,    98,     0,   105,     0,     0,
       0,     0,   104,     0,   204,   204,     0,   204,   204,    96,
     161,     0,   152,   155,     0,     0,    42,   121,     0,   121,
       0,   161,   131,   132,   127,   136,   137,   135,   138,   207,
       0,     0,   227,     0,   227,     0,     0,   205,     0,     0,
     178,     0,     0,   207,     0,     0,   100,   102,     0,     0,
       0,   341,   340,   265,   345,   346,   335,   325,     0,   287,
       0,     0,     0,    66,     0,     0,     0,   270,     0,   278,
       0,     0,   288,     0,     0,     0,   315,   291,     0,   311,
     121,     0,     0,   207,   207,     0,     0,   204,   204,    23,
     244,   244,    33,   207,   207,   394,   392,   372,   373,   374,
     370,   371,     0,   227,     0,   227,     0,     0,   225,     0,
      87,    88,   236,    76,   140,   141,     0,     0,     0,     0,
     124,     0,     0,     0,     0,   134,     0,   103,   204,   204,
     106,     0,     0,   204,   204,   115,   121,   207,   207,   161,
     207,   207,   153,     0,   121,   319,     0,     0,    42,    41,
      85,     0,    95,    93,     0,     0,   223,     0,   210,     0,
       0,     0,   208,     0,   218,   222,   213,   176,     0,    12,
     174,   175,     0,     0,   183,     0,   223,     0,     0,   246,
     247,   338,   329,   333,     0,   337,   246,   299,   321,   302,
      78,     0,   266,   272,   267,   244,   269,   279,    73,   298,
       0,   296,     0,   320,   294,     0,     0,   103,   121,     0,
       0,   223,   223,   207,   207,    29,    30,   244,     0,     0,
       0,     0,   234,     0,     0,     0,   228,   230,     0,   151,
       0,   142,   143,   146,   147,   145,   148,    70,     0,   207,
     207,   204,   204,   207,   207,     0,     0,     0,   157,     0,
       0,   159,   160,   163,     0,   156,     0,     0,     0,    44,
       0,     0,    92,   223,   192,   214,   212,     0,   217,     0,
       0,     0,   224,     0,     0,     0,   101,     0,   180,     0,
       0,   344,     0,   335,   244,   244,     0,     0,   246,   326,
       0,     0,   246,     0,   271,     0,   297,   295,   293,    59,
      61,     0,   223,     0,   196,   198,     0,     0,    32,     0,
     223,     0,   232,   231,     0,   233,    89,   150,    99,     0,
       0,   207,   207,     0,     0,     0,     0,   107,     0,    97,
     170,     0,    48,    46,     0,    45,     0,     0,    82,   188,
      12,     0,   211,   221,   220,   219,   215,   179,     0,   223,
     177,    12,   246,   246,   339,   332,   331,   330,   328,   336,
     334,     0,   303,   274,     0,   268,    58,   200,     0,    12,
      12,   223,     0,    20,    27,     0,   235,   108,     0,     0,
       0,   110,   111,   116,   117,   158,   165,   168,     0,   163,
       0,    43,     0,     0,    83,    12,     0,   216,   223,   194,
       0,   343,   342,   304,   246,    12,   223,     0,     0,   184,
       0,   223,     0,   112,     0,     0,   170,     0,   170,    47,
      49,   121,   121,     0,   193,   190,    12,   181,   273,     0,
     202,   197,   199,    12,   223,    28,   109,     0,   114,     0,
     167,   162,   164,    90,    84,   189,    12,     0,   201,    12,
       0,   186,   113,   119,   118,   161,     0,   195,     0,   185,
      12,     0,   191,   203,     0,   169,   187
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -557,  -557,  -557,  -557,  -557,   -64,   941,  -557,  -557,  -557,
    -557,  -557,  -557,  -557,   946,   -37,  -320,  -557,  -557,   -63,
     948,   -40,   954,    40,   -69,   -30,   808,  -556,   -43,  -145,
    -256,   -75,  -557,  -557,  -557,  -388,  -553,  -557,  -557,  -557,
    -557,  -206,  -557,    88,  -281,  -274,  -248,  -473,  -557,   719,
    -557,  -557,   350,  -557,   322,   325,   957,   -39,  -557,  -557,
    -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,   476,   249,   498,   708,  -489,   105,  -132,  -557,
     642,  -557,   777,    70,  -557,   -83,  -557,  -557,  -322,  -557,
    -168,   543,  -557,  -557,   567,  -557,   -28,  -557,  -557,   -33,
    -175,   707,  -557,   135,   992,  -557,  -557,  -557,  -557,  -557,
    -557,  -557,   478,  -499,   477,  -557,  -557,  -557,  -557,  -557,
     -17,  1441,   -81,   -14,  -110,  -557,   829
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   135,
     248,   552,   678,    37,    39,    40,   475,   794,   607,   445,
      54,    55,    41,    42,   102,   150,   451,   452,   104,   176,
     356,   177,   854,   882,   481,   508,   296,   462,   706,   875,
     178,   179,   180,   181,   641,   447,   448,   302,   303,   304,
     604,   915,   790,   472,   846,   847,    45,    46,    28,   629,
     811,   335,   893,   920,   855,   906,   800,   886,   829,   830,
     865,   909,    76,   186,   326,   327,   626,   500,   281,   432,
     433,   289,   290,   305,    29,    50,   193,    51,   512,   513,
     105,   106,   107,   526,   527,   108,   151,   110,   111,   152,
     352,   353,   113,   153,   373,   115,   116,   117,   118,   516,
     746,   642,   643,   848,   645,   119,   120,   121,   122,   201,
     355,   155,   156,   157,   158,   254,   255
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     134,   174,    71,   357,   188,   197,    77,   173,   614,   203,
     174,   501,   446,   449,   126,   210,   173,   644,   112,   287,
     514,   103,   623,   109,   517,   337,    84,   175,   249,   282,
     697,   330,   484,   257,   698,   439,   175,   482,   483,   363,
     803,   898,    11,   590,   496,   291,   370,    52,   571,   232,
     567,   496,   154,   297,   752,   174,   635,   199,   250,   251,
     718,   173,    84,   298,    33,    53,   485,   486,   487,   488,
      30,     8,   198,   297,   202,    31,   204,   205,    67,   331,
     687,   175,   211,   298,   191,   357,   379,    85,    47,    48,
     291,   299,   624,   209,   342,   174,  -240,   753,  -240,   173,
     173,    63,   332,    44,   497,   531,   358,   359,   804,   843,
     568,   101,   899,   182,   698,    47,   300,   301,    34,    72,
     719,   175,    80,    44,   253,   256,   710,   549,   725,   320,
     726,    68,   728,   307,    84,   343,   300,   301,   209,    84,
     174,   318,   522,   185,   183,   380,   173,   321,    10,    78,
     453,   189,   340,   322,   606,   357,   308,   323,   309,   653,
     112,   310,   576,   103,   345,   109,   175,   574,   575,   425,
     259,   291,    73,   112,   112,    78,   103,   103,   109,   109,
      73,   523,   730,    12,   654,   530,   114,   409,   410,   411,
     412,   507,   772,   230,   773,   655,   775,   325,   716,   477,
     365,   286,   208,   368,   208,   371,   341,   260,   368,   374,
     375,   376,   377,   261,   262,   263,   264,   265,   266,   209,
     101,    13,   174,    35,   713,   200,   174,    36,   173,   320,
     844,   260,   743,   478,   209,   453,   209,   261,   262,   263,
     264,   265,   266,   744,   644,   735,   267,   321,   175,   470,
     284,   285,   175,   322,    94,   328,   714,   323,   414,   416,
     418,   420,   422,   423,   424,    96,   148,   174,   745,   471,
     267,   505,   611,   260,   613,   525,   528,    84,    38,   261,
     262,   263,   264,   265,   266,   101,   324,   333,    84,   476,
      56,   507,   174,   175,   863,    78,   690,   325,   173,   268,
      57,   426,   652,   382,   691,   692,   269,   270,   387,   388,
     199,   174,   267,   656,   476,    47,   650,   741,   175,   427,
     426,   509,   510,   511,   749,   428,    84,   136,   114,   429,
     271,   137,   192,   693,   694,   695,   696,   175,   427,   802,
       1,   114,   114,   354,   428,   640,   808,     2,   429,   354,
     434,   825,    71,   625,    77,   126,   354,   450,   660,   662,
     618,    74,   620,   665,   194,   506,   463,   174,   195,   431,
     311,   312,    75,   314,   315,   828,   101,   430,    81,   555,
     253,    60,   313,   835,   101,   316,   836,   639,   431,    82,
     491,    61,   493,   175,   338,   495,   792,   793,   339,   557,
     845,   558,   610,   559,   612,   560,   317,   561,   174,   174,
     174,   174,   260,    43,   173,   173,   173,   173,   261,   262,
     263,   264,   265,   266,   625,    69,   820,   581,   582,   174,
     823,   682,    70,   684,   175,   175,   175,   175,   241,   583,
     242,   453,   921,   243,   870,    78,   578,   579,    64,   123,
     872,   267,   133,   174,   124,   175,   244,   608,   742,   173,
     584,   814,   245,    47,   795,   666,   246,   675,   676,   247,
      44,   174,   625,    49,   625,   796,   625,   173,   515,   175,
     112,   578,   579,   103,   591,   109,   897,    58,   507,   913,
     861,   862,   585,   914,   592,   354,   562,   175,   564,   378,
     174,   566,    59,   648,   649,    62,   454,    78,   578,   579,
     455,   272,   273,   274,   797,   437,   456,   174,   663,   689,
     542,   707,   438,   275,   276,    83,   175,   174,    78,   715,
     879,   547,   880,   173,    34,   174,   625,   548,   625,    78,
     625,   173,   888,   175,   277,    78,   578,   579,   174,   278,
     174,   573,   174,   175,   578,   579,   457,   777,    65,   522,
     458,   175,   397,   580,   174,   733,   328,   398,   490,   459,
     460,   461,   755,   658,   175,   812,   175,   518,   175,   518,
     518,   522,   507,   504,   519,   586,   532,   537,   518,   507,
     175,   586,   587,   761,   768,   657,   518,   518,   667,   174,
     260,   578,   579,   756,   757,   173,   261,   262,   263,   264,
     265,   266,   174,    66,   174,   518,   174,   850,   507,   185,
     739,   740,   758,   217,   851,   175,   507,   125,   218,   507,
     219,   220,   127,   221,   545,   546,   128,   434,   175,   267,
     175,   453,   175,   507,   218,   129,   219,   220,   114,   221,
    -286,   174,  -286,   354,   535,   214,   130,   173,   174,   536,
     319,   816,   817,   131,   132,   174,   334,   535,   354,   354,
     720,   184,   539,   354,    78,   721,    79,   175,    14,    15,
     258,    16,   224,   225,   175,   550,   258,   507,   224,   520,
     215,   175,   216,   507,   174,   187,  -284,    17,  -284,   476,
     476,   190,   174,    18,   196,   174,    19,    20,   551,    78,
      22,   473,   385,   386,   806,   206,    78,   174,   498,   174,
     175,   724,  -285,   813,  -285,    78,   212,   569,   175,   507,
      78,   175,   596,   821,   822,    78,   856,   617,   615,   234,
      78,   738,   619,   175,    78,   175,   621,   860,   260,   349,
     350,    23,   632,   233,   261,   262,   263,   264,   265,   266,
      24,    25,   235,   174,    26,   867,   868,   213,   463,   174,
     236,    78,   260,   681,   237,   328,   764,   765,   261,   262,
     263,   264,   265,   266,   857,   238,    78,   267,   683,   175,
     239,   883,   669,   670,   489,   175,   396,    78,   747,   685,
     748,   889,   679,   680,   240,   174,   903,   904,   747,   503,
     877,   267,   174,   174,   279,   214,   280,   288,   173,   173,
     295,   306,   907,   329,   336,   360,   454,   361,   799,   910,
     778,   175,   381,   476,   364,   476,   456,   435,   175,   175,
     436,   366,   916,   441,   442,   918,   708,   709,   372,   711,
     712,   383,   384,   443,   392,   294,   924,   394,   400,   543,
     544,   166,   167,   168,   169,   444,    86,   827,   553,   440,
     554,   464,   465,   466,   637,   834,   457,   467,   638,   468,
     458,   469,    87,    88,    89,   479,   480,    90,    91,   459,
     460,   461,   492,   494,    86,   499,   502,   524,   260,   533,
      92,   534,   540,    93,   261,   262,   263,   264,   265,   266,
      87,    88,    89,   599,   859,    90,    91,   166,   167,   168,
     169,   283,   766,   767,   538,   577,   563,    94,    92,   565,
     570,    93,   609,   616,   627,    95,   869,   267,    96,    97,
     597,   598,   588,   600,   601,   589,   593,    98,   779,   780,
      99,   100,   783,   784,   594,    94,   595,   603,   101,   628,
     630,   631,   633,   885,   634,   636,    96,   348,   651,   668,
    -319,   890,   443,   671,   646,    98,   895,   672,    99,   100,
     166,   167,   168,   169,   444,   677,   101,   159,   160,    14,
      15,  -319,    16,  -319,  -319,   688,   208,   161,   701,   911,
    -319,  -319,  -319,  -319,   705,   717,   702,   722,    17,   723,
     727,   729,   732,   734,    18,   162,   163,    19,    20,    21,
     736,    22,   164,   673,   674,   165,   731,   737,   209,   344,
     839,   840,   759,   166,   167,   168,   169,   170,   159,   160,
     760,   171,   762,   260,   172,   763,    84,   769,   161,   261,
     262,   263,   264,   265,   266,   770,   101,   771,   774,   786,
     776,   787,    23,   788,   699,   700,   162,   163,   789,   703,
     704,    24,    25,   164,   798,    26,   165,   801,  -319,   807,
     809,   791,   267,   810,   166,   167,   168,   169,   170,   805,
     818,   824,   231,   826,   138,   172,   831,   837,   833,  -319,
     207,  -319,  -319,   832,   208,   139,   140,   101,  -319,  -319,
    -319,  -319,   838,   141,   252,   142,   841,   842,   143,   138,
     853,   849,   852,   144,   145,   146,   147,   148,   858,   866,
     139,   140,   864,   871,   873,   222,   209,   874,   141,   149,
     142,   351,   876,   143,   881,   884,   101,   887,   144,   145,
     146,   147,   148,   891,   892,   894,   223,   896,   224,   225,
     901,   138,   905,   908,   149,   226,   227,   228,   229,   912,
     917,   101,   139,   140,   919,   922,   923,   781,   782,   925,
     141,   926,   142,   362,   391,   143,   138,   389,   541,   602,
     144,   145,   146,   147,   148,   395,   390,   139,   140,   878,
     902,   900,   393,   785,   622,   141,   149,   142,   369,   686,
     143,   138,   367,   101,   572,   144,   145,   146,   147,   148,
     754,   815,   139,   140,   819,   647,   556,     0,     0,     0,
     141,   149,   142,   529,     0,   143,   138,     0,   101,     0,
     144,   145,   146,   147,   148,     0,     0,   139,   140,     0,
       0,     0,     0,     0,     0,   141,   149,   142,   659,     0,
     143,   138,     0,   101,     0,   144,   145,   146,   147,   148,
       0,     0,   139,   140,     0,     0,     0,     0,     0,     0,
     141,   149,   142,   661,     0,   143,   138,     0,   101,     0,
     144,   145,   146,   147,   148,     0,     0,   139,   140,     0,
       0,     0,     0,     0,     0,   141,   149,   142,   664,     0,
     143,   138,     0,   101,     0,   144,   145,   146,   147,   148,
       0,     0,   139,   140,     0,     0,     0,     0,     0,     0,
     141,   149,   142,     0,     0,   143,   138,     0,   101,     0,
     144,   145,   146,   147,   148,     0,     0,   139,   140,     0,
       0,     0,     0,     0,     0,   141,   149,   474,     0,     0,
     143,   138,     0,   101,     0,   144,   145,   146,   147,   148,
       0,     0,   139,   140,     0,     0,     0,     0,     0,     0,
     141,   149,   474,     0,     0,   143,     0,     0,   101,     0,
     144,   145,   146,   147,   605,     0,     0,   -50,   -50,     0,
       0,   291,   138,     0,     0,     0,   149,   -50,   -50,   -50,
       0,     0,   292,   101,   413,   293,     0,   294,     0,   -50,
       0,   141,     0,   142,     0,     0,   143,   138,     0,     0,
     -50,   144,   145,   146,   147,   148,     0,     0,     0,   415,
       0,     0,     0,     0,     0,     0,   141,   149,   142,   209,
       0,   143,   138,     0,   101,     0,   144,   145,   146,   147,
     148,     0,     0,     0,   417,     0,     0,     0,     0,     0,
       0,   141,   149,   142,     0,     0,   143,   138,     0,   101,
       0,   144,   145,   146,   147,   148,     0,     0,     0,   419,
       0,     0,     0,     0,     0,     0,   141,   149,   142,     0,
       0,   143,   138,     0,   101,     0,   144,   145,   146,   147,
     148,     0,     0,     0,   421,     0,     0,     0,     0,   138,
       0,   141,   149,   142,     0,     0,   143,     0,     0,   101,
       0,   144,   145,   146,   147,   148,     0,     0,   141,     0,
     142,     0,     0,   143,     0,     0,     0,   149,   144,   145,
     146,   147,   148,     0,   101,     0,     0,   346,     0,     0,
     162,   163,     0,     0,   149,     0,   260,     0,     0,     0,
     165,   101,   261,   262,   263,   264,   265,   266,   166,   167,
     168,   169,   170,   260,     0,     0,     0,     0,     0,   261,
     262,   263,   264,   265,   266,     0,     0,     0,     0,     0,
       0,   101,   260,     0,     0,   267,   347,     0,   261,   262,
     263,   264,   265,   266,     0,     0,     0,     0,     0,     0,
     260,     0,   267,     0,     0,   399,   261,   262,   263,   264,
     265,   266,     0,     0,     0,     0,   260,     0,   521,     0,
       0,   267,   261,   262,   263,   264,   265,   266,     0,     0,
       0,     0,     0,     0,     0,   260,     0,     0,     0,   267,
     750,   261,   262,   263,   264,   265,   266,  -312,     0,     0,
     260,     0,     0,   751,     0,   267,   261,   262,   263,   264,
     265,   266,     0,     0,     0,     0,     0,     0,  -312,     0,
    -312,  -312,     0,   538,   267,     0,     0,  -312,  -312,  -312,
    -312,   401,   402,   403,   404,   405,   406,   407,   408,   267
};

static const yytype_int16 yycheck[] =
{
      64,    70,    39,   209,    79,    88,    45,    70,   481,    92,
      79,   331,   293,   294,    54,    98,    79,   516,    51,   164,
     342,    51,     6,    51,   346,   193,     5,    70,   138,   161,
     586,     5,   313,   143,   587,   291,    79,   311,   312,   214,
       8,     5,     2,     5,     5,    48,   221,    76,   436,   124,
       5,     5,    69,    26,    17,   124,    59,    90,   139,   140,
       5,   124,     5,    36,     5,    94,   314,   315,   316,   317,
      59,    76,    89,    26,    91,    64,    93,    94,     5,    53,
     569,   124,    99,    36,     5,   291,     5,    30,    18,    19,
      48,    64,    76,    96,    17,   164,    58,    60,    60,   162,
     163,    31,    76,    76,    65,   361,    85,    86,    76,    63,
      65,    95,    76,    73,   667,    45,    89,    90,    59,     5,
      65,   164,     5,    76,   141,   142,   599,     5,   617,     8,
     619,    58,   621,     6,     5,    58,    89,    90,    96,     5,
     209,    50,    58,    64,    74,    64,   209,    26,    76,    58,
     295,    81,    23,    32,   474,   361,    29,    36,    31,     5,
     193,    34,   443,   193,    30,   193,   209,   441,   442,   279,
       5,    48,    58,   206,   207,    58,   206,   207,   206,   207,
      58,    97,    59,     0,    30,   360,    51,   268,   269,   270,
     271,   336,   681,   123,   683,    41,   685,    76,    60,    28,
     217,    76,    64,   220,    64,   222,    17,    42,   225,   226,
     227,   228,   229,    48,    49,    50,    51,    52,    53,    96,
      95,    59,   291,    72,    28,    90,   295,    76,   291,     8,
     786,    42,     5,    62,    96,   380,    96,    48,    49,    50,
      51,    52,    53,    16,   743,   633,    81,    26,   291,     5,
     162,   163,   295,    32,    64,   185,    60,    36,   272,   273,
     274,   275,   276,   277,   278,    75,    76,   336,    41,    25,
      81,   335,   478,    42,   480,   358,   359,     5,    76,    48,
      49,    50,    51,    52,    53,    95,    65,    50,     5,   306,
      26,   436,   361,   336,    63,    58,   577,    76,   361,    47,
      36,     8,    30,   233,   578,   579,    54,    55,   238,   239,
     343,   380,    81,    30,   331,   245,   522,   639,   361,    26,
       8,   338,   339,   340,   646,    32,     5,    72,   193,    36,
      78,    76,    83,   581,   582,   583,   584,   380,    26,   727,
       4,   206,   207,   208,    32,   513,   734,    11,    36,   214,
     280,    30,   389,   498,   393,   395,   221,    76,   533,   534,
     492,    48,   494,   538,    72,    76,   296,   436,    76,    76,
      54,    55,    59,    44,    45,   763,    95,    65,    48,   396,
     397,    26,    66,   771,    95,    56,   774,    17,    76,    59,
     320,    36,   322,   436,    38,   325,   716,   717,    42,   413,
     788,   415,   477,   417,   479,   419,    77,   421,   477,   478,
     479,   480,    42,    76,   477,   478,   479,   480,    48,    49,
      50,    51,    52,    53,   569,    53,   748,    44,    45,   498,
     752,   563,    60,   565,   477,   478,   479,   480,     8,    56,
      10,   586,   915,    13,   832,    58,    54,    55,     5,    48,
     838,    81,    65,   522,    53,   498,    26,   474,    66,   522,
      77,   742,    32,   393,    65,   540,    36,   550,   551,    39,
      76,   540,   617,    76,   619,    76,   621,   540,   343,   522,
     513,    54,    55,   513,    26,   513,   874,    76,   633,    72,
     812,   813,    65,    76,    36,   360,   426,   540,   428,    50,
     569,   431,    76,   520,   521,    76,    26,    58,    54,    55,
      30,    44,    45,    46,   720,    58,    36,   586,   535,    65,
      50,   596,    65,    56,    57,    49,   569,   596,    58,   604,
     850,    50,   852,   596,    59,   604,   681,    50,   683,    58,
     685,   604,   864,   586,    77,    58,    54,    55,   617,    82,
     619,    50,   621,   596,    54,    55,    76,    65,    12,    58,
      80,   604,    58,    63,   633,   629,   496,    63,   319,    89,
      90,    91,   655,    50,   617,    17,   619,    58,   621,    58,
      58,    58,   727,   334,    65,    58,    65,    65,    58,   734,
     633,    58,    65,   668,   677,    65,    58,    58,    65,   668,
      42,    54,    55,    65,    65,   668,    48,    49,    50,    51,
      52,    53,   681,    76,   683,    58,   685,    58,   763,    64,
     637,   638,    65,    38,    65,   668,   771,    76,    59,   774,
      61,    62,    76,    64,   385,   386,    76,   567,   681,    81,
     683,   786,   685,   788,    59,    59,    61,    62,   513,    64,
      59,   720,    61,   518,    58,    64,    59,   720,   727,    63,
     184,   744,   745,    76,    76,   734,   190,    58,   533,   534,
      58,    76,    63,   538,    58,    63,    60,   720,     7,     8,
      59,    10,    61,    62,   727,    14,    59,   832,    61,    62,
      59,   734,    61,   838,   763,    76,    59,    26,    61,   716,
     717,    76,   771,    32,    76,   774,    35,    36,    37,    58,
      39,    60,   236,   237,   731,    60,    58,   786,    60,   788,
     763,   616,    59,    17,    61,    58,    76,    60,   771,   874,
      58,   774,    60,   750,   751,    58,   800,    60,   489,    59,
      58,   636,    60,   786,    58,   788,    60,   811,    42,   206,
     207,    80,   503,    48,    48,    49,    50,    51,    52,    53,
      89,    90,    59,   832,    93,   829,   830,    76,   698,   838,
      76,    58,    42,    60,    76,   705,   671,   672,    48,    49,
      50,    51,    52,    53,   801,    48,    58,    81,    60,   832,
      48,   855,   543,   544,   318,   838,    66,    58,    58,    60,
      60,   865,   553,   554,    59,   874,   881,   882,    58,   333,
      60,    81,   881,   882,    79,    64,    64,    76,   881,   882,
      64,    53,   886,     5,    60,    64,    26,    48,   723,   893,
      30,   874,     5,   850,    76,   852,    36,    28,   881,   882,
      60,    76,   906,    54,    55,   909,   597,   598,    76,   600,
     601,    76,    76,    64,    76,    64,   920,    76,    76,   383,
     384,    72,    73,    74,    75,    76,     3,   762,   392,    76,
     394,    76,    76,    76,    18,   770,    76,    76,    22,    76,
      80,    30,    19,    20,    21,    28,    28,    24,    25,    89,
      90,    91,    76,    76,     3,     9,    76,    92,    42,    64,
      37,    64,    53,    40,    48,    49,    50,    51,    52,    53,
      19,    20,    21,    65,   809,    24,    25,    72,    73,    74,
      75,    76,   673,   674,    64,    64,    76,    64,    37,    76,
      76,    40,     5,     5,     5,    72,   831,    81,    75,    76,
     464,   465,    76,   467,   468,    76,    76,    84,   699,   700,
      87,    88,   703,   704,    76,    64,    76,    76,    95,    76,
       5,     5,    60,   858,     5,     5,    75,    76,    76,    53,
      38,   866,    64,     5,    41,    84,   871,     5,    87,    88,
      72,    73,    74,    75,    76,    15,    95,    26,    27,     7,
       8,    59,    10,    61,    62,    28,    64,    36,    76,   894,
      68,    69,    70,    71,    62,    58,    76,    30,    26,     5,
      60,    28,     5,    60,    32,    54,    55,    35,    36,    37,
      76,    39,    61,   547,   548,    64,    53,     9,    96,    28,
     781,   782,     5,    72,    73,    74,    75,    76,    26,    27,
       5,    80,     5,    42,    83,    60,     5,    30,    36,    48,
      49,    50,    51,    52,    53,     5,    95,    60,    60,    60,
      76,     5,    80,    60,   588,   589,    54,    55,    30,   593,
     594,    89,    90,    61,    28,    93,    64,    53,    38,     5,
       5,    76,    81,     5,    72,    73,    74,    75,    76,    76,
      30,    76,    80,     5,    43,    83,     5,     5,    59,    59,
      60,    61,    62,    60,    64,    54,    55,    95,    68,    69,
      70,    71,    60,    62,    63,    64,     5,     5,    67,    43,
      63,    28,    60,    72,    73,    74,    75,    76,     5,     5,
      54,    55,    17,     5,     5,    38,    96,    60,    62,    88,
      64,    65,     5,    67,    28,     5,    95,     5,    72,    73,
      74,    75,    76,     5,     5,     5,    59,     5,    61,    62,
      64,    43,     5,     5,    88,    68,    69,    70,    71,     5,
       5,    95,    54,    55,     5,     5,     5,   701,   702,    65,
      62,     5,    64,    65,   243,    67,    43,   241,   380,   470,
      72,    73,    74,    75,    76,   247,   242,    54,    55,   849,
     878,   876,   245,   705,   496,    62,    88,    64,    65,   567,
      67,    43,   220,    95,   437,    72,    73,    74,    75,    76,
     653,   743,    54,    55,   747,   518,   397,    -1,    -1,    -1,
      62,    88,    64,    65,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    65,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    65,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    -1,    -1,    67,    43,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    88,    64,    -1,    -1,
      67,    43,    -1,    95,    -1,    72,    73,    74,    75,    76,
      -1,    -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      62,    88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,
      72,    73,    74,    75,    76,    -1,    -1,    44,    45,    -1,
      -1,    48,    43,    -1,    -1,    -1,    88,    54,    55,    56,
      -1,    -1,    59,    95,    55,    62,    -1,    64,    -1,    66,
      -1,    62,    -1,    64,    -1,    -1,    67,    43,    -1,    -1,
      77,    72,    73,    74,    75,    76,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    96,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,    -1,
      -1,    67,    43,    -1,    95,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    43,
      -1,    62,    88,    64,    -1,    -1,    67,    -1,    -1,    95,
      -1,    72,    73,    74,    75,    76,    -1,    -1,    62,    -1,
      64,    -1,    -1,    67,    -1,    -1,    -1,    88,    72,    73,
      74,    75,    76,    -1,    95,    -1,    -1,    33,    -1,    -1,
      54,    55,    -1,    -1,    88,    -1,    42,    -1,    -1,    -1,
      64,    95,    48,    49,    50,    51,    52,    53,    72,    73,
      74,    75,    76,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    42,    -1,    -1,    81,    65,    -1,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      42,    -1,    81,    -1,    -1,    65,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    42,    -1,    60,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    81,
      66,    48,    49,    50,    51,    52,    53,    38,    -1,    -1,
      42,    -1,    -1,    60,    -1,    81,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,
      61,    62,    -1,    64,    81,    -1,    -1,    68,    69,    70,
      71,   260,   261,   262,   263,   264,   265,   266,   267,    81
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    99,   100,   103,   104,   106,    76,   101,
      76,   121,     0,    59,     7,     8,    10,    26,    32,    35,
      36,    37,    39,    80,    89,    90,    93,   105,   156,   182,
      59,    64,   102,     5,    59,    72,    76,   111,    76,   112,
     113,   120,   121,    76,    76,   154,   155,   181,   181,    76,
     183,   185,    76,    94,   118,   119,    26,    36,    76,    76,
      26,    36,    76,   181,     5,    12,    76,     5,    58,    53,
      60,   113,     5,    58,    48,    59,   170,   155,    58,    60,
       5,    48,    59,   170,     5,    30,     3,    19,    20,    21,
      24,    25,    37,    40,    64,    72,    75,    76,    84,    87,
      88,    95,   122,   123,   126,   188,   189,   190,   193,   194,
     195,   196,   197,   200,   201,   203,   204,   205,   206,   213,
     214,   215,   216,    48,    53,    76,   119,    76,    76,    59,
      59,    76,    76,    65,   103,   107,    72,    76,    43,    54,
      55,    62,    64,    67,    72,    73,    74,    75,    76,    88,
     123,   194,   197,   201,   218,   219,   220,   221,   222,    26,
      27,    36,    54,    55,    61,    64,    72,    73,    74,    75,
      76,    80,    83,   117,   122,   126,   127,   129,   138,   139,
     140,   141,   121,   181,    76,    64,   171,    76,   129,   181,
      76,     5,   171,   184,    72,    76,    76,   183,   218,   197,
     201,   217,   218,   183,   218,   218,    60,    60,    64,    96,
     183,   218,    76,    76,    64,    59,    61,    38,    59,    61,
      62,    64,    38,    59,    61,    62,    68,    69,    70,    71,
     181,    80,   129,    48,    59,    59,    76,    76,    48,    48,
      59,     8,    10,    13,    26,    32,    36,    39,   108,   222,
     220,   220,    63,   218,   223,   224,   218,   222,    59,     5,
      42,    48,    49,    50,    51,    52,    53,    81,    47,    54,
      55,    78,    44,    45,    46,    56,    57,    77,    82,    79,
      64,   176,   176,    76,   141,   141,    76,   127,    76,   179,
     180,    48,    59,    62,    64,    64,   134,    26,    36,    64,
      89,    90,   145,   146,   147,   181,    53,     6,    29,    31,
      34,    54,    55,    66,    44,    45,    56,    77,    50,   170,
       8,    26,    32,    36,    65,    76,   172,   173,   181,     5,
       5,    53,    76,    50,   170,   159,    60,   188,    38,    42,
      23,    17,    17,    58,    28,    30,    33,    65,    76,   189,
     189,    65,   198,   199,   201,   218,   128,   139,    85,    86,
      64,    48,    65,   198,    76,   218,    76,   202,   218,    65,
     198,   218,    76,   202,   218,   218,   218,   218,    50,     5,
      64,     5,   181,    76,    76,   170,   170,   181,   181,   112,
     120,   104,    76,   154,    76,   118,    66,    58,    63,    65,
      76,   219,   219,   219,   219,   219,   219,   219,   219,   220,
     220,   220,   220,    55,   221,    55,   221,    55,   221,    55,
     221,    55,   221,   221,   221,   222,     8,    26,    32,    36,
      65,    76,   177,   178,   181,    28,    60,    58,    65,   128,
      76,    54,    55,    64,    76,   117,   142,   143,   144,   142,
      76,   124,   125,   127,    26,    30,    36,    76,    80,    89,
      90,    91,   135,   181,    76,    76,    76,    76,    76,    30,
       5,    25,   151,    60,    64,   114,   218,    28,    62,    28,
      28,   132,   143,   143,   142,   144,   144,   144,   144,   170,
     171,   181,    76,   181,    76,   181,     5,    65,    60,     9,
     175,   114,    76,   170,   171,   103,    76,   127,   133,   218,
     218,   218,   186,   187,   186,   201,   207,   186,    58,    65,
      62,    60,    58,    97,    92,   183,   191,   192,   183,    65,
     198,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   170,   170,   171,   171,    50,    50,     5,
      14,    37,   109,   170,   170,   218,   224,   221,   221,   221,
     221,   221,   181,    76,   181,    76,   181,     5,    65,    60,
      76,   133,   180,    50,   143,   143,   142,    64,    54,    55,
      63,    44,    45,    56,    77,    65,    58,    65,    76,    76,
       5,    26,    36,    76,    76,    76,    60,   170,   170,    65,
     170,   170,   147,    76,   148,    76,   114,   116,   218,     5,
     129,   139,   129,   139,   145,   171,     5,    60,   176,    60,
     176,    60,   173,     6,    76,   127,   174,     5,    76,   157,
       5,     5,   171,    60,     5,    59,     5,    18,    22,    17,
     188,   142,   209,   210,   211,   212,    41,   199,   218,   218,
     139,    76,    30,     5,    30,    41,    30,    65,    50,    65,
     198,    65,   198,   218,    65,   198,   129,    65,    53,   171,
     171,     5,     5,   170,   170,   183,   183,    15,   110,   171,
     171,    60,   176,    60,   176,    60,   178,   174,    28,    65,
     142,   143,   143,   144,   144,   144,   144,   125,   134,   170,
     170,    76,    76,   170,   170,    62,   136,   129,   171,   171,
     145,   171,   171,    28,    60,   129,    60,    58,     5,    65,
      58,    63,    30,     5,   175,   174,   174,    60,   174,    28,
      59,    53,     5,   103,    60,   133,    76,     9,   175,   218,
     218,   186,    66,     5,    16,    41,   208,    58,    60,   186,
      66,    60,    17,    60,   192,   183,    65,    65,    65,     5,
       5,   129,     5,    60,   175,   175,   171,   171,   183,    30,
       5,    60,   174,   174,    60,   174,    76,    65,    30,   171,
     171,   170,   170,   171,   171,   172,    60,     5,    60,    30,
     150,    76,   114,   114,   115,    65,    76,   139,    28,   175,
     164,    53,   133,     8,    76,    76,   218,     5,   133,     5,
       5,   158,    17,    17,   142,   210,   183,   183,    30,   212,
     186,   218,   218,   186,    76,    30,     5,   175,   133,   166,
     167,     5,    60,    59,   175,   133,   133,     5,    60,   171,
     171,     5,     5,    63,   125,   133,   152,   153,   211,    28,
      58,    65,    60,    63,   130,   162,   103,   218,     5,   175,
     103,   186,   186,    63,    17,   168,     5,   103,   103,   175,
     133,     5,   133,     5,    60,   137,     5,    60,   150,   114,
     114,    28,   131,   103,     5,   175,   165,     5,   186,   103,
     175,     5,     5,   160,     5,   175,     5,   133,     5,    76,
     153,    64,   152,   129,   129,     5,   163,   103,     5,   169,
     103,   175,     5,    72,    76,   149,   103,     5,   103,     5,
     161,   145,     5,     5,   103,    65,     5
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
     147,   147,   149,   150,   151,   151,   151,   152,   152,   153,
     153,   154,   154,   155,   155,   155,   156,   156,   157,   156,
     158,   156,   159,   156,   160,   156,   161,   156,   162,   156,
     163,   156,   164,   156,   165,   156,   166,   156,   167,   156,
     168,   156,   169,   156,   170,   171,   171,   171,   172,   172,
     173,   173,   173,   173,   173,   173,   173,   173,   174,   174,
     174,   174,   174,   175,   175,   176,   176,   176,   177,   177,
     178,   178,   178,   178,   178,   178,   179,   179,   180,   181,
     181,   182,   184,   183,   185,   183,   187,   186,   188,   188,
     188,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   190,   190,   190,   190,
     191,   191,   191,   192,   192,   193,   193,   194,   194,   194,
     195,   195,   195,   195,   196,   196,   196,   197,   197,   197,
     197,   197,   197,   197,   197,   197,   197,   197,   197,   198,
     198,   199,   199,   199,   199,   200,   200,   200,   200,   200,
     200,   201,   201,   201,   201,   201,   201,   201,   201,   201,
     202,   202,   203,   204,   204,   205,   205,   207,   206,   208,
     208,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   214,   215,   215,   215,   216,   217,   217,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   219,   219,   219,
     219,   219,   219,   219,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   221,   221,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   223,   223,   224,   224
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
       4,     0,     0,     0,     7,     5,     0,     3,     1,     6,
       0,     2,     1,     4,     6,     6,     7,     9,     0,     9,
       0,    11,     0,     7,     0,    13,     0,    15,     0,    12,
       0,    14,     0,    11,     0,    13,     0,    11,     0,    11,
       0,    12,     0,    14,     0,     3,     2,     0,     3,     1,
       3,     5,     4,     3,     4,     5,     6,     4,     1,     3,
       3,     3,     1,     0,     3,     3,     2,     0,     3,     1,
       3,     4,     4,     4,     3,     5,     3,     1,     1,     3,
       1,     3,     0,     4,     0,     2,     0,     2,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     5,     5,     7,     5,
       1,     3,     2,     6,     4,     1,     2,     2,     4,     5,
       1,     1,     3,     1,     1,     3,     1,     4,     4,     3,
       3,     4,     3,     6,     5,     6,     5,     6,     5,     3,
       1,     1,     3,     5,     6,     3,     3,     3,     3,     3,
       3,     4,     3,     2,     3,     4,     2,     3,     2,     1,
       3,     1,     3,     2,     2,     4,     6,     0,     7,     0,
       2,     2,     3,     1,     3,     0,     3,     1,     1,     3,
       4,     4,     8,     8,     6,     4,     3,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       3,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       4,     4,     4,     4,     4,     3,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     3,     1,     2,     2,     2,
       3,     1,     3,     1,     3
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
#line 2249 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2255 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2261 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2267 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2273 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2279 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2285 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2291 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2297 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2303 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2309 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2315 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2321 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2327 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2333 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2339 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2345 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2351 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2357 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2363 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2369 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2375 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2381 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2387 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2393 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2399 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2417 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2423 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2429 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2435 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2441 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2447 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2465 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2477 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2489 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2495 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2501 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2507 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2525 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2531 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2537 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2543 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2549 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2555 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2561 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2579 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2585 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2591 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2597 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2603 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2609 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2615 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2621 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2633 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2639 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2645 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2651 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2657 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2663 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2669 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2681 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2687 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2705 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2711 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2717 "pascal.tab.c"
    break;

  case 81: /* type: ARROW spec_ref  */
#line 363 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 82: /* $@1: %empty  */
#line 364 "pascal.y"
                                                       { }
#line 2729 "pascal.tab.c"
    break;

  case 83: /* $@2: %empty  */
#line 364 "pascal.y"
                                                           { }
#line 2735 "pascal.tab.c"
    break;

  case 84: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 364 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2741 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY OFSY type  */
#line 365 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 86: /* type: PROCEDURESY pf_params  */
#line 366 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2753 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 367 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2759 "pascal.tab.c"
    break;

  case 88: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 368 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 369 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 90: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 370 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2777 "pascal.tab.c"
    break;

  case 91: /* $@3: %empty  */
#line 371 "pascal.y"
                         { }
#line 2783 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 371 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt SETSY OFSY simple_type  */
#line 372 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2795 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt FILESY  */
#line 373 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2801 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY OFSY type  */
#line 374 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 96: /* type: OBJECTSY record_body ENDSY  */
#line 375 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2813 "pascal.tab.c"
    break;

  case 97: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 376 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2819 "pascal.tab.c"
    break;

  case 98: /* type: CLASSSY class_members ENDSY  */
#line 377 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2825 "pascal.tab.c"
    break;

  case 99: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 378 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 100: /* result_type: IDENT  */
#line 381 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2837 "pascal.tab.c"
    break;

  case 101: /* result_type: IDENT PERIOD IDENT  */
#line 382 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2843 "pascal.tab.c"
    break;

  case 102: /* result_type: spec_ref  */
#line 383 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2849 "pascal.tab.c"
    break;

  case 103: /* class_members: %empty  */
#line 386 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2855 "pascal.tab.c"
    break;

  case 104: /* class_members: class_members class_member  */
#line 387 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2861 "pascal.tab.c"
    break;

  case 105: /* class_member: IDENT  */
#line 390 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2867 "pascal.tab.c"
    break;

  case 106: /* class_member: IDENT SEMICOLON  */
#line 391 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2873 "pascal.tab.c"
    break;

  case 107: /* class_member: id_list COLON type SEMICOLON  */
#line 392 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 108: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 393 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2885 "pascal.tab.c"
    break;

  case 109: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 394 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 110: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 395 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2897 "pascal.tab.c"
    break;

  case 111: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 396 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2903 "pascal.tab.c"
    break;

  case 112: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 397 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2909 "pascal.tab.c"
    break;

  case 113: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 398 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2915 "pascal.tab.c"
    break;

  case 114: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 399 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2921 "pascal.tab.c"
    break;

  case 115: /* prop_index_opt: %empty  */
#line 402 "pascal.y"
    { (yyval.node) = NULL; }
#line 2927 "pascal.tab.c"
    break;

  case 116: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 403 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2933 "pascal.tab.c"
    break;

  case 117: /* prop_specs: %empty  */
#line 406 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2939 "pascal.tab.c"
    break;

  case 118: /* prop_specs: prop_specs IDENT IDENT  */
#line 407 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2945 "pascal.tab.c"
    break;

  case 119: /* prop_specs: prop_specs IDENT INTCONST  */
#line 408 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2951 "pascal.tab.c"
    break;

  case 120: /* packed_opt: PACKEDSY  */
#line 411 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2957 "pascal.tab.c"
    break;

  case 121: /* packed_opt: %empty  */
#line 412 "pascal.y"
     { (yyval.node) = NULL; }
#line 2963 "pascal.tab.c"
    break;

  case 122: /* simple_type: LPARENT enum_list RPARENT  */
#line 415 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2969 "pascal.tab.c"
    break;

  case 123: /* simple_type: IDENT  */
#line 416 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2975 "pascal.tab.c"
    break;

  case 124: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 417 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2981 "pascal.tab.c"
    break;

  case 125: /* simple_type: IDENT PERIOD IDENT  */
#line 418 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2987 "pascal.tab.c"
    break;

  case 126: /* simple_type: spec_ref  */
#line 419 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2993 "pascal.tab.c"
    break;

  case 127: /* simple_type: lo_bound DOTDOT b_expr  */
#line 420 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2999 "pascal.tab.c"
    break;

  case 128: /* lo_bound: lo_term  */
#line 423 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3005 "pascal.tab.c"
    break;

  case 129: /* lo_bound: PLUS lo_term  */
#line 424 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3011 "pascal.tab.c"
    break;

  case 130: /* lo_bound: MINUS lo_term  */
#line 425 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3017 "pascal.tab.c"
    break;

  case 131: /* lo_bound: lo_bound PLUS b_term  */
#line 426 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3023 "pascal.tab.c"
    break;

  case 132: /* lo_bound: lo_bound MINUS b_term  */
#line 427 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3029 "pascal.tab.c"
    break;

  case 133: /* lo_term: scalar_constant  */
#line 430 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3035 "pascal.tab.c"
    break;

  case 134: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 431 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 135: /* lo_term: lo_term MUL b_factor  */
#line 432 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3047 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term IDIV b_factor  */
#line 433 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3053 "pascal.tab.c"
    break;

  case 137: /* lo_term: lo_term IMOD b_factor  */
#line 434 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3059 "pascal.tab.c"
    break;

  case 138: /* lo_term: lo_term SHIFTOP b_factor  */
#line 435 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3065 "pascal.tab.c"
    break;

  case 139: /* b_expr: b_term  */
#line 438 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3071 "pascal.tab.c"
    break;

  case 140: /* b_expr: PLUS b_term  */
#line 439 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3077 "pascal.tab.c"
    break;

  case 141: /* b_expr: MINUS b_term  */
#line 440 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 142: /* b_expr: b_expr PLUS b_term  */
#line 441 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3089 "pascal.tab.c"
    break;

  case 143: /* b_expr: b_expr MINUS b_term  */
#line 442 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3095 "pascal.tab.c"
    break;

  case 144: /* b_term: b_factor  */
#line 445 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3101 "pascal.tab.c"
    break;

  case 145: /* b_term: b_term MUL b_factor  */
#line 446 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term IDIV b_factor  */
#line 447 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 147: /* b_term: b_term IMOD b_factor  */
#line 448 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3119 "pascal.tab.c"
    break;

  case 148: /* b_term: b_term SHIFTOP b_factor  */
#line 449 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3125 "pascal.tab.c"
    break;

  case 149: /* b_factor: scalar_constant  */
#line 452 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3131 "pascal.tab.c"
    break;

  case 150: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 453 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3137 "pascal.tab.c"
    break;

  case 151: /* b_factor: LPARENT b_expr RPARENT  */
#line 454 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3143 "pascal.tab.c"
    break;

  case 152: /* record_body: record_field_list record_case_opt  */
#line 457 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 153: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 460 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3155 "pascal.tab.c"
    break;

  case 154: /* record_field_list: record_field  */
#line 461 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3161 "pascal.tab.c"
    break;

  case 155: /* $@4: %empty  */
#line 464 "pascal.y"
                  { }
#line 3167 "pascal.tab.c"
    break;

  case 156: /* record_field: id_list COLON $@4 type  */
#line 464 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3173 "pascal.tab.c"
    break;

  case 157: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 465 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 158: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 466 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 159: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 467 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3191 "pascal.tab.c"
    break;

  case 160: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 468 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3197 "pascal.tab.c"
    break;

  case 161: /* record_field: %empty  */
#line 469 "pascal.y"
     { (yyval.node) = NULL; }
#line 3203 "pascal.tab.c"
    break;

  case 162: /* case_arm_mark: %empty  */
#line 472 "pascal.y"
    { }
#line 3209 "pascal.tab.c"
    break;

  case 163: /* case_open: %empty  */
#line 475 "pascal.y"
    { }
#line 3215 "pascal.tab.c"
    break;

  case 164: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 478 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3221 "pascal.tab.c"
    break;

  case 165: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 479 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3227 "pascal.tab.c"
    break;

  case 166: /* record_case_opt: %empty  */
#line 480 "pascal.y"
     { (yyval.node) = NULL; }
#line 3233 "pascal.tab.c"
    break;

  case 167: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 483 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 168: /* record_case_list: record_case_arm  */
#line 484 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3245 "pascal.tab.c"
    break;

  case 169: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 487 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3251 "pascal.tab.c"
    break;

  case 170: /* record_case_arm: %empty  */
#line 488 "pascal.y"
     { (yyval.node) = NULL; }
#line 3257 "pascal.tab.c"
    break;

  case 171: /* var_decl_list: var_decl_list var_decl  */
#line 491 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 172: /* var_decl_list: var_decl  */
#line 492 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3269 "pascal.tab.c"
    break;

  case 173: /* var_decl: id_list COLON type SEMICOLON  */
#line 495 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3275 "pascal.tab.c"
    break;

  case 174: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 496 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3281 "pascal.tab.c"
    break;

  case 175: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 497 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3287 "pascal.tab.c"
    break;

  case 176: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 500 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3293 "pascal.tab.c"
    break;

  case 177: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 501 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3299 "pascal.tab.c"
    break;

  case 178: /* $@5: %empty  */
#line 502 "pascal.y"
                                                                               { }
#line 3305 "pascal.tab.c"
    break;

  case 179: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 502 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3311 "pascal.tab.c"
    break;

  case 180: /* $@6: %empty  */
#line 503 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3317 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 503 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3323 "pascal.tab.c"
    break;

  case 182: /* $@7: %empty  */
#line 504 "pascal.y"
                                        { }
#line 3329 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 504 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3335 "pascal.tab.c"
    break;

  case 184: /* $@8: %empty  */
#line 505 "pascal.y"
                                                                                                           { }
#line 3341 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 505 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3347 "pascal.tab.c"
    break;

  case 186: /* $@9: %empty  */
#line 506 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3353 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 506 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3359 "pascal.tab.c"
    break;

  case 188: /* $@10: %empty  */
#line 507 "pascal.y"
                                                                                                 { }
#line 3365 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3371 "pascal.tab.c"
    break;

  case 190: /* $@11: %empty  */
#line 508 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3377 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 508 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3383 "pascal.tab.c"
    break;

  case 192: /* $@12: %empty  */
#line 509 "pascal.y"
                                                                                            { }
#line 3389 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 509 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3395 "pascal.tab.c"
    break;

  case 194: /* $@13: %empty  */
#line 510 "pascal.y"
                                                                                                             { }
#line 3401 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3409 "pascal.tab.c"
    break;

  case 196: /* $@14: %empty  */
#line 513 "pascal.y"
                                                                                              { }
#line 3415 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3423 "pascal.tab.c"
    break;

  case 198: /* $@15: %empty  */
#line 516 "pascal.y"
                                                                                             { }
#line 3429 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3437 "pascal.tab.c"
    break;

  case 200: /* $@16: %empty  */
#line 519 "pascal.y"
                                                                                                    { }
#line 3443 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3451 "pascal.tab.c"
    break;

  case 202: /* $@17: %empty  */
#line 522 "pascal.y"
                                                                                                                     { }
#line 3457 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 522 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3465 "pascal.tab.c"
    break;

  case 204: /* pv_mark: %empty  */
#line 527 "pascal.y"
    { }
#line 3471 "pascal.tab.c"
    break;

  case 205: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 530 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3477 "pascal.tab.c"
    break;

  case 206: /* parameter_list_opt: LPARENT RPARENT  */
#line 531 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3483 "pascal.tab.c"
    break;

  case 207: /* parameter_list_opt: %empty  */
#line 532 "pascal.y"
     { (yyval.node) = NULL; }
#line 3489 "pascal.tab.c"
    break;

  case 208: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 535 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3495 "pascal.tab.c"
    break;

  case 209: /* parameter_decl_list: parameter_decl  */
#line 536 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3501 "pascal.tab.c"
    break;

  case 210: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 539 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3507 "pascal.tab.c"
    break;

  case 211: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 540 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3513 "pascal.tab.c"
    break;

  case 212: /* parameter_decl: VARSY id_list COLON param_type  */
#line 541 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3519 "pascal.tab.c"
    break;

  case 213: /* parameter_decl: id_list COLON param_type  */
#line 542 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3525 "pascal.tab.c"
    break;

  case 214: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 543 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3531 "pascal.tab.c"
    break;

  case 215: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 544 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3537 "pascal.tab.c"
    break;

  case 216: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 545 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3543 "pascal.tab.c"
    break;

  case 217: /* parameter_decl: IDENT id_list COLON param_type  */
#line 546 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3549 "pascal.tab.c"
    break;

  case 218: /* param_type: IDENT  */
#line 549 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3555 "pascal.tab.c"
    break;

  case 219: /* param_type: IDENT PERIOD IDENT  */
#line 550 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3561 "pascal.tab.c"
    break;

  case 220: /* param_type: ARRAYSY OFSY IDENT  */
#line 551 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3567 "pascal.tab.c"
    break;

  case 221: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 552 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3573 "pascal.tab.c"
    break;

  case 222: /* param_type: spec_ref  */
#line 553 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3579 "pascal.tab.c"
    break;

  case 223: /* routine_directives: %empty  */
#line 556 "pascal.y"
    { (yyval.node) = NULL; }
#line 3585 "pascal.tab.c"
    break;

  case 224: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 557 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3598 "pascal.tab.c"
    break;

  case 225: /* pf_params: LPARENT pf_sections RPARENT  */
#line 567 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3604 "pascal.tab.c"
    break;

  case 226: /* pf_params: LPARENT RPARENT  */
#line 568 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3610 "pascal.tab.c"
    break;

  case 227: /* pf_params: %empty  */
#line 569 "pascal.y"
     { (yyval.node) = NULL; }
#line 3616 "pascal.tab.c"
    break;

  case 228: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 572 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3622 "pascal.tab.c"
    break;

  case 229: /* pf_sections: pf_section  */
#line 573 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3628 "pascal.tab.c"
    break;

  case 230: /* pf_section: id_list COLON param_type  */
#line 576 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3634 "pascal.tab.c"
    break;

  case 231: /* pf_section: VARSY id_list COLON param_type  */
#line 577 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3640 "pascal.tab.c"
    break;

  case 232: /* pf_section: CONSTSY id_list COLON param_type  */
#line 578 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3646 "pascal.tab.c"
    break;

  case 233: /* pf_section: IDENT id_list COLON param_type  */
#line 579 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3652 "pascal.tab.c"
    break;

  case 234: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 580 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3658 "pascal.tab.c"
    break;

  case 235: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 581 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3664 "pascal.tab.c"
    break;

  case 236: /* enum_list: enum_list COMMA enum_member  */
#line 584 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3670 "pascal.tab.c"
    break;

  case 237: /* enum_list: enum_member  */
#line 585 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3676 "pascal.tab.c"
    break;

  case 238: /* enum_member: IDENT  */
#line 588 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3682 "pascal.tab.c"
    break;

  case 239: /* id_list: id_list COMMA IDENT  */
#line 591 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3688 "pascal.tab.c"
    break;

  case 240: /* id_list: IDENT  */
#line 592 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3694 "pascal.tab.c"
    break;

  case 241: /* body: BEGINSY statement_list ENDSY  */
#line 595 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3700 "pascal.tab.c"
    break;

  case 242: /* $@18: %empty  */
#line 598 "pascal.y"
                             { pas_stmt_mark(); }
#line 3706 "pascal.tab.c"
    break;

  case 243: /* statement_list: statement_list SEMICOLON $@18 statement  */
#line 598 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3712 "pascal.tab.c"
    break;

  case 244: /* $@19: %empty  */
#line 599 "pascal.y"
     { pas_stmt_mark(); }
#line 3718 "pascal.tab.c"
    break;

  case 245: /* statement_list: $@19 statement  */
#line 599 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3724 "pascal.tab.c"
    break;

  case 246: /* $@20: %empty  */
#line 602 "pascal.y"
    { pas_stmt_mark(); }
#line 3730 "pascal.tab.c"
    break;

  case 247: /* body_stmt: $@20 statement  */
#line 602 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3736 "pascal.tab.c"
    break;

  case 248: /* statement: statement_no_label  */
#line 605 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3742 "pascal.tab.c"
    break;

  case 249: /* statement: INTCONST COLON statement_no_label  */
#line 606 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3748 "pascal.tab.c"
    break;

  case 250: /* statement: IDENT COLON statement_no_label  */
#line 607 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3754 "pascal.tab.c"
    break;

  case 251: /* statement_no_label: assignment  */
#line 610 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3760 "pascal.tab.c"
    break;

  case 252: /* statement_no_label: call  */
#line 611 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3766 "pascal.tab.c"
    break;

  case 253: /* statement_no_label: compound_statement  */
#line 612 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3772 "pascal.tab.c"
    break;

  case 254: /* statement_no_label: goto_statement  */
#line 613 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3778 "pascal.tab.c"
    break;

  case 255: /* statement_no_label: if_statement  */
#line 614 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3784 "pascal.tab.c"
    break;

  case 256: /* statement_no_label: case_statement  */
#line 615 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3790 "pascal.tab.c"
    break;

  case 257: /* statement_no_label: while_statement  */
#line 616 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3796 "pascal.tab.c"
    break;

  case 258: /* statement_no_label: repeat_statement  */
#line 617 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3802 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: for_statement  */
#line 618 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3808 "pascal.tab.c"
    break;

  case 260: /* statement_no_label: with_statement  */
#line 619 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3814 "pascal.tab.c"
    break;

  case 261: /* statement_no_label: try_statement  */
#line 620 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3820 "pascal.tab.c"
    break;

  case 262: /* statement_no_label: raise_statement  */
#line 621 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3826 "pascal.tab.c"
    break;

  case 263: /* statement_no_label: inherited_call  */
#line 622 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3832 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: INHERITEDSY  */
#line 623 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3838 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: %empty  */
#line 624 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3844 "pascal.tab.c"
    break;

  case 266: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 627 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3850 "pascal.tab.c"
    break;

  case 267: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 628 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3856 "pascal.tab.c"
    break;

  case 268: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 629 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3862 "pascal.tab.c"
    break;

  case 269: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 630 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3868 "pascal.tab.c"
    break;

  case 270: /* handler_list: handler  */
#line 633 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3874 "pascal.tab.c"
    break;

  case 271: /* handler_list: handler_list SEMICOLON handler  */
#line 634 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3880 "pascal.tab.c"
    break;

  case 272: /* handler_list: handler_list SEMICOLON  */
#line 635 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3886 "pascal.tab.c"
    break;

  case 273: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 638 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3892 "pascal.tab.c"
    break;

  case 274: /* handler: ONSY IDENT DOSY body_stmt  */
#line 639 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3898 "pascal.tab.c"
    break;

  case 275: /* raise_statement: RAISESY  */
#line 642 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3904 "pascal.tab.c"
    break;

  case 276: /* raise_statement: RAISESY expression  */
#line 643 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3910 "pascal.tab.c"
    break;

  case 277: /* inherited_call: INHERITEDSY IDENT  */
#line 646 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3916 "pascal.tab.c"
    break;

  case 278: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 647 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3922 "pascal.tab.c"
    break;

  case 279: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 648 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3928 "pascal.tab.c"
    break;

  case 280: /* call: IDENT  */
#line 651 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3934 "pascal.tab.c"
    break;

  case 281: /* call: spec_expr  */
#line 652 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3940 "pascal.tab.c"
    break;

  case 282: /* call: selector PERIOD IDENT  */
#line 653 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3946 "pascal.tab.c"
    break;

  case 283: /* call: call_with_args  */
#line 654 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3952 "pascal.tab.c"
    break;

  case 284: /* recv_primary: STRINGCONST  */
#line 657 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 3958 "pascal.tab.c"
    break;

  case 285: /* recv_primary: LPARENT expression RPARENT  */
#line 658 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3964 "pascal.tab.c"
    break;

  case 286: /* recv_primary: spec_expr  */
#line 659 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3970 "pascal.tab.c"
    break;

  case 287: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 662 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3976 "pascal.tab.c"
    break;

  case 288: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 663 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3982 "pascal.tab.c"
    break;

  case 289: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 664 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3988 "pascal.tab.c"
    break;

  case 290: /* call_with_args: IDENT LPARENT RPARENT  */
#line 665 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3994 "pascal.tab.c"
    break;

  case 291: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 666 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4000 "pascal.tab.c"
    break;

  case 292: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 667 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4006 "pascal.tab.c"
    break;

  case 293: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 668 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4012 "pascal.tab.c"
    break;

  case 294: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 669 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4018 "pascal.tab.c"
    break;

  case 295: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 670 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4024 "pascal.tab.c"
    break;

  case 296: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 671 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4030 "pascal.tab.c"
    break;

  case 297: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 672 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4036 "pascal.tab.c"
    break;

  case 298: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 673 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4042 "pascal.tab.c"
    break;

  case 299: /* argument_list: argument_list COMMA argument  */
#line 676 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4048 "pascal.tab.c"
    break;

  case 300: /* argument_list: argument  */
#line 677 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4054 "pascal.tab.c"
    break;

  case 301: /* argument: expression  */
#line 680 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4060 "pascal.tab.c"
    break;

  case 302: /* argument: expression COLON expression  */
#line 681 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4066 "pascal.tab.c"
    break;

  case 303: /* argument: expression COLON expression COLON expression  */
#line 682 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4072 "pascal.tab.c"
    break;

  case 304: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 683 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4078 "pascal.tab.c"
    break;

  case 305: /* assignment: selector BECOMES expression  */
#line 686 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4084 "pascal.tab.c"
    break;

  case 306: /* assignment: selector ADDBECOMES expression  */
#line 687 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4090 "pascal.tab.c"
    break;

  case 307: /* assignment: selector SUBBECOMES expression  */
#line 688 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4096 "pascal.tab.c"
    break;

  case 308: /* assignment: selector MULBECOMES expression  */
#line 689 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4102 "pascal.tab.c"
    break;

  case 309: /* assignment: selector DIVBECOMES expression  */
#line 690 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4108 "pascal.tab.c"
    break;

  case 310: /* assignment: call_with_args BECOMES expression  */
#line 691 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4114 "pascal.tab.c"
    break;

  case 311: /* selector: selector LBRACK expression_list RBRACK  */
#line 694 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4120 "pascal.tab.c"
    break;

  case 312: /* selector: selector PERIOD IDENT  */
#line 695 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4126 "pascal.tab.c"
    break;

  case 313: /* selector: selector ARROW  */
#line 696 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4132 "pascal.tab.c"
    break;

  case 314: /* selector: call_with_args PERIOD IDENT  */
#line 697 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4138 "pascal.tab.c"
    break;

  case 315: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 698 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4144 "pascal.tab.c"
    break;

  case 316: /* selector: call_with_args ARROW  */
#line 699 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4150 "pascal.tab.c"
    break;

  case 317: /* selector: recv_primary PERIOD IDENT  */
#line 700 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4156 "pascal.tab.c"
    break;

  case 318: /* selector: recv_primary ARROW  */
#line 701 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4162 "pascal.tab.c"
    break;

  case 319: /* selector: IDENT  */
#line 702 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4168 "pascal.tab.c"
    break;

  case 320: /* expression_list: expression_list COMMA expression  */
#line 705 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4174 "pascal.tab.c"
    break;

  case 321: /* expression_list: expression  */
#line 706 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4180 "pascal.tab.c"
    break;

  case 322: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 709 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4186 "pascal.tab.c"
    break;

  case 323: /* goto_statement: GOTOSY INTCONST  */
#line 712 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4192 "pascal.tab.c"
    break;

  case 324: /* goto_statement: GOTOSY IDENT  */
#line 713 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4198 "pascal.tab.c"
    break;

  case 325: /* if_statement: IFSY expression THENSY body_stmt  */
#line 716 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4204 "pascal.tab.c"
    break;

  case 326: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 717 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4210 "pascal.tab.c"
    break;

  case 327: /* $@21: %empty  */
#line 720 "pascal.y"
                           { }
#line 4216 "pascal.tab.c"
    break;

  case 328: /* case_statement: CASESY expression OFSY $@21 case_list case_tail ENDSY  */
#line 720 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4222 "pascal.tab.c"
    break;

  case 329: /* case_tail: %empty  */
#line 723 "pascal.y"
    { (yyval.node) = NULL; }
#line 4228 "pascal.tab.c"
    break;

  case 330: /* case_tail: ELSESY statement_list  */
#line 724 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4234 "pascal.tab.c"
    break;

  case 331: /* case_tail: OTHERWISESY statement_list  */
#line 725 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4240 "pascal.tab.c"
    break;

  case 332: /* case_list: case_list SEMICOLON case_elem  */
#line 728 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4246 "pascal.tab.c"
    break;

  case 333: /* case_list: case_elem  */
#line 729 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4252 "pascal.tab.c"
    break;

  case 334: /* case_elem: constant_list COLON body_stmt  */
#line 732 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4258 "pascal.tab.c"
    break;

  case 335: /* case_elem: %empty  */
#line 733 "pascal.y"
     { (yyval.node) = NULL; }
#line 4264 "pascal.tab.c"
    break;

  case 336: /* constant_list: constant_list COMMA case_label  */
#line 736 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4270 "pascal.tab.c"
    break;

  case 337: /* constant_list: case_label  */
#line 737 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4276 "pascal.tab.c"
    break;

  case 338: /* case_label: b_expr  */
#line 740 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4282 "pascal.tab.c"
    break;

  case 339: /* case_label: b_expr DOTDOT b_expr  */
#line 741 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4288 "pascal.tab.c"
    break;

  case 340: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 744 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4294 "pascal.tab.c"
    break;

  case 341: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 747 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4300 "pascal.tab.c"
    break;

  case 342: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 750 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4306 "pascal.tab.c"
    break;

  case 343: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 751 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4312 "pascal.tab.c"
    break;

  case 344: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 752 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4318 "pascal.tab.c"
    break;

  case 345: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 755 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 346: /* with_open: with_open COMMA selector  */
#line 758 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4330 "pascal.tab.c"
    break;

  case 347: /* with_open: selector  */
#line 759 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4336 "pascal.tab.c"
    break;

  case 348: /* expression: simple_expression  */
#line 766 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4342 "pascal.tab.c"
    break;

  case 349: /* expression: expression INOP simple_expression  */
#line 767 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4348 "pascal.tab.c"
    break;

  case 350: /* expression: expression LTOP simple_expression  */
#line 768 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4354 "pascal.tab.c"
    break;

  case 351: /* expression: expression LEOP simple_expression  */
#line 769 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 352: /* expression: expression GTOP simple_expression  */
#line 770 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4366 "pascal.tab.c"
    break;

  case 353: /* expression: expression GEOP simple_expression  */
#line 771 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4372 "pascal.tab.c"
    break;

  case 354: /* expression: expression NEOP simple_expression  */
#line 772 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4378 "pascal.tab.c"
    break;

  case 355: /* expression: expression EQOP simple_expression  */
#line 773 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4384 "pascal.tab.c"
    break;

  case 356: /* expression: expression ISOP simple_expression  */
#line 774 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4390 "pascal.tab.c"
    break;

  case 357: /* simple_expression: term  */
#line 777 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4396 "pascal.tab.c"
    break;

  case 358: /* simple_expression: PLUS term  */
#line 778 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4402 "pascal.tab.c"
    break;

  case 359: /* simple_expression: MINUS term  */
#line 779 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4408 "pascal.tab.c"
    break;

  case 360: /* simple_expression: simple_expression PLUS term  */
#line 780 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4414 "pascal.tab.c"
    break;

  case 361: /* simple_expression: simple_expression MINUS term  */
#line 781 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4420 "pascal.tab.c"
    break;

  case 362: /* simple_expression: simple_expression OROP term  */
#line 782 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4426 "pascal.tab.c"
    break;

  case 363: /* simple_expression: simple_expression XOROP term  */
#line 783 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4432 "pascal.tab.c"
    break;

  case 364: /* term: power  */
#line 786 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4438 "pascal.tab.c"
    break;

  case 365: /* term: term MUL power  */
#line 787 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4444 "pascal.tab.c"
    break;

  case 366: /* term: term RDIV power  */
#line 788 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4450 "pascal.tab.c"
    break;

  case 367: /* term: term IDIV power  */
#line 789 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4456 "pascal.tab.c"
    break;

  case 368: /* term: term IMOD power  */
#line 790 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4462 "pascal.tab.c"
    break;

  case 369: /* term: term ANDOP power  */
#line 791 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4468 "pascal.tab.c"
    break;

  case 370: /* term: term MUL MINUS power  */
#line 792 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4474 "pascal.tab.c"
    break;

  case 371: /* term: term RDIV MINUS power  */
#line 793 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4480 "pascal.tab.c"
    break;

  case 372: /* term: term IDIV MINUS power  */
#line 794 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4486 "pascal.tab.c"
    break;

  case 373: /* term: term IMOD MINUS power  */
#line 795 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4492 "pascal.tab.c"
    break;

  case 374: /* term: term ANDOP MINUS power  */
#line 796 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4498 "pascal.tab.c"
    break;

  case 375: /* term: term SHIFTOP power  */
#line 797 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4504 "pascal.tab.c"
    break;

  case 376: /* term: term ASOP power  */
#line 798 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4510 "pascal.tab.c"
    break;

  case 377: /* power: factor  */
#line 801 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4516 "pascal.tab.c"
    break;

  case 378: /* power: power POWOP factor  */
#line 802 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4522 "pascal.tab.c"
    break;

  case 379: /* factor: selector  */
#line 805 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4528 "pascal.tab.c"
    break;

  case 380: /* factor: call_with_args  */
#line 806 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4534 "pascal.tab.c"
    break;

  case 381: /* factor: INTCONST  */
#line 807 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4540 "pascal.tab.c"
    break;

  case 382: /* factor: REALCONST  */
#line 808 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4546 "pascal.tab.c"
    break;

  case 383: /* factor: STRINGCONST  */
#line 809 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4552 "pascal.tab.c"
    break;

  case 384: /* factor: CHARCODE  */
#line 810 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4558 "pascal.tab.c"
    break;

  case 385: /* factor: LPARENT expression RPARENT  */
#line 811 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4564 "pascal.tab.c"
    break;

  case 386: /* factor: inherited_call  */
#line 812 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4570 "pascal.tab.c"
    break;

  case 387: /* factor: NOTSY factor  */
#line 813 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4576 "pascal.tab.c"
    break;

  case 388: /* factor: ATSIGN factor  */
#line 814 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4582 "pascal.tab.c"
    break;

  case 389: /* factor: LBRACK RBRACK  */
#line 815 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4588 "pascal.tab.c"
    break;

  case 390: /* factor: LBRACK set_member_list RBRACK  */
#line 816 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4594 "pascal.tab.c"
    break;

  case 391: /* set_member_list: set_member  */
#line 819 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4600 "pascal.tab.c"
    break;

  case 392: /* set_member_list: set_member_list COMMA set_member  */
#line 820 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4606 "pascal.tab.c"
    break;

  case 393: /* set_member: expression  */
#line 823 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4612 "pascal.tab.c"
    break;

  case 394: /* set_member: expression DOTDOT expression  */
#line 824 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4618 "pascal.tab.c"
    break;


#line 4622 "pascal.tab.c"

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

#line 830 "pascal.y"

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
