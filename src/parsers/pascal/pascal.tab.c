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
#define YYLAST   1835

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

#define YYPACT_NINF (-577)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-333)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     516,   156,   177,   340,  -577,   220,   207,  -577,  -577,   145,
    -577,    93,  -577,  -577,   386,   284,   177,   297,   312,   312,
     327,  -577,   295,   225,   339,   384,   354,  -577,  -577,  -577,
     401,   312,   466,   473,   419,  -577,  -577,    80,   502,   284,
    -577,    88,   451,   279,  -577,   312,  -577,   550,   155,   396,
     181,  1006,   637,   470,   295,  -577,   492,   503,   511,   497,
     548,   583,   605,  -577,   500,  -577,  -577,  -577,  -577,   517,
    1400,    50,  -577,  -577,   177,   312,   639,   620,  -577,   647,
      50,  -577,   312,   653,    77,  -577,  -577,   652,   667,  -577,
    1400,   356,  1400,  -577,  1400,  1400,   574,  -577,   942,  -577,
    1400,   673,   709,  -577,     4,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,   257,   568,  -577,  1064,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,   312,  1150,   656,  -577,   729,   733,
     743,   736,   739,   720,   757,  -577,   747,   562,  -577,  -577,
    1608,  1608,  1608,  1200,  1400,  1608,  -577,  -577,  -577,   563,
     141,   673,   623,  -577,   782,   608,   139,   369,   783,   751,
    -577,   688,   767,  -577,   767,  1001,  1001,    60,   769,  -577,
    -577,  -577,  -577,  1485,    23,   268,  -577,  -577,  -577,  -577,
     799,   480,  -577,   395,   226,   451,   275,  -577,   132,   851,
    -577,    53,   344,  -577,  -577,   795,  1006,  -577,  -577,   692,
     391,   488,   782,   608,    37,  1005,   287,  1167,  1649,  1084,
    1084,  1225,   645,    24,  1754,   798,   811,  1250,   805,  -577,
    1400,   806,  -577,  1400,  1275,  1400,   809,  -577,  1400,  1400,
    1400,  1400,  1400,   359,    75,   245,   866,   312,   815,   819,
      15,  -577,  -577,   312,   312,  -577,   284,   177,  -577,   835,
     312,   836,   295,  -577,  -577,   783,   783,  -577,  1047,   484,
    -577,  1667,  -577,   839,  -577,  1400,  1400,  1400,  1400,  1400,
    1400,  1400,  1400,  1608,  1608,  1608,  1608,  1491,  1516,  1541,
    1566,  1591,  1608,  1608,  1608,   844,   254,  -577,   192,   862,
     863,   867,   226,   226,    86,  -577,  -577,   501,  -577,   645,
     852,   824,   824,   856,   254,   315,   870,   871,   872,   200,
     874,   881,   882,   897,   247,  -577,   713,  1425,   202,   931,
     934,  -577,   702,   702,   824,   702,   702,   702,   702,  -577,
     620,   312,   888,   312,   891,  -577,   312,    31,  -577,   724,
     954,  -577,  1425,   893,  -577,   620,  -577,   258,  -577,  1400,
    1400,  1400,  -577,  -577,   356,  -577,  -577,  -577,  -577,  1170,
    -577,  -577,  -577,   506,  -577,   699,  1685,   101,  -577,   878,
    -577,  1300,   645,  -577,   527,   907,  1754,   912,   570,  1754,
    -577,   537,  1754,  1751,   640,  1754,  1754,  1754,  1754,   924,
    -577,  -577,   254,  -577,   360,  -577,  -577,  -577,  -577,   620,
     620,   454,   474,   284,   189,  1104,  -577,   312,  -577,   295,
    1400,  1400,  -577,   730,   914,   369,   369,   369,   369,   369,
     369,   369,   369,   783,   783,   783,   783,  1608,   751,  1608,
     751,  1608,   751,  1608,   751,  1608,   751,   751,   751,  -577,
     918,    86,   558,  -577,  -577,   524,   312,   906,   312,   910,
    -577,   312,    34,  -577,   753,   915,   258,   769,  -577,   476,
    -577,   702,   702,   824,   926,  -577,   211,   331,  -577,   426,
    -577,   578,   917,  -577,   919,   152,   427,   920,   921,   922,
    -577,   778,  -577,  -577,   935,  -577,   123,  -577,  -577,  -577,
     710,   929,  -577,  -577,  1450,  1009,  1754,    50,   645,    50,
     645,   710,   331,   331,   701,  -577,  -577,  -577,  -577,   620,
    1010,   793,   767,   808,   767,   814,    52,  -577,    26,  1011,
     943,  1015,  1017,   620,   963,  1019,   219,  -577,  1023,   887,
     624,  1754,  -577,  1006,  -577,   608,   824,   988,  1400,  -577,
    1400,  1400,   645,  -577,   958,   316,   290,  -577,   332,  -577,
     580,   494,  -577,  1325,  1350,  1400,  -577,  -577,  1375,  -577,
      50,   596,   982,   620,   620,   620,   620,  1031,  1032,  -577,
    -577,  -577,  -577,  -577,  1027,   620,   620,  1754,  -577,   751,
     751,   751,   751,   751,  -577,   254,   983,  -577,   817,   767,
     826,   767,   850,   223,  -577,    26,  -577,  1016,  -577,  -577,
     331,   331,   464,   824,   702,   702,  -577,   702,   702,   702,
     702,  -577,  -577,  -577,  -577,  -577,   972,   973,  -577,  -577,
     999,    50,   620,   620,   710,  -577,  -577,   620,   620,  -577,
     282,    50,   142,   992,    85,  1667,  -577,  -577,   650,  -577,
    -577,  1033,  1057,  -577,    26,  -577,    26,  1024,    26,  -577,
    1038,   227,  -577,  1030,  -577,  1080,  -577,  -577,  -577,  1028,
     258,  -577,   993,  1082,  1400,  1400,  -577,  -577,   462,   162,
    -577,   864,  -577,  -577,  -577,  1701,  1720,  -577,    29,  -577,
     878,  -577,  -577,  -577,  -577,  -577,  -577,   600,  -577,   613,
    1754,  -577,   628,  1087,  1101,    50,  1105,  1055,  1056,  1059,
    -577,  -577,   620,   620,  1112,  1112,  -577,  1090,  1117,  1067,
    -577,  -577,    26,  -577,    26,  1069,    26,  -577,  -577,  1061,
    -577,   468,   331,   331,  -577,  -577,  -577,  -577,   573,   620,
     620,  -577,  -577,   620,   620,    52,  1071,  1133,  -577,  1085,
    1114,   620,   620,  -577,  -577,  -577,  1066,  -577,  1425,  1425,
     381,  -577,   645,  1121,  -577,  -577,   943,  1097,  -577,   258,
    -577,    51,  1075,  1400,  -577,  1147,   258,  1148,  -577,  1151,
     943,   745,   903,  -577,   824,   824,  -577,  -577,  1127,   824,
    -577,  -577,  1400,  1400,  -577,  1088,  -577,   351,  -577,  -577,
    -577,  -577,  -577,  1153,  -577,   258,   258,   258,   943,   943,
    1160,  1106,  1112,  1108,  -577,   258,   621,  -577,  -577,   258,
    -577,  -577,  -577,  -577,  1164,  1110,   620,   620,  1168,  1169,
     110,   254,  -577,   258,  -577,  1115,  1118,   824,  1152,  -577,
    -577,   649,  -577,  1122,  1120,  -577,   943,  -577,  1400,  -577,
    -577,  -577,  -577,  1754,  -577,  1176,  -577,  -577,  -577,  -577,
    -577,   701,  -577,  1112,  1112,  -577,  -577,  -577,  1735,  1754,
    -577,  1171,  -577,  -577,   943,  1180,  1182,  1184,  -577,  -577,
    -577,   258,  -577,   943,  1187,  -577,  -577,  -577,   258,  1193,
    1139,  -577,  -577,  -577,  -577,  -577,   258,   258,  1196,  -577,
     883,  -577,  1425,  -577,  1425,  1178,  -577,  -577,  1202,  1754,
    -577,   943,  1205,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  1207,  1208,   943,  1216,  -577,  1222,  -577,   258,    44,
    -577,  -577,   824,  1172,   824,  -577,  -577,    50,    50,  1223,
    -577,   943,  -577,  -577,  -577,  1230,   943,   943,   943,  -577,
    -577,  -577,  -577,   943,  -577,  1232,  -577,   672,  -577,  -577,
    1196,  -577,  -577,  -577,  -577,  1237,  -577,  -577,  -577,  -577,
    1239,   943,  -577,  -577,  -577,   710,  1241,  -577,  1242,  1244,
    1245,  -577,  -577,  1186,  -577,  -577,  -577,  -577,  1247,  -577,
    -577
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
       0,     0,   240,     0,   240,     0,     0,   218,     0,     0,
     187,     0,     0,   220,     0,     0,   104,   106,     0,     0,
       0,   354,   353,   278,   358,   359,   348,   338,     0,   300,
       0,     0,     0,    67,     0,     0,     0,   283,     0,   291,
       0,     0,   301,     0,     0,     0,   328,   304,     0,   324,
     127,     0,     0,   220,   220,   220,   220,     0,     0,   217,
     217,    23,   257,   257,    33,   220,   220,   407,   405,   385,
     386,   387,   383,   384,   108,     0,   107,   102,     0,   240,
       0,   240,     0,     0,   238,     0,    89,    90,   249,    77,
     146,   147,     0,     0,     0,     0,   130,     0,     0,     0,
       0,   140,   109,   217,   217,   112,     0,     0,   217,   217,
     121,   127,   220,   220,   170,   217,   217,   220,   220,   159,
       0,   127,   332,     0,     0,    42,    41,    87,     0,    97,
      95,     0,     0,   236,     0,   223,     0,     0,     0,   221,
       0,   231,   235,   226,   185,     0,    12,   183,   184,     0,
       0,   192,     0,   236,     0,     0,   259,   260,   351,   342,
     346,     0,   350,   259,   312,   334,   315,    79,     0,   279,
     285,   280,   257,   282,   292,    74,   311,     0,   309,     0,
     333,   307,     0,     0,   109,   127,     0,     0,     0,     0,
     236,   236,   220,   220,    29,    30,   257,     0,     0,     0,
      71,   109,     0,   247,     0,     0,     0,   241,   243,     0,
     157,     0,   148,   149,   152,   153,   151,   154,     0,   220,
     220,   217,   217,   220,   220,     0,     0,     0,   163,     0,
       0,   220,   220,   165,   166,   172,     0,   162,     0,     0,
       0,    44,     0,     0,    94,   236,   201,   227,   225,     0,
     230,     0,     0,     0,   237,     0,     0,     0,   105,     0,
     189,     0,     0,   357,     0,   348,   257,   257,     0,     0,
     259,   339,     0,     0,   259,     0,   284,     0,   310,   308,
     306,    59,    62,     0,   236,     0,     0,     0,   205,   207,
       0,     0,    32,     0,   236,     0,     0,   245,   244,     0,
     246,    91,   156,   101,     0,     0,   220,   220,     0,     0,
       0,     0,   113,     0,    99,     0,     0,   179,     0,    48,
      46,     0,    45,     0,     0,    84,   197,    12,     0,   224,
     234,   233,   232,   228,   188,     0,   236,   186,    12,   259,
     259,   352,   345,   344,   343,   341,   349,   347,     0,   316,
     287,     0,   281,    58,   209,     0,     0,     0,    12,    12,
     236,     0,    20,    27,     0,   103,   248,   114,     0,     0,
       0,   116,   117,   122,   123,   164,     0,     0,   174,   177,
       0,   172,     0,    43,     0,     0,    85,    12,     0,   229,
     236,   203,     0,   356,   355,   317,   259,    12,   236,   236,
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
    -577,  -577,  -577,  -577,  -577,   -65,  1008,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  1007,   -36,  -311,  -577,  -577,   -58,
    1013,   -35,  1012,    38,   -70,   -40,  -279,  -576,   -64,   121,
    -272,   -78,  -577,  -577,  -577,  -412,   671,  -242,  -577,  -577,
    -577,  -577,  -207,  -577,   -34,  -267,  -299,  -209,  -489,  -577,
    -289,  -577,  -577,   367,  -577,   336,   347,  1020,   -28,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,   577,   338,   526,   755,  -565,
      79,  -146,  -577,   684,  -577,   821,    19,  -577,   -85,  -577,
    -577,  -301,  -577,  -190,   611,  -577,  -577,   601,  -577,   -30,
    -577,  -577,   -25,  -176,   744,  -577,   102,  1060,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,   509,  -521,   507,  -577,  -577,
    -577,  -577,  -577,   269,  1558,   -99,  -208,  -112,  -577,   880
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     253,   574,   707,    37,    39,    40,   495,   831,   634,   465,
      54,    55,    41,    42,   103,   152,   442,   443,   105,   179,
     367,   180,   896,   928,   501,   528,   287,   305,   480,   736,
     919,   181,   182,   183,   184,   668,   467,   468,   313,   314,
     315,   631,   965,   827,   492,   888,   889,    45,    46,    28,
     656,   848,   346,   941,   972,   897,   954,   837,   932,   868,
     869,   907,   959,   958,   957,    77,   189,   337,   338,   653,
     520,   289,   452,   453,   297,   298,   316,    29,    50,   196,
      51,   532,   533,   106,   107,   108,   546,   547,   109,   153,
     111,   112,   154,   363,   364,   114,   155,   384,   116,   117,
     118,   119,   536,   778,   669,   670,   890,   672,   120,   121,
     122,   123,   204,   366,   157,   158,   159,   160,   259,   260
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   177,   191,    72,   200,   368,   348,   178,   206,   710,
     177,   104,   641,   176,   213,   671,   178,    78,   290,   127,
     485,   110,   176,   502,   503,   471,   113,   459,   254,    85,
     718,   521,   650,   262,   466,   469,   516,    47,    48,   593,
      11,   374,   255,   256,   597,   445,   784,   236,   381,   946,
      64,   303,   534,   397,   353,   177,   537,   504,   341,   840,
     331,   178,   161,  -299,    47,  -299,   202,   176,   217,   428,
     430,   432,   434,   436,   437,   438,   162,   163,   332,   757,
     390,   758,   194,   760,   333,    68,   164,   304,   334,   785,
     750,   398,   368,    73,   186,   354,   517,   177,    33,   594,
     551,   192,   651,   178,   165,   166,   342,   176,   176,   369,
     370,   167,   185,   561,   168,   516,   505,   506,   507,   508,
     947,   102,   169,   170,   171,   172,   173,   841,   336,   343,
     174,   292,   293,   175,   299,   740,   294,   285,    69,   286,
     331,   188,   177,   233,   264,   102,    74,   807,   178,   808,
     751,   810,    34,   115,   176,   102,   104,   615,   332,   542,
      81,   625,   600,   601,   333,   368,   110,   775,   334,   104,
     104,   113,   439,   883,   423,   424,   425,   426,   776,   110,
     110,   265,   212,   633,   113,   113,    85,   266,   267,   268,
     269,   270,   271,   203,   571,   550,   602,   335,   543,   626,
     446,   629,   748,   777,    30,   211,   211,   339,   336,    31,
    -253,    86,  -253,    79,    14,    15,   177,    16,   447,   579,
     272,   580,   178,   581,   448,   582,   306,   583,   449,   177,
     497,   446,     8,    17,   177,   178,   307,   212,   212,    18,
     178,   176,    19,    20,    21,   884,    22,    74,   767,   447,
     391,    56,   490,    10,   671,   448,   394,   450,  -253,   449,
    -253,    57,   401,   402,   498,   604,   605,   299,   451,    47,
     325,   326,   491,   303,   606,   299,   309,   177,   662,    13,
     310,   525,   327,   178,   545,   548,   762,    23,   295,   311,
     312,   638,    85,   640,   306,   680,    24,    25,   115,   451,
      26,    58,   177,   328,   307,   722,   723,   454,   178,   392,
     745,   115,   115,   365,   176,   212,   218,   356,   219,   365,
     681,    85,   177,   212,   481,   329,   365,    75,   178,   202,
     441,   682,   308,    79,   526,   677,   721,    85,    76,   156,
      12,   472,   746,   667,   309,   473,   679,   839,   310,   102,
     511,   474,   513,   102,   845,   515,    85,   311,   312,   201,
      38,   205,   683,   207,   208,   773,   645,    72,   647,   214,
     728,    52,   781,    43,   127,   607,   608,   687,   689,    78,
      61,   862,   692,   865,   866,   867,   177,   609,    44,    53,
      62,   475,   178,   874,   344,   476,    85,   876,   724,   725,
     726,   727,    79,    49,   477,   478,   479,   444,   610,   389,
     562,   885,   258,   261,   351,    59,   273,    79,    79,   637,
      95,   639,   195,   274,   275,   444,    47,   177,   177,   177,
     177,    97,   150,   178,   178,   178,   178,   829,   830,   176,
     176,   176,   176,   713,    82,   715,   832,   276,   177,   322,
     323,   102,   728,   616,   178,    83,   535,   833,    35,   914,
      60,   324,    36,   617,   481,   588,   916,   590,   527,   806,
     592,    65,   177,   365,   920,   921,   973,    63,   178,   857,
     604,   605,   693,   860,   176,    66,   318,   704,   705,   376,
     177,   611,   379,   104,   382,    67,   178,   379,   385,   386,
     387,   388,   176,   110,   569,   352,   945,   851,   113,   319,
      34,   320,    79,   444,   321,   177,   604,   605,   604,   605,
       1,   178,   604,   605,   570,   177,   599,     2,   774,   720,
     265,   178,    79,   812,   542,   339,   266,   267,   268,   269,
     270,   271,   411,   737,   685,   834,   126,   412,   903,   904,
     472,   177,   542,   747,   587,    70,   131,   178,    79,   457,
     474,   177,    71,   176,   538,   135,   458,   178,   128,   272,
     246,   539,   247,   176,   177,   248,   177,   527,   177,   129,
     178,   925,   178,   926,   178,   538,   496,   130,   249,   138,
     177,   765,   552,   139,   250,   538,   178,   787,   251,   472,
     475,   252,   557,   813,   476,   934,   220,   132,    79,   474,
      80,   496,   454,   477,   478,   479,   585,   793,   529,   530,
     531,   802,  -297,   586,  -297,   177,    84,   221,   555,   222,
     223,   178,   224,   556,   209,   115,   585,   176,   538,   652,
     365,   666,   177,   612,   177,   684,   177,   472,   178,   475,
     178,   875,   178,   476,   585,   365,   365,   474,   538,   133,
     365,   694,   477,   478,   479,   788,   265,   263,   510,   227,
     228,   538,   266,   267,   268,   269,   270,   271,   789,   577,
     258,   134,   177,   524,   188,   124,   538,   217,   178,   177,
     125,   853,   854,   790,   176,   178,   177,   475,   555,   165,
     166,   476,   178,   559,   237,   272,   444,   892,   752,   168,
     477,   478,   479,   753,   893,   187,   652,   169,   170,   171,
     172,   173,   756,   190,   197,   177,   177,   177,   198,   193,
     349,   178,   178,   178,   350,   177,   306,   567,   568,   177,
     102,   178,   770,   199,   963,   178,   307,   481,   964,   215,
     285,   177,   286,   177,   339,   604,   605,   178,   263,   178,
     227,   540,   849,   635,   330,   652,   463,   652,   243,   652,
     345,    79,   898,   493,   169,   170,   171,   172,   464,   798,
     799,   527,    79,   902,   518,   216,   309,   265,   238,  -298,
     310,  -298,   239,   266,   267,   268,   269,   270,   271,   311,
     312,   177,   240,   911,   912,   244,   245,   178,   177,   675,
     676,    79,   241,   595,   178,   242,   177,   177,   399,   400,
     360,   361,   178,   178,   690,   481,   272,   277,   278,   279,
     284,   288,   929,   652,   836,   652,    79,   652,   621,   280,
     281,   221,   935,   222,   223,   296,   224,   642,   177,   951,
     952,    79,   317,   644,   178,   347,   340,   177,   177,   372,
     282,   659,   371,   178,   178,   283,    79,   955,   646,   176,
     176,   393,    79,   864,   648,    79,   960,   712,   461,   462,
     527,   375,   377,   873,    79,   383,   714,   527,   463,   966,
     455,   395,   968,   969,   970,   396,   169,   170,   171,   172,
     464,   696,   697,   698,   699,   664,   509,   978,    79,   665,
     716,   406,   408,   708,   709,   414,   527,   527,   527,   440,
     850,   523,   779,   456,   780,   901,   527,   489,   460,   265,
     527,   302,   470,   771,   772,   266,   267,   268,   269,   270,
     271,   779,   444,   923,   527,   265,   482,   483,   484,   913,
     486,   266,   267,   268,   269,   270,   271,   487,   488,   499,
     738,   739,   500,   519,   512,   743,   744,   514,   272,   522,
     544,   553,   563,   564,   565,   566,   554,   560,   558,   931,
    -332,   584,   589,   575,   272,   576,   591,   936,   937,   938,
     603,   596,   527,   613,   943,   614,   618,   619,   620,   527,
     624,  -332,   210,  -332,  -332,   630,   211,   527,   527,    87,
    -332,  -332,  -332,  -332,   636,   643,   654,   496,   496,   655,
     657,   961,   658,   660,   661,    88,    89,    90,   663,   673,
      91,    92,   843,   355,   678,   695,   700,   701,   212,   527,
     800,   801,   706,    93,   719,   285,    94,   265,   731,   732,
     749,   858,   859,   266,   267,   268,   269,   270,   271,   622,
     623,   735,   755,   754,   627,   628,   761,   814,   815,   768,
      95,   818,   819,   169,   170,   171,   172,   291,    96,   825,
     826,    97,    98,   763,   759,   764,   272,    87,   766,   265,
      99,   769,   791,   100,   101,   266,   267,   268,   269,   270,
     271,   102,   225,    88,    89,    90,   792,   899,    91,    92,
     794,    14,    15,   410,    16,   795,   796,    85,   572,   797,
     803,    93,   804,   226,    94,   227,   228,   805,   272,   809,
      17,   821,   229,   230,   231,   232,    18,   811,   822,    19,
      20,   573,   828,    22,   824,   823,   702,   703,    95,   835,
     838,   842,   844,   846,   879,   880,   847,   855,   863,    97,
     359,   496,   234,   496,   861,   870,   871,   872,    99,   877,
     878,   100,   101,   881,   882,   886,   162,   163,   887,   102,
     891,   900,   894,   895,    23,   908,   164,   909,   906,   910,
     729,   730,   915,    24,    25,   733,   734,    26,   917,   918,
     357,   922,   741,   742,   165,   166,   927,   930,  -332,   265,
     933,   167,   939,   940,   168,   266,   267,   268,   269,   270,
     271,   942,   169,   170,   171,   172,   173,   944,   953,  -332,
     235,  -332,  -332,   175,   211,   956,   949,   962,  -332,  -332,
    -332,  -332,   967,   140,   971,   102,   974,   975,   272,   976,
     977,   979,   980,   403,   141,   142,   405,   711,   924,   404,
     950,   820,   143,   257,   144,   409,   212,   145,   140,   948,
     407,   649,   146,   147,   148,   149,   150,   717,   598,   141,
     142,   786,   674,   378,   852,     0,   856,   143,   151,   144,
     362,   578,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,   816,   817,
       0,     0,   143,   151,   144,   373,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     380,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   549,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     686,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   688,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     691,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   494,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   494,     0,     0,   145,     0,     0,
     102,     0,   146,   147,   148,   149,   632,     0,     0,   -50,
     -50,     0,     0,   299,   140,     0,     0,     0,   151,   -50,
     -50,   -50,     0,     0,   300,   102,   427,   301,     0,   302,
       0,   -50,     0,   143,     0,   144,     0,     0,   145,   140,
       0,     0,   -50,   146,   147,   148,   149,   150,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   212,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   431,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   435,     0,     0,     0,
       0,   140,     0,   143,   151,   144,     0,     0,   145,     0,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     143,     0,   144,     0,     0,   145,     0,     0,     0,   151,
     146,   147,   148,   149,   150,     0,   102,     0,     0,     0,
       0,   265,     0,     0,     0,     0,   151,   266,   267,   268,
     269,   270,   271,   102,     0,     0,     0,     0,     0,   265,
       0,     0,     0,     0,   358,   266,   267,   268,   269,   270,
     271,     0,     0,     0,     0,     0,     0,   265,     0,     0,
     272,     0,   413,   266,   267,   268,   269,   270,   271,     0,
       0,     0,     0,   265,     0,   541,     0,     0,   272,   266,
     267,   268,   269,   270,   271,     0,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,   272,   782,   266,   267,
     268,   269,   270,   271,     0,     0,     0,   265,     0,     0,
     783,     0,   272,   266,   267,   268,   269,   270,   271,  -325,
       0,     0,     0,     0,     0,     0,   265,     0,   905,     0,
       0,   272,   266,   267,   268,   269,   270,   271,     0,     0,
    -325,     0,  -325,  -325,     0,   558,   272,     0,     0,  -325,
    -325,  -325,  -325,   415,   416,   417,   418,   419,   420,   421,
     422,     0,     0,     0,     0,   272
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,    39,    89,   212,   196,    71,    93,   585,
      80,    51,   501,    71,    99,   536,    80,    45,   164,    54,
     309,    51,    80,   322,   323,   304,    51,   299,   140,     5,
     595,   342,     6,   145,   301,   302,     5,    18,    19,     5,
       2,   217,   141,   142,   456,   287,    17,   125,   224,     5,
      31,    28,   353,    38,    17,   125,   357,   324,     5,     8,
       8,   125,    12,    59,    45,    61,    91,   125,    64,   277,
     278,   279,   280,   281,   282,   283,    26,    27,    26,   644,
       5,   646,     5,   648,    32,     5,    36,    64,    36,    60,
       5,    76,   299,     5,    75,    58,    65,   167,     5,    65,
     372,    82,    76,   167,    54,    55,    53,   165,   166,    85,
      86,    61,    74,   392,    64,     5,   325,   326,   327,   328,
      76,    95,    72,    73,    74,    75,    76,    76,    76,    76,
      80,   165,   166,    83,    48,   624,    76,    62,    58,    64,
       8,    64,   212,   124,     5,    95,    58,   712,   212,   714,
      65,   716,    59,    51,   212,    95,   196,     5,    26,    58,
       5,    38,   461,   462,    32,   372,   196,     5,    36,   209,
     210,   196,   284,    63,   273,   274,   275,   276,    16,   209,
     210,    42,    96,   494,   209,   210,     5,    48,    49,    50,
      51,    52,    53,    91,     5,   371,   463,    65,    97,    76,
       8,   490,    60,    41,    59,    64,    64,   188,    76,    64,
      58,    30,    60,    58,     7,     8,   286,    10,    26,   427,
      81,   429,   286,   431,    32,   433,    26,   435,    36,   299,
      28,     8,    76,    26,   304,   299,    36,    96,    96,    32,
     304,   299,    35,    36,    37,   821,    39,    58,   660,    26,
       5,    26,     5,    76,   775,    32,   237,    65,    58,    36,
      60,    36,   243,   244,    62,    54,    55,    48,    76,   250,
      44,    45,    25,    28,    63,    48,    76,   347,    59,    59,
      80,   346,    56,   347,   369,   370,    59,    80,   167,    89,
      90,   498,     5,   500,    26,     5,    89,    90,   196,    76,
      93,    76,   372,    77,    36,   604,   605,   288,   372,    64,
      28,   209,   210,   211,   372,    96,    59,    30,    61,   217,
      30,     5,   392,    96,   305,    50,   224,    48,   392,   354,
      76,    41,    64,    58,    76,   542,   603,     5,    59,    70,
       0,    26,    60,   533,    76,    30,    30,   759,    80,    95,
     331,    36,   333,    95,   766,   336,     5,    89,    90,    90,
      76,    92,    30,    94,    95,   666,   512,   403,   514,   100,
     612,    76,   673,    76,   409,    44,    45,   553,   554,   407,
      26,    30,   558,   795,   796,   797,   456,    56,    76,    94,
      36,    76,   456,   805,    50,    80,     5,   809,   607,   608,
     609,   610,    58,    76,    89,    90,    91,   286,    77,    50,
      50,   823,   143,   144,    23,    76,    47,    58,    58,   497,
      64,   499,    84,    54,    55,   304,   407,   497,   498,   499,
     500,    75,    76,   497,   498,   499,   500,   748,   749,   497,
     498,   499,   500,   589,    48,   591,    65,    78,   518,    54,
      55,    95,   694,    26,   518,    59,   354,    76,    72,   871,
      76,    66,    76,    36,   445,   446,   878,   448,   347,   711,
     451,     5,   542,   371,   886,   887,   965,    76,   542,   780,
      54,    55,   560,   784,   542,    12,     6,   572,   573,   220,
     560,    65,   223,   533,   225,    76,   560,   228,   229,   230,
     231,   232,   560,   533,    50,    17,   918,   774,   533,    29,
      59,    31,    58,   392,    34,   585,    54,    55,    54,    55,
       4,   585,    54,    55,    50,   595,    50,    11,    66,    65,
      42,   595,    58,    65,    58,   516,    48,    49,    50,    51,
      52,    53,    58,   621,    50,   752,    76,    63,   849,   850,
      26,   621,    58,   631,    30,    53,    59,   621,    58,    58,
      36,   631,    60,   621,    58,    65,    65,   631,    76,    81,
       8,    65,    10,   631,   644,    13,   646,   456,   648,    76,
     644,   892,   646,   894,   648,    58,   317,    76,    26,    72,
     660,   656,    65,    76,    32,    58,   660,   682,    36,    26,
      76,    39,    65,    30,    80,   906,    38,    59,    58,    36,
      60,   342,   593,    89,    90,    91,    58,   695,   349,   350,
     351,   706,    59,    65,    61,   695,    49,    59,    58,    61,
      62,   695,    64,    63,    60,   533,    58,   695,    58,   518,
     538,    17,   712,    65,   714,    65,   716,    26,   712,    76,
     714,    30,   716,    80,    58,   553,   554,    36,    58,    76,
     558,    65,    89,    90,    91,    65,    42,    59,   330,    61,
      62,    58,    48,    49,    50,    51,    52,    53,    65,   410,
     411,    76,   752,   345,    64,    48,    58,    64,   752,   759,
      53,   776,   777,    65,   752,   759,   766,    76,    58,    54,
      55,    80,   766,    63,    48,    81,   585,    58,    58,    64,
      89,    90,    91,    63,    65,    76,   595,    72,    73,    74,
      75,    76,   643,    76,    72,   795,   796,   797,    76,    76,
      38,   795,   796,   797,    42,   805,    26,   399,   400,   809,
      95,   805,   663,    76,    72,   809,    36,   728,    76,    76,
      62,   821,    64,   823,   735,    54,    55,   821,    59,   823,
      61,    62,    17,   494,   187,   644,    64,   646,    48,   648,
     193,    58,   837,    60,    72,    73,    74,    75,    76,   700,
     701,   660,    58,   848,    60,    76,    76,    42,    59,    59,
      80,    61,    59,    48,    49,    50,    51,    52,    53,    89,
      90,   871,    59,   868,   869,    48,    59,   871,   878,   540,
     541,    58,    76,    60,   878,    76,   886,   887,   241,   242,
     209,   210,   886,   887,   555,   806,    81,    44,    45,    46,
      79,    64,   897,   712,   755,   714,    58,   716,    60,    56,
      57,    59,   907,    61,    62,    76,    64,   509,   918,   927,
     928,    58,    53,    60,   918,    60,     5,   927,   928,    48,
      77,   523,    64,   927,   928,    82,    58,   932,    60,   927,
     928,     5,    58,   794,    60,    58,   941,    60,    54,    55,
     759,    76,    76,   804,    58,    76,    60,   766,    64,   954,
      28,    76,   957,   958,   959,    76,    72,    73,    74,    75,
      76,   563,   564,   565,   566,    18,   329,   972,    58,    22,
      60,    76,    76,   575,   576,    76,   795,   796,   797,    75,
      17,   344,    58,    60,    60,   846,   805,    30,    76,    42,
     809,    64,    76,   664,   665,    48,    49,    50,    51,    52,
      53,    58,   821,    60,   823,    42,    76,    76,    76,   870,
      76,    48,    49,    50,    51,    52,    53,    76,    76,    28,
     622,   623,    28,     9,    76,   627,   628,    76,    81,    76,
      92,    64,   395,   396,   397,   398,    64,    53,    64,   900,
      38,    63,    76,   406,    81,   408,    76,   908,   909,   910,
      64,    76,   871,    76,   915,    76,    76,    76,    76,   878,
      65,    59,    60,    61,    62,    76,    64,   886,   887,     3,
      68,    69,    70,    71,     5,     5,     5,   748,   749,    76,
       5,   942,     5,    60,     5,    19,    20,    21,     5,    41,
      24,    25,   763,    28,    76,    53,     5,     5,    96,   918,
     702,   703,    15,    37,    28,    62,    40,    42,    76,    76,
      58,   782,   783,    48,    49,    50,    51,    52,    53,   482,
     483,    62,     5,    30,   487,   488,    28,   729,   730,    76,
      64,   733,   734,    72,    73,    74,    75,    76,    72,   741,
     742,    75,    76,    53,    60,     5,    81,     3,    60,    42,
      84,     9,     5,    87,    88,    48,    49,    50,    51,    52,
      53,    95,    38,    19,    20,    21,     5,   838,    24,    25,
       5,     7,     8,    66,    10,    60,    60,     5,    14,    60,
      30,    37,     5,    59,    40,    61,    62,    60,    81,    60,
      26,    60,    68,    69,    70,    71,    32,    76,     5,    35,
      36,    37,    76,    39,    30,    60,   569,   570,    64,    28,
      53,    76,     5,     5,   816,   817,     5,    30,     5,    75,
      76,   892,    12,   894,    76,     5,    60,    59,    84,     5,
      60,    87,    88,     5,     5,    60,    26,    27,    60,    95,
      28,     5,    60,    63,    80,     5,    36,     5,    17,     5,
     613,   614,     5,    89,    90,   618,   619,    93,     5,    60,
      33,     5,   625,   626,    54,    55,    28,     5,    38,    42,
       5,    61,     5,     5,    64,    48,    49,    50,    51,    52,
      53,     5,    72,    73,    74,    75,    76,     5,     5,    59,
      80,    61,    62,    83,    64,     5,    64,     5,    68,    69,
      70,    71,     5,    43,     5,    95,     5,     5,    81,     5,
       5,    65,     5,   246,    54,    55,   248,   586,   891,   247,
     924,   735,    62,    63,    64,   252,    96,    67,    43,   922,
     250,   516,    72,    73,    74,    75,    76,   593,   457,    54,
      55,   680,   538,   223,   775,    -1,   779,    62,    88,    64,
      65,   411,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,   731,   732,
      -1,    -1,    62,    88,    64,    65,    -1,    67,    43,    -1,
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
      65,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    54,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,    64,
      -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,    74,
      75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    62,    88,    64,    -1,    -1,    67,    -1,    -1,
      95,    -1,    72,    73,    74,    75,    76,    -1,    -1,    44,
      45,    -1,    -1,    48,    43,    -1,    -1,    -1,    88,    54,
      55,    56,    -1,    -1,    59,    95,    55,    62,    -1,    64,
      -1,    66,    -1,    62,    -1,    64,    -1,    -1,    67,    43,
      -1,    -1,    77,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    96,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    43,    -1,    62,    88,    64,    -1,    -1,    67,    -1,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    88,
      72,    73,    74,    75,    76,    -1,    95,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    88,    48,    49,    50,
      51,    52,    53,    95,    -1,    -1,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    65,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      81,    -1,    65,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    42,    -1,    60,    -1,    -1,    81,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    81,    66,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    42,    -1,    -1,
      60,    -1,    81,    48,    49,    50,    51,    52,    53,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    63,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    -1,    -1,
      59,    -1,    61,    62,    -1,    64,    81,    -1,    -1,    68,
      69,    70,    71,   265,   266,   267,   268,   269,   270,   271,
     272,    -1,    -1,    -1,    -1,    81
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
     174,   184,    76,   184,    76,   184,     5,    65,    60,     9,
     178,   114,    76,   173,   174,   103,    76,   127,   133,   221,
     221,   221,   189,   190,   189,   204,   210,   189,    58,    65,
      62,    60,    58,    97,    92,   186,   194,   195,   186,    65,
     201,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   173,   173,   173,   173,   174,   174,    50,
      50,     5,    14,    37,   109,   173,   173,   221,   227,   224,
     224,   224,   224,   224,    63,    58,    65,    30,   184,    76,
     184,    76,   184,     5,    65,    60,    76,   133,   183,    50,
     144,   144,   143,    64,    54,    55,    63,    44,    45,    56,
      77,    65,    65,    76,    76,     5,    26,    36,    76,    76,
      76,    60,   173,   173,    65,    38,    76,   173,   173,   148,
      76,   149,    76,   114,   116,   221,     5,   129,   140,   129,
     140,   146,   174,     5,    60,   179,    60,   179,    60,   176,
       6,    76,   127,   177,     5,    76,   158,     5,     5,   174,
      60,     5,    59,     5,    18,    22,    17,   191,   143,   212,
     213,   214,   215,    41,   202,   221,   221,   140,    76,    30,
       5,    30,    41,    30,    65,    50,    65,   201,    65,   201,
     221,    65,   201,   129,    65,    53,   174,   174,   174,   174,
       5,     5,   173,   173,   186,   186,    15,   110,   174,   174,
     125,   134,    60,   179,    60,   179,    60,   181,   177,    28,
      65,   143,   144,   144,   145,   145,   145,   145,   135,   173,
     173,    76,    76,   173,   173,    62,   137,   129,   174,   174,
     146,   173,   173,   174,   174,    28,    60,   129,    60,    58,
       5,    65,    58,    63,    30,     5,   178,   177,   177,    60,
     177,    28,    59,    53,     5,   103,    60,   133,    76,     9,
     178,   221,   221,   189,    66,     5,    16,    41,   211,    58,
      60,   189,    66,    60,    17,    60,   195,   186,    65,    65,
      65,     5,     5,   129,     5,    60,    60,    60,   178,   178,
     174,   174,   186,    30,     5,    60,   135,   177,   177,    60,
     177,    76,    65,    30,   174,   174,   173,   173,   174,   174,
     175,    60,     5,    60,    30,   174,   174,   151,    76,   114,
     114,   115,    65,    76,   140,    28,   178,   165,    53,   133,
       8,    76,    76,   221,     5,   133,     5,     5,   159,    17,
      17,   143,   213,   186,   186,    30,   215,   189,   221,   221,
     189,    76,    30,     5,   178,   133,   133,   133,   167,   168,
       5,    60,    59,   178,   133,    30,   133,     5,    60,   174,
     174,     5,     5,    63,   125,   133,    60,    60,   153,   154,
     214,    28,    58,    65,    60,    63,   130,   163,   103,   221,
       5,   178,   103,   189,   189,    63,    17,   169,     5,     5,
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
       2,     1,     4,     6,     6,     7,     9,     0,     9,     0,
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
#line 2299 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2305 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2311 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2317 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2323 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2329 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2335 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2341 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2347 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2353 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2359 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2365 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2371 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2377 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2383 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2389 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2395 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2401 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2407 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2413 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2419 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2425 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2431 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2437 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2443 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2449 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2455 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2461 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2467 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2473 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2479 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2485 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2491 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2497 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2503 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2509 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2515 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2521 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2527 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2533 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2539 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2545 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2551 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2557 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2563 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2569 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2575 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2581 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2587 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2593 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2599 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2605 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2611 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2617 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2623 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2629 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2635 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2641 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2647 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 323 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2653 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 324 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2659 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 327 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2665 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 328 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2671 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 331 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2677 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 332 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2683 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 335 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2689 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 338 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2695 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 339 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2701 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 342 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2707 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 343 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2713 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2719 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 347 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2725 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 350 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2731 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2737 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2743 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 355 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2749 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 358 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2755 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 359 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2761 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 362 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2767 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 363 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2773 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 364 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2779 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 365 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2785 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 366 "pascal.y"
                                                       { }
#line 2791 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 366 "pascal.y"
                                                           { }
#line 2797 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 366 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2803 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 367 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2809 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 368 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2815 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 369 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2821 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 370 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2827 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 371 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2833 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 372 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2839 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 373 "pascal.y"
                         { }
#line 2845 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 373 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2851 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt SETSY OFSY simple_type  */
#line 374 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2857 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY  */
#line 375 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2863 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY OFSY type  */
#line 376 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2869 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY record_body ENDSY  */
#line 377 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2875 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 378 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2881 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY class_members ENDSY  */
#line 379 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2887 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 380 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2893 "pascal.tab.c"
    break;

  case 102: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 381 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2899 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 382 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2905 "pascal.tab.c"
    break;

  case 104: /* result_type: IDENT  */
#line 385 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2911 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT PERIOD IDENT  */
#line 386 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2917 "pascal.tab.c"
    break;

  case 106: /* result_type: spec_ref  */
#line 387 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2923 "pascal.tab.c"
    break;

  case 107: /* guid_opt: %empty  */
#line 390 "pascal.y"
    { (yyval.node) = NULL; }
#line 2929 "pascal.tab.c"
    break;

  case 108: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 391 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2935 "pascal.tab.c"
    break;

  case 109: /* class_members: %empty  */
#line 394 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2941 "pascal.tab.c"
    break;

  case 110: /* class_members: class_members class_member  */
#line 395 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2947 "pascal.tab.c"
    break;

  case 111: /* class_member: IDENT  */
#line 398 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2953 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT SEMICOLON  */
#line 399 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2959 "pascal.tab.c"
    break;

  case 113: /* class_member: id_list COLON type SEMICOLON  */
#line 400 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2965 "pascal.tab.c"
    break;

  case 114: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 401 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2971 "pascal.tab.c"
    break;

  case 115: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 402 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2977 "pascal.tab.c"
    break;

  case 116: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 403 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2983 "pascal.tab.c"
    break;

  case 117: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 404 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2989 "pascal.tab.c"
    break;

  case 118: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 405 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2995 "pascal.tab.c"
    break;

  case 119: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 406 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3001 "pascal.tab.c"
    break;

  case 120: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 407 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3007 "pascal.tab.c"
    break;

  case 121: /* prop_index_opt: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = NULL; }
#line 3013 "pascal.tab.c"
    break;

  case 122: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 411 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3019 "pascal.tab.c"
    break;

  case 123: /* prop_specs: %empty  */
#line 414 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3025 "pascal.tab.c"
    break;

  case 124: /* prop_specs: prop_specs IDENT IDENT  */
#line 415 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3031 "pascal.tab.c"
    break;

  case 125: /* prop_specs: prop_specs IDENT INTCONST  */
#line 416 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3037 "pascal.tab.c"
    break;

  case 126: /* packed_opt: PACKEDSY  */
#line 419 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3043 "pascal.tab.c"
    break;

  case 127: /* packed_opt: %empty  */
#line 420 "pascal.y"
     { (yyval.node) = NULL; }
#line 3049 "pascal.tab.c"
    break;

  case 128: /* simple_type: LPARENT enum_list RPARENT  */
#line 423 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3055 "pascal.tab.c"
    break;

  case 129: /* simple_type: IDENT  */
#line 424 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3061 "pascal.tab.c"
    break;

  case 130: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 425 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3067 "pascal.tab.c"
    break;

  case 131: /* simple_type: IDENT PERIOD IDENT  */
#line 426 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3073 "pascal.tab.c"
    break;

  case 132: /* simple_type: spec_ref  */
#line 427 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3079 "pascal.tab.c"
    break;

  case 133: /* simple_type: lo_bound DOTDOT b_expr  */
#line 428 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3085 "pascal.tab.c"
    break;

  case 134: /* lo_bound: lo_term  */
#line 431 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3091 "pascal.tab.c"
    break;

  case 135: /* lo_bound: PLUS lo_term  */
#line 432 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3097 "pascal.tab.c"
    break;

  case 136: /* lo_bound: MINUS lo_term  */
#line 433 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3103 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_bound PLUS b_term  */
#line 434 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3109 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_bound MINUS b_term  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3115 "pascal.tab.c"
    break;

  case 139: /* lo_term: scalar_constant  */
#line 438 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3121 "pascal.tab.c"
    break;

  case 140: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 439 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 141: /* lo_term: lo_term MUL b_factor  */
#line 440 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3133 "pascal.tab.c"
    break;

  case 142: /* lo_term: lo_term IDIV b_factor  */
#line 441 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3139 "pascal.tab.c"
    break;

  case 143: /* lo_term: lo_term IMOD b_factor  */
#line 442 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3145 "pascal.tab.c"
    break;

  case 144: /* lo_term: lo_term SHIFTOP b_factor  */
#line 443 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3151 "pascal.tab.c"
    break;

  case 145: /* b_expr: b_term  */
#line 446 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3157 "pascal.tab.c"
    break;

  case 146: /* b_expr: PLUS b_term  */
#line 447 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3163 "pascal.tab.c"
    break;

  case 147: /* b_expr: MINUS b_term  */
#line 448 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3169 "pascal.tab.c"
    break;

  case 148: /* b_expr: b_expr PLUS b_term  */
#line 449 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3175 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_expr MINUS b_term  */
#line 450 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3181 "pascal.tab.c"
    break;

  case 150: /* b_term: b_factor  */
#line 453 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3187 "pascal.tab.c"
    break;

  case 151: /* b_term: b_term MUL b_factor  */
#line 454 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3193 "pascal.tab.c"
    break;

  case 152: /* b_term: b_term IDIV b_factor  */
#line 455 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3199 "pascal.tab.c"
    break;

  case 153: /* b_term: b_term IMOD b_factor  */
#line 456 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3205 "pascal.tab.c"
    break;

  case 154: /* b_term: b_term SHIFTOP b_factor  */
#line 457 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3211 "pascal.tab.c"
    break;

  case 155: /* b_factor: scalar_constant  */
#line 460 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3217 "pascal.tab.c"
    break;

  case 156: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 461 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3223 "pascal.tab.c"
    break;

  case 157: /* b_factor: LPARENT b_expr RPARENT  */
#line 462 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3229 "pascal.tab.c"
    break;

  case 158: /* record_body: record_field_list record_case_opt  */
#line 465 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3235 "pascal.tab.c"
    break;

  case 159: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 468 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3241 "pascal.tab.c"
    break;

  case 160: /* record_field_list: record_field  */
#line 469 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3247 "pascal.tab.c"
    break;

  case 161: /* $@4: %empty  */
#line 472 "pascal.y"
                  { }
#line 3253 "pascal.tab.c"
    break;

  case 162: /* record_field: id_list COLON $@4 type  */
#line 472 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3259 "pascal.tab.c"
    break;

  case 163: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 473 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3265 "pascal.tab.c"
    break;

  case 164: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 474 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3271 "pascal.tab.c"
    break;

  case 165: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 475 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3277 "pascal.tab.c"
    break;

  case 166: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 476 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3283 "pascal.tab.c"
    break;

  case 167: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 477 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3289 "pascal.tab.c"
    break;

  case 168: /* record_field: IDENT record_field  */
#line 478 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3295 "pascal.tab.c"
    break;

  case 169: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 479 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3301 "pascal.tab.c"
    break;

  case 170: /* record_field: %empty  */
#line 480 "pascal.y"
     { (yyval.node) = NULL; }
#line 3307 "pascal.tab.c"
    break;

  case 171: /* case_arm_mark: %empty  */
#line 483 "pascal.y"
    { }
#line 3313 "pascal.tab.c"
    break;

  case 172: /* case_open: %empty  */
#line 486 "pascal.y"
    { }
#line 3319 "pascal.tab.c"
    break;

  case 173: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 489 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3325 "pascal.tab.c"
    break;

  case 174: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 490 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3331 "pascal.tab.c"
    break;

  case 175: /* record_case_opt: %empty  */
#line 491 "pascal.y"
     { (yyval.node) = NULL; }
#line 3337 "pascal.tab.c"
    break;

  case 176: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 494 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3343 "pascal.tab.c"
    break;

  case 177: /* record_case_list: record_case_arm  */
#line 495 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3349 "pascal.tab.c"
    break;

  case 178: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 498 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3355 "pascal.tab.c"
    break;

  case 179: /* record_case_arm: %empty  */
#line 499 "pascal.y"
     { (yyval.node) = NULL; }
#line 3361 "pascal.tab.c"
    break;

  case 180: /* var_decl_list: var_decl_list var_decl  */
#line 502 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3367 "pascal.tab.c"
    break;

  case 181: /* var_decl_list: var_decl  */
#line 503 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3373 "pascal.tab.c"
    break;

  case 182: /* var_decl: id_list COLON type SEMICOLON  */
#line 506 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3379 "pascal.tab.c"
    break;

  case 183: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 507 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3385 "pascal.tab.c"
    break;

  case 184: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 508 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3391 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 511 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3397 "pascal.tab.c"
    break;

  case 186: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 512 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 3403 "pascal.tab.c"
    break;

  case 187: /* $@5: %empty  */
#line 513 "pascal.y"
                                                                               { }
#line 3409 "pascal.tab.c"
    break;

  case 188: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3415 "pascal.tab.c"
    break;

  case 189: /* $@6: %empty  */
#line 514 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3421 "pascal.tab.c"
    break;

  case 190: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3427 "pascal.tab.c"
    break;

  case 191: /* $@7: %empty  */
#line 515 "pascal.y"
                                        { }
#line 3433 "pascal.tab.c"
    break;

  case 192: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 515 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3439 "pascal.tab.c"
    break;

  case 193: /* $@8: %empty  */
#line 516 "pascal.y"
                                                                                                           { }
#line 3445 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 516 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3451 "pascal.tab.c"
    break;

  case 195: /* $@9: %empty  */
#line 517 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3457 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3463 "pascal.tab.c"
    break;

  case 197: /* $@10: %empty  */
#line 518 "pascal.y"
                                                                                                 { }
#line 3469 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 518 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3475 "pascal.tab.c"
    break;

  case 199: /* $@11: %empty  */
#line 519 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3481 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 519 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3487 "pascal.tab.c"
    break;

  case 201: /* $@12: %empty  */
#line 520 "pascal.y"
                                                                                            { }
#line 3493 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3499 "pascal.tab.c"
    break;

  case 203: /* $@13: %empty  */
#line 521 "pascal.y"
                                                                                                             { }
#line 3505 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 521 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3513 "pascal.tab.c"
    break;

  case 205: /* $@14: %empty  */
#line 524 "pascal.y"
                                                                                              { }
#line 3519 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 524 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3527 "pascal.tab.c"
    break;

  case 207: /* $@15: %empty  */
#line 527 "pascal.y"
                                                                                             { }
#line 3533 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 527 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3541 "pascal.tab.c"
    break;

  case 209: /* $@16: %empty  */
#line 530 "pascal.y"
                                                                                                    { }
#line 3547 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 530 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3555 "pascal.tab.c"
    break;

  case 211: /* $@17: %empty  */
#line 533 "pascal.y"
                                                                                                                { }
#line 3561 "pascal.tab.c"
    break;

  case 212: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 533 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3570 "pascal.tab.c"
    break;

  case 213: /* $@18: %empty  */
#line 537 "pascal.y"
                                                                                                                  { }
#line 3576 "pascal.tab.c"
    break;

  case 214: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 537 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3585 "pascal.tab.c"
    break;

  case 215: /* $@19: %empty  */
#line 541 "pascal.y"
                                                                                                                     { }
#line 3591 "pascal.tab.c"
    break;

  case 216: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3599 "pascal.tab.c"
    break;

  case 217: /* pv_mark: %empty  */
#line 546 "pascal.y"
    { }
#line 3605 "pascal.tab.c"
    break;

  case 218: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 549 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3611 "pascal.tab.c"
    break;

  case 219: /* parameter_list_opt: LPARENT RPARENT  */
#line 550 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3617 "pascal.tab.c"
    break;

  case 220: /* parameter_list_opt: %empty  */
#line 551 "pascal.y"
     { (yyval.node) = NULL; }
#line 3623 "pascal.tab.c"
    break;

  case 221: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 554 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3629 "pascal.tab.c"
    break;

  case 222: /* parameter_decl_list: parameter_decl  */
#line 555 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3635 "pascal.tab.c"
    break;

  case 223: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 558 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3641 "pascal.tab.c"
    break;

  case 224: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 559 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3647 "pascal.tab.c"
    break;

  case 225: /* parameter_decl: VARSY id_list COLON param_type  */
#line 560 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3653 "pascal.tab.c"
    break;

  case 226: /* parameter_decl: id_list COLON param_type  */
#line 561 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3659 "pascal.tab.c"
    break;

  case 227: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 562 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3665 "pascal.tab.c"
    break;

  case 228: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 563 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3671 "pascal.tab.c"
    break;

  case 229: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 564 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3677 "pascal.tab.c"
    break;

  case 230: /* parameter_decl: IDENT id_list COLON param_type  */
#line 565 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3683 "pascal.tab.c"
    break;

  case 231: /* param_type: IDENT  */
#line 568 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3689 "pascal.tab.c"
    break;

  case 232: /* param_type: IDENT PERIOD IDENT  */
#line 569 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3695 "pascal.tab.c"
    break;

  case 233: /* param_type: ARRAYSY OFSY IDENT  */
#line 570 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3701 "pascal.tab.c"
    break;

  case 234: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 571 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3707 "pascal.tab.c"
    break;

  case 235: /* param_type: spec_ref  */
#line 572 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3713 "pascal.tab.c"
    break;

  case 236: /* routine_directives: %empty  */
#line 575 "pascal.y"
    { (yyval.node) = NULL; }
#line 3719 "pascal.tab.c"
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
#line 3732 "pascal.tab.c"
    break;

  case 238: /* pf_params: LPARENT pf_sections RPARENT  */
#line 586 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3738 "pascal.tab.c"
    break;

  case 239: /* pf_params: LPARENT RPARENT  */
#line 587 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3744 "pascal.tab.c"
    break;

  case 240: /* pf_params: %empty  */
#line 588 "pascal.y"
     { (yyval.node) = NULL; }
#line 3750 "pascal.tab.c"
    break;

  case 241: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 591 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3756 "pascal.tab.c"
    break;

  case 242: /* pf_sections: pf_section  */
#line 592 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3762 "pascal.tab.c"
    break;

  case 243: /* pf_section: id_list COLON param_type  */
#line 595 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3768 "pascal.tab.c"
    break;

  case 244: /* pf_section: VARSY id_list COLON param_type  */
#line 596 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3774 "pascal.tab.c"
    break;

  case 245: /* pf_section: CONSTSY id_list COLON param_type  */
#line 597 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3780 "pascal.tab.c"
    break;

  case 246: /* pf_section: IDENT id_list COLON param_type  */
#line 598 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3786 "pascal.tab.c"
    break;

  case 247: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 599 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3792 "pascal.tab.c"
    break;

  case 248: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 600 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3798 "pascal.tab.c"
    break;

  case 249: /* enum_list: enum_list COMMA enum_member  */
#line 603 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3804 "pascal.tab.c"
    break;

  case 250: /* enum_list: enum_member  */
#line 604 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3810 "pascal.tab.c"
    break;

  case 251: /* enum_member: IDENT  */
#line 607 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3816 "pascal.tab.c"
    break;

  case 252: /* id_list: id_list COMMA IDENT  */
#line 610 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3822 "pascal.tab.c"
    break;

  case 253: /* id_list: IDENT  */
#line 611 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3828 "pascal.tab.c"
    break;

  case 254: /* body: BEGINSY statement_list ENDSY  */
#line 614 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3834 "pascal.tab.c"
    break;

  case 255: /* $@20: %empty  */
#line 617 "pascal.y"
                             { pas_stmt_mark(); }
#line 3840 "pascal.tab.c"
    break;

  case 256: /* statement_list: statement_list SEMICOLON $@20 statement  */
#line 617 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3846 "pascal.tab.c"
    break;

  case 257: /* $@21: %empty  */
#line 618 "pascal.y"
     { pas_stmt_mark(); }
#line 3852 "pascal.tab.c"
    break;

  case 258: /* statement_list: $@21 statement  */
#line 618 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3858 "pascal.tab.c"
    break;

  case 259: /* $@22: %empty  */
#line 621 "pascal.y"
    { pas_stmt_mark(); }
#line 3864 "pascal.tab.c"
    break;

  case 260: /* body_stmt: $@22 statement  */
#line 621 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3870 "pascal.tab.c"
    break;

  case 261: /* statement: statement_no_label  */
#line 624 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3876 "pascal.tab.c"
    break;

  case 262: /* statement: INTCONST COLON statement_no_label  */
#line 625 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3882 "pascal.tab.c"
    break;

  case 263: /* statement: IDENT COLON statement_no_label  */
#line 626 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3888 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: assignment  */
#line 629 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3894 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: call  */
#line 630 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3900 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: compound_statement  */
#line 631 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3906 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: goto_statement  */
#line 632 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3912 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: if_statement  */
#line 633 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3918 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: case_statement  */
#line 634 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3924 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: while_statement  */
#line 635 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3930 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: repeat_statement  */
#line 636 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3936 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: for_statement  */
#line 637 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3942 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: with_statement  */
#line 638 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3948 "pascal.tab.c"
    break;

  case 274: /* statement_no_label: try_statement  */
#line 639 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3954 "pascal.tab.c"
    break;

  case 275: /* statement_no_label: raise_statement  */
#line 640 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3960 "pascal.tab.c"
    break;

  case 276: /* statement_no_label: inherited_call  */
#line 641 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3966 "pascal.tab.c"
    break;

  case 277: /* statement_no_label: INHERITEDSY  */
#line 642 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3972 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: %empty  */
#line 643 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3978 "pascal.tab.c"
    break;

  case 279: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 646 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3984 "pascal.tab.c"
    break;

  case 280: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 647 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3990 "pascal.tab.c"
    break;

  case 281: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 648 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3996 "pascal.tab.c"
    break;

  case 282: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 649 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4002 "pascal.tab.c"
    break;

  case 283: /* handler_list: handler  */
#line 652 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4008 "pascal.tab.c"
    break;

  case 284: /* handler_list: handler_list SEMICOLON handler  */
#line 653 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4014 "pascal.tab.c"
    break;

  case 285: /* handler_list: handler_list SEMICOLON  */
#line 654 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4020 "pascal.tab.c"
    break;

  case 286: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 657 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4026 "pascal.tab.c"
    break;

  case 287: /* handler: ONSY IDENT DOSY body_stmt  */
#line 658 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4032 "pascal.tab.c"
    break;

  case 288: /* raise_statement: RAISESY  */
#line 661 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4038 "pascal.tab.c"
    break;

  case 289: /* raise_statement: RAISESY expression  */
#line 662 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4044 "pascal.tab.c"
    break;

  case 290: /* inherited_call: INHERITEDSY IDENT  */
#line 665 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4050 "pascal.tab.c"
    break;

  case 291: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 666 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4056 "pascal.tab.c"
    break;

  case 292: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 667 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4062 "pascal.tab.c"
    break;

  case 293: /* call: IDENT  */
#line 670 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4068 "pascal.tab.c"
    break;

  case 294: /* call: spec_expr  */
#line 671 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4074 "pascal.tab.c"
    break;

  case 295: /* call: selector PERIOD IDENT  */
#line 672 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4080 "pascal.tab.c"
    break;

  case 296: /* call: call_with_args  */
#line 673 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4086 "pascal.tab.c"
    break;

  case 297: /* recv_primary: STRINGCONST  */
#line 676 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4092 "pascal.tab.c"
    break;

  case 298: /* recv_primary: LPARENT expression RPARENT  */
#line 677 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4098 "pascal.tab.c"
    break;

  case 299: /* recv_primary: spec_expr  */
#line 678 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4104 "pascal.tab.c"
    break;

  case 300: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 681 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4110 "pascal.tab.c"
    break;

  case 301: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 682 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4116 "pascal.tab.c"
    break;

  case 302: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 683 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4122 "pascal.tab.c"
    break;

  case 303: /* call_with_args: IDENT LPARENT RPARENT  */
#line 684 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4128 "pascal.tab.c"
    break;

  case 304: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 685 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4134 "pascal.tab.c"
    break;

  case 305: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 686 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4140 "pascal.tab.c"
    break;

  case 306: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 687 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4146 "pascal.tab.c"
    break;

  case 307: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 688 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4152 "pascal.tab.c"
    break;

  case 308: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 689 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4158 "pascal.tab.c"
    break;

  case 309: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 690 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4164 "pascal.tab.c"
    break;

  case 310: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 691 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4170 "pascal.tab.c"
    break;

  case 311: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 692 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4176 "pascal.tab.c"
    break;

  case 312: /* argument_list: argument_list COMMA argument  */
#line 695 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4182 "pascal.tab.c"
    break;

  case 313: /* argument_list: argument  */
#line 696 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4188 "pascal.tab.c"
    break;

  case 314: /* argument: expression  */
#line 699 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4194 "pascal.tab.c"
    break;

  case 315: /* argument: expression COLON expression  */
#line 700 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4200 "pascal.tab.c"
    break;

  case 316: /* argument: expression COLON expression COLON expression  */
#line 701 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4206 "pascal.tab.c"
    break;

  case 317: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 702 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4212 "pascal.tab.c"
    break;

  case 318: /* assignment: selector BECOMES expression  */
#line 705 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4218 "pascal.tab.c"
    break;

  case 319: /* assignment: selector ADDBECOMES expression  */
#line 706 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4224 "pascal.tab.c"
    break;

  case 320: /* assignment: selector SUBBECOMES expression  */
#line 707 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4230 "pascal.tab.c"
    break;

  case 321: /* assignment: selector MULBECOMES expression  */
#line 708 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4236 "pascal.tab.c"
    break;

  case 322: /* assignment: selector DIVBECOMES expression  */
#line 709 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4242 "pascal.tab.c"
    break;

  case 323: /* assignment: call_with_args BECOMES expression  */
#line 710 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4248 "pascal.tab.c"
    break;

  case 324: /* selector: selector LBRACK expression_list RBRACK  */
#line 713 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4254 "pascal.tab.c"
    break;

  case 325: /* selector: selector PERIOD IDENT  */
#line 714 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4260 "pascal.tab.c"
    break;

  case 326: /* selector: selector ARROW  */
#line 715 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4266 "pascal.tab.c"
    break;

  case 327: /* selector: call_with_args PERIOD IDENT  */
#line 716 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4272 "pascal.tab.c"
    break;

  case 328: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 717 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4278 "pascal.tab.c"
    break;

  case 329: /* selector: call_with_args ARROW  */
#line 718 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4284 "pascal.tab.c"
    break;

  case 330: /* selector: recv_primary PERIOD IDENT  */
#line 719 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4290 "pascal.tab.c"
    break;

  case 331: /* selector: recv_primary ARROW  */
#line 720 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4296 "pascal.tab.c"
    break;

  case 332: /* selector: IDENT  */
#line 721 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4302 "pascal.tab.c"
    break;

  case 333: /* expression_list: expression_list COMMA expression  */
#line 724 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4308 "pascal.tab.c"
    break;

  case 334: /* expression_list: expression  */
#line 725 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4314 "pascal.tab.c"
    break;

  case 335: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 728 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4320 "pascal.tab.c"
    break;

  case 336: /* goto_statement: GOTOSY INTCONST  */
#line 731 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4326 "pascal.tab.c"
    break;

  case 337: /* goto_statement: GOTOSY IDENT  */
#line 732 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4332 "pascal.tab.c"
    break;

  case 338: /* if_statement: IFSY expression THENSY body_stmt  */
#line 735 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4338 "pascal.tab.c"
    break;

  case 339: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 736 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4344 "pascal.tab.c"
    break;

  case 340: /* $@23: %empty  */
#line 739 "pascal.y"
                           { }
#line 4350 "pascal.tab.c"
    break;

  case 341: /* case_statement: CASESY expression OFSY $@23 case_list case_tail ENDSY  */
#line 739 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4356 "pascal.tab.c"
    break;

  case 342: /* case_tail: %empty  */
#line 742 "pascal.y"
    { (yyval.node) = NULL; }
#line 4362 "pascal.tab.c"
    break;

  case 343: /* case_tail: ELSESY statement_list  */
#line 743 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4368 "pascal.tab.c"
    break;

  case 344: /* case_tail: OTHERWISESY statement_list  */
#line 744 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4374 "pascal.tab.c"
    break;

  case 345: /* case_list: case_list SEMICOLON case_elem  */
#line 747 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4380 "pascal.tab.c"
    break;

  case 346: /* case_list: case_elem  */
#line 748 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4386 "pascal.tab.c"
    break;

  case 347: /* case_elem: constant_list COLON body_stmt  */
#line 751 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4392 "pascal.tab.c"
    break;

  case 348: /* case_elem: %empty  */
#line 752 "pascal.y"
     { (yyval.node) = NULL; }
#line 4398 "pascal.tab.c"
    break;

  case 349: /* constant_list: constant_list COMMA case_label  */
#line 755 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4404 "pascal.tab.c"
    break;

  case 350: /* constant_list: case_label  */
#line 756 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4410 "pascal.tab.c"
    break;

  case 351: /* case_label: b_expr  */
#line 759 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4416 "pascal.tab.c"
    break;

  case 352: /* case_label: b_expr DOTDOT b_expr  */
#line 760 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4422 "pascal.tab.c"
    break;

  case 353: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 763 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4428 "pascal.tab.c"
    break;

  case 354: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 766 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4434 "pascal.tab.c"
    break;

  case 355: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 769 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4440 "pascal.tab.c"
    break;

  case 356: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 770 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4446 "pascal.tab.c"
    break;

  case 357: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 771 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4452 "pascal.tab.c"
    break;

  case 358: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 774 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4458 "pascal.tab.c"
    break;

  case 359: /* with_open: with_open COMMA selector  */
#line 777 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4464 "pascal.tab.c"
    break;

  case 360: /* with_open: selector  */
#line 778 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4470 "pascal.tab.c"
    break;

  case 361: /* expression: simple_expression  */
#line 785 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4476 "pascal.tab.c"
    break;

  case 362: /* expression: expression INOP simple_expression  */
#line 786 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4482 "pascal.tab.c"
    break;

  case 363: /* expression: expression LTOP simple_expression  */
#line 787 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4488 "pascal.tab.c"
    break;

  case 364: /* expression: expression LEOP simple_expression  */
#line 788 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4494 "pascal.tab.c"
    break;

  case 365: /* expression: expression GTOP simple_expression  */
#line 789 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4500 "pascal.tab.c"
    break;

  case 366: /* expression: expression GEOP simple_expression  */
#line 790 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4506 "pascal.tab.c"
    break;

  case 367: /* expression: expression NEOP simple_expression  */
#line 791 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4512 "pascal.tab.c"
    break;

  case 368: /* expression: expression EQOP simple_expression  */
#line 792 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4518 "pascal.tab.c"
    break;

  case 369: /* expression: expression ISOP simple_expression  */
#line 793 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4524 "pascal.tab.c"
    break;

  case 370: /* simple_expression: term  */
#line 796 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4530 "pascal.tab.c"
    break;

  case 371: /* simple_expression: PLUS term  */
#line 797 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4536 "pascal.tab.c"
    break;

  case 372: /* simple_expression: MINUS term  */
#line 798 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4542 "pascal.tab.c"
    break;

  case 373: /* simple_expression: simple_expression PLUS term  */
#line 799 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4548 "pascal.tab.c"
    break;

  case 374: /* simple_expression: simple_expression MINUS term  */
#line 800 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4554 "pascal.tab.c"
    break;

  case 375: /* simple_expression: simple_expression OROP term  */
#line 801 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4560 "pascal.tab.c"
    break;

  case 376: /* simple_expression: simple_expression XOROP term  */
#line 802 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4566 "pascal.tab.c"
    break;

  case 377: /* term: power  */
#line 805 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4572 "pascal.tab.c"
    break;

  case 378: /* term: term MUL power  */
#line 806 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4578 "pascal.tab.c"
    break;

  case 379: /* term: term RDIV power  */
#line 807 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4584 "pascal.tab.c"
    break;

  case 380: /* term: term IDIV power  */
#line 808 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4590 "pascal.tab.c"
    break;

  case 381: /* term: term IMOD power  */
#line 809 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4596 "pascal.tab.c"
    break;

  case 382: /* term: term ANDOP power  */
#line 810 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4602 "pascal.tab.c"
    break;

  case 383: /* term: term MUL MINUS power  */
#line 811 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4608 "pascal.tab.c"
    break;

  case 384: /* term: term RDIV MINUS power  */
#line 812 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4614 "pascal.tab.c"
    break;

  case 385: /* term: term IDIV MINUS power  */
#line 813 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4620 "pascal.tab.c"
    break;

  case 386: /* term: term IMOD MINUS power  */
#line 814 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4626 "pascal.tab.c"
    break;

  case 387: /* term: term ANDOP MINUS power  */
#line 815 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4632 "pascal.tab.c"
    break;

  case 388: /* term: term SHIFTOP power  */
#line 816 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4638 "pascal.tab.c"
    break;

  case 389: /* term: term ASOP power  */
#line 817 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4644 "pascal.tab.c"
    break;

  case 390: /* power: factor  */
#line 820 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4650 "pascal.tab.c"
    break;

  case 391: /* power: power POWOP factor  */
#line 821 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4656 "pascal.tab.c"
    break;

  case 392: /* factor: selector  */
#line 824 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4662 "pascal.tab.c"
    break;

  case 393: /* factor: call_with_args  */
#line 825 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4668 "pascal.tab.c"
    break;

  case 394: /* factor: INTCONST  */
#line 826 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4674 "pascal.tab.c"
    break;

  case 395: /* factor: REALCONST  */
#line 827 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4680 "pascal.tab.c"
    break;

  case 396: /* factor: STRINGCONST  */
#line 828 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4686 "pascal.tab.c"
    break;

  case 397: /* factor: CHARCODE  */
#line 829 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4692 "pascal.tab.c"
    break;

  case 398: /* factor: LPARENT expression RPARENT  */
#line 830 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4698 "pascal.tab.c"
    break;

  case 399: /* factor: inherited_call  */
#line 831 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4704 "pascal.tab.c"
    break;

  case 400: /* factor: NOTSY factor  */
#line 832 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4710 "pascal.tab.c"
    break;

  case 401: /* factor: ATSIGN factor  */
#line 833 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4716 "pascal.tab.c"
    break;

  case 402: /* factor: LBRACK RBRACK  */
#line 834 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4722 "pascal.tab.c"
    break;

  case 403: /* factor: LBRACK set_member_list RBRACK  */
#line 835 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4728 "pascal.tab.c"
    break;

  case 404: /* set_member_list: set_member  */
#line 838 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4734 "pascal.tab.c"
    break;

  case 405: /* set_member_list: set_member_list COMMA set_member  */
#line 839 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4740 "pascal.tab.c"
    break;

  case 406: /* set_member: expression  */
#line 842 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4746 "pascal.tab.c"
    break;

  case 407: /* set_member: expression DOTDOT expression  */
#line 843 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4752 "pascal.tab.c"
    break;


#line 4756 "pascal.tab.c"

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
