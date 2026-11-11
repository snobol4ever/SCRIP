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
#define YYLAST   1848

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  129
/* YYNRULES -- Number of rules.  */
#define YYNRULES  402
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  966

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
     362,   363,   364,   365,   365,   365,   366,   367,   368,   369,
     370,   371,   372,   372,   373,   374,   375,   376,   377,   378,
     379,   382,   383,   384,   387,   388,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   403,   404,   407,   408,
     409,   412,   413,   416,   417,   418,   419,   420,   421,   424,
     425,   426,   427,   428,   431,   432,   433,   434,   435,   436,
     439,   440,   441,   442,   443,   446,   447,   448,   449,   450,
     453,   454,   455,   458,   461,   462,   465,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   476,   479,   482,   483,
     484,   487,   488,   491,   492,   495,   496,   499,   500,   501,
     504,   505,   506,   506,   507,   507,   508,   508,   509,   509,
     510,   510,   511,   511,   512,   512,   513,   513,   514,   514,
     517,   517,   520,   520,   523,   523,   526,   526,   530,   530,
     534,   534,   539,   542,   543,   544,   547,   548,   551,   552,
     553,   554,   555,   556,   557,   558,   561,   562,   563,   564,
     565,   568,   569,   579,   580,   581,   584,   585,   588,   589,
     590,   591,   592,   593,   596,   597,   600,   603,   604,   607,
     610,   610,   611,   611,   614,   614,   617,   618,   619,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   639,   640,   641,   642,   645,   646,
     647,   650,   651,   654,   655,   658,   659,   660,   663,   664,
     665,   666,   669,   670,   671,   674,   675,   676,   677,   678,
     679,   680,   681,   682,   683,   684,   685,   688,   689,   692,
     693,   694,   695,   698,   699,   700,   701,   702,   703,   706,
     707,   708,   709,   710,   711,   712,   713,   714,   717,   718,
     721,   724,   725,   728,   729,   732,   732,   735,   736,   737,
     740,   741,   744,   745,   748,   749,   752,   753,   756,   759,
     762,   763,   764,   767,   770,   771,   778,   779,   780,   781,
     782,   783,   784,   785,   786,   789,   790,   791,   792,   793,
     794,   795,   798,   799,   800,   801,   802,   803,   804,   805,
     806,   807,   808,   809,   810,   813,   814,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   831,
     832,   835,   836
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

#define YYPACT_NINF (-579)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-328)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     484,     3,   167,   290,  -579,    72,  1085,  -579,  -579,     1,
    -579,    38,  -579,  -579,   294,   193,   167,   217,   230,   230,
     273,  -579,   172,   128,   291,   397,   343,  -579,  -579,  -579,
     413,   230,   496,   525,   482,  -579,  -579,    75,   464,   193,
    -579,    76,   543,    10,  -579,   230,  -579,   489,    79,   236,
     143,   972,   302,   504,   172,  -579,   537,   538,   546,   582,
     588,   586,   587,  -579,   138,  -579,  -579,  -579,  -579,   403,
    1421,   612,  -579,  -579,   167,   230,   595,   615,  -579,   596,
     612,  -579,   230,   618,    71,  -579,  -579,   605,   622,  -579,
    1421,   -12,  1421,  -579,  1421,  1421,   648,  -579,  1080,  -579,
    1421,   626,   650,  -579,   533,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,   655,   344,  -579,   849,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   230,  1144,   670,  -579,   668,   682,
     691,   679,   686,   715,   719,  -579,   713,   683,  -579,  -579,
    1613,  1613,  1613,  1189,  1421,  1613,  -579,  -579,  -579,   688,
     154,   626,   709,  -579,   642,   593,   160,   512,   452,   702,
    -579,   720,  -579,   720,   704,   704,   185,   714,  -579,  -579,
    -579,  -579,  1009,   163,   402,  -579,  -579,  -579,  -579,   741,
     440,  -579,   299,   265,   543,   357,  -579,   213,   793,  -579,
      17,   359,  -579,  -579,   752,   972,  -579,  -579,   604,   300,
     470,   642,   593,   126,  1161,   170,  1661,  1142,  1059,  1059,
    1224,   756,    41,  1767,   772,   777,  1261,   764,  -579,  1421,
     765,  -579,  1421,  1286,  1421,   768,  -579,  1421,  1421,  1421,
    1421,  1421,   364,   131,   848,   230,   778,   782,   144,  -579,
    -579,   230,   230,  -579,   193,   167,  -579,   783,   230,   784,
     172,  -579,  -579,   452,   452,  -579,  1673,   353,  -579,  1685,
    -579,   786,  -579,  1421,  1421,  1421,  1421,  1421,  1421,  1421,
    1421,  1613,  1613,  1613,  1613,  1496,  1521,  1546,  1571,  1596,
    1613,  1613,  1613,   239,   838,   807,   805,   265,   265,    34,
    -579,  -579,   488,  -579,   756,   797,   909,   909,   800,   205,
     282,   812,   813,   814,   197,   815,   817,   818,   867,    62,
    -579,   693,  1446,   173,   870,   874,  -579,   380,   380,   909,
     380,   380,   380,   380,  -579,   615,   230,   827,   230,   828,
    -579,   230,    33,  -579,   696,  -579,  -579,  1446,   831,  -579,
     615,  -579,   222,  -579,  1421,  1421,  1421,  -579,  -579,   -12,
    -579,  -579,  -579,  -579,   949,  -579,  -579,  -579,   511,  -579,
     678,  1707,   122,  -579,   821,  -579,  1311,   756,  -579,   521,
     851,  1767,   858,   526,  1767,  -579,   523,  1767,   322,   540,
    1767,  1767,  1767,  1767,   872,  -579,   205,  -579,   400,  -579,
    -579,  -579,  -579,   615,   615,   407,   477,   193,    80,  1093,
    -579,   230,  -579,   172,  1421,  1421,  -579,   700,   859,   512,
     512,   512,   512,   512,   512,   512,   512,   452,   452,   452,
     452,  1613,   702,  1613,   702,  1613,   702,  1613,   702,  1613,
     702,   702,   702,  -579,   230,   857,   230,   860,  -579,   230,
      63,  -579,   706,   862,   222,   714,  -579,   481,  -579,   380,
     380,   909,   871,  -579,   519,   269,  -579,   439,  -579,    34,
     542,  -579,  -579,   863,  -579,   864,    84,   368,   866,   868,
     873,  -579,   727,  -579,  -579,   869,  -579,   156,  -579,  -579,
    -579,   387,   875,  -579,  -579,  1471,   938,  1767,   612,   756,
     612,   756,   387,   269,   269,   333,  -579,  -579,  -579,  -579,
     615,   945,   733,   720,   739,   720,   745,   114,  -579,    19,
      45,   950,   952,   615,   899,   961,   103,  -579,   971,   919,
     608,  1767,  -579,   972,  -579,   593,   909,   937,  1421,  -579,
    1421,  1421,   756,  -579,   914,   192,   229,  -579,   210,  -579,
     558,   506,  -579,  1346,  1371,  1421,  -579,  -579,  1396,  -579,
     612,   559,   941,   615,   615,   615,   615,   990,   993,  -579,
    -579,  -579,  -579,  -579,   984,   615,   615,  1767,  -579,   702,
     702,   702,   702,   702,   749,   720,   757,   720,   761,   256,
    -579,    19,  -579,   973,  -579,  -579,   269,   269,   471,   909,
     380,   380,  -579,   380,   380,   380,   380,  -579,   205,  -579,
    -579,  -579,  -579,   926,   927,  -579,  -579,   942,   612,   615,
     615,   387,  -579,  -579,   615,   615,  -579,   208,   612,    85,
     947,    67,  1685,  -579,  -579,   574,  -579,  -579,   976,  1002,
    -579,    19,  -579,    19,   954,    19,  -579,   987,   166,  -579,
     963,  1017,  1018,  -579,  -579,  -579,   964,   222,  -579,   962,
    -579,  1421,  1421,  -579,  -579,   379,   258,  -579,   766,  -579,
    -579,  -579,  1720,  1732,  -579,    31,  -579,   821,  -579,  -579,
    -579,  -579,  -579,  -579,   561,  -579,   570,  1767,  -579,   575,
    1020,  1021,   612,  1023,   977,   979,   980,  -579,  -579,   615,
     615,  1038,  1038,  -579,  1019,  1045,   991,    19,  -579,    19,
     992,    19,  -579,  -579,   982,  -579,   487,   269,   269,  -579,
    -579,  -579,  -579,  -579,   309,   615,   615,  -579,  -579,   615,
     615,   114,   995,  1056,  -579,  1006,  1039,   615,   615,  -579,
    -579,  -579,   994,  -579,  1446,  1446,   268,  -579,   756,  1044,
    -579,  -579,  1000,  1024,  -579,   222,  -579,    37,  1005,  1421,
    -579,  -579,  1077,   222,  1082,  -579,    53,   833,   879,  -579,
     909,   909,  -579,  -579,  1055,   909,  -579,  -579,  1421,  1421,
    -579,  1012,  -579,   212,  -579,  -579,  -579,  -579,  -579,  1084,
    -579,   222,   222,   222,  1000,  1000,  1086,  1030,  1038,  1035,
    -579,   222,  -579,  -579,   222,  -579,  -579,  -579,  -579,  1092,
    1042,   615,   615,  1099,  1101,    54,   205,  -579,   222,  -579,
    1048,  1049,   909,  1070,  -579,  -579,   578,  -579,  1050,  1051,
    -579,  1000,  -579,  1421,  -579,  -579,  -579,  -579,  1767,  -579,
    1107,  -579,  1108,  -579,  -579,  -579,   333,  -579,  1038,  1038,
    -579,  -579,  -579,  1745,  1767,  -579,  1109,  -579,  -579,  1000,
    1122,  1126,  1128,  -579,  -579,  -579,   222,  -579,  1000,  1131,
    -579,  -579,   222,  1132,  1078,  -579,  -579,  -579,  -579,  -579,
     222,   222,  1140,  -579,   779,  -579,  1446,  -579,  1446,  1130,
    -579,  -579,  1150,  1767,  -579,  1000,  -579,  1154,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  1155,  1158,  1000,  1159,
    -579,  1162,  -579,   222,    23,  -579,  -579,   909,  1102,   909,
    -579,  -579,   612,   612,  1167,  -579,  1000,  -579,  -579,  -579,
    1172,  1000,  1000,  1000,  -579,  -579,  -579,  -579,  1000,  -579,
    1174,  -579,   606,  -579,  -579,  1140,  -579,  -579,  -579,  -579,
    1176,  -579,  -579,  -579,  -579,  1180,  1000,  -579,  -579,  -579,
     387,  1182,  -579,  1183,  1191,  1192,  -579,  -579,  1135,  -579,
    -579,  -579,  -579,  1196,  -579,  -579
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      64,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   252,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    62,   212,   248,    16,   176,     0,     0,   212,
       0,   273,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    65,    13,     0,
       0,   122,    38,    18,     0,     0,     0,   215,   175,     0,
     122,    19,     0,     0,   215,   250,   249,     0,     0,   252,
       0,     0,     0,   252,     0,     0,     0,   292,   288,   252,
     283,   272,     0,    68,   289,    67,   253,   256,   269,   270,
     271,   260,     0,   291,   259,     0,   261,   262,   263,   264,
     265,   266,   267,   268,     0,   122,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   389,   392,   390,   391,
     327,     0,   294,   394,   388,   387,     0,   356,   365,   372,
     385,   235,   121,   235,     0,     0,     0,     0,    51,    54,
      52,    53,   124,   104,   165,   134,    75,    74,   127,     0,
       0,    79,     0,   129,    63,     0,   212,     0,     0,   247,
       0,     0,   212,   186,     0,   273,   331,   332,     0,     0,
       0,     0,   355,     0,     0,     0,     0,     0,   273,   273,
       0,     0,     0,   284,   285,     0,     0,     0,   326,     0,
       0,   324,     0,     0,     0,     0,   321,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,   212,
     212,     0,     0,     4,     0,     0,    12,     0,     0,     0,
       0,    21,   395,   366,   367,   397,   401,     0,   399,     0,
     396,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,     0,    50,   130,   131,    80,
      82,   246,     0,   245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,   170,
     155,     0,     0,     0,    95,     0,    92,     0,     0,     0,
       0,     0,     0,     0,   212,   215,     0,     0,     0,     0,
     214,   248,     0,   217,     0,   231,   177,     0,     0,   212,
     215,    12,     0,   251,     0,     0,     0,   254,   254,     0,
     335,   330,   254,   293,   288,   257,   258,   298,     0,   308,
     387,   309,     0,    77,   252,   252,     0,     0,   297,     0,
     325,   318,   322,     0,   329,   300,     0,   313,   290,     0,
     314,   315,   316,   317,     0,    60,     0,    57,     0,   212,
     212,   212,   212,   215,   215,     0,     0,    24,     0,    31,
     212,    26,   212,    25,     0,     0,   398,   393,   320,   357,
     358,   359,   360,   361,   362,   363,   364,   370,   368,   369,
     371,     0,   375,     0,   376,     0,   377,     0,   373,     0,
     374,   383,   384,   386,     0,     0,     0,     0,   234,   248,
       0,   237,     0,     0,     0,     0,   123,     0,   126,     0,
       0,     0,    50,   150,     0,   140,   145,     0,    81,    71,
       0,    69,    72,     0,    99,     0,   106,     0,     0,     0,
       0,   105,     0,   212,   212,     0,   163,     0,   212,   212,
      97,   165,     0,   153,   156,     0,     0,    42,   122,     0,
     122,     0,   165,   132,   133,   128,   137,   138,   136,   139,
     215,     0,     0,   235,     0,   235,     0,     0,   213,     0,
     182,     0,     0,   215,     0,     0,   101,   103,     0,     0,
       0,   349,   348,   273,   353,   354,   343,   333,     0,   295,
       0,     0,     0,    66,     0,     0,     0,   278,     0,   286,
       0,     0,   296,     0,     0,     0,   323,   299,     0,   319,
     122,     0,     0,   215,   215,   215,   215,     0,     0,   212,
     212,    23,   252,   252,    33,   215,   215,   402,   400,   380,
     381,   382,   378,   379,     0,   235,     0,   235,     0,     0,
     233,     0,    88,    89,   244,    76,   141,   142,     0,     0,
       0,     0,   125,     0,     0,     0,     0,   135,     0,   104,
     212,   212,   107,     0,     0,   212,   212,   116,   122,   215,
     215,   165,   212,   212,   215,   215,   154,     0,   122,   327,
       0,     0,    42,    41,    86,     0,    96,    94,     0,     0,
     231,     0,   218,     0,     0,     0,   216,     0,   226,   230,
     221,     0,     0,    12,   178,   179,     0,     0,   187,     0,
     231,     0,     0,   254,   255,   346,   337,   341,     0,   345,
     254,   307,   329,   310,    78,     0,   274,   280,   275,   252,
     277,   287,    73,   306,     0,   304,     0,   328,   302,     0,
       0,   104,   122,     0,     0,     0,     0,   231,   231,   215,
     215,    29,    30,   252,     0,     0,     0,     0,   242,     0,
       0,     0,   236,   238,     0,   152,     0,   143,   144,   147,
     148,   146,   149,    70,     0,   215,   215,   212,   212,   215,
     215,     0,     0,     0,   158,     0,     0,   215,   215,   160,
     161,   167,     0,   157,     0,     0,     0,    44,     0,     0,
      93,   231,   196,   222,   220,     0,   225,     0,     0,     0,
     180,   232,     0,     0,     0,   102,   184,     0,     0,   352,
       0,   343,   252,   252,     0,     0,   254,   334,     0,     0,
     254,     0,   279,     0,   305,   303,   301,    59,    61,     0,
     231,     0,     0,     0,   200,   202,     0,     0,    32,     0,
     231,     0,   240,   239,     0,   241,    90,   151,   100,     0,
       0,   215,   215,     0,     0,     0,     0,   108,     0,    98,
       0,     0,   174,     0,    48,    46,     0,    45,     0,     0,
      83,   192,    12,     0,   219,   229,   228,   227,   223,   183,
       0,   231,     0,    12,   254,   254,   347,   340,   339,   338,
     336,   344,   342,     0,   311,   282,     0,   276,    58,   204,
       0,     0,     0,    12,    12,   231,     0,    20,    27,     0,
     243,   109,     0,     0,     0,   111,   112,   117,   118,   159,
       0,     0,   169,   172,     0,   167,     0,    43,     0,     0,
      84,    12,     0,   224,   231,   198,   181,     0,   351,   350,
     312,   254,    12,   231,   231,   231,     0,     0,   188,     0,
     231,     0,   113,     0,     0,   164,   162,   174,     0,   174,
      47,    49,   122,   122,     0,   197,   194,    12,   185,   281,
       0,   210,   208,   206,   201,   203,    12,   231,    28,   110,
       0,   115,     0,   171,   166,   168,    91,    85,   193,    12,
       0,   205,    12,    12,    12,     0,   190,   114,   120,   119,
     165,     0,   199,     0,     0,     0,   189,    12,     0,   195,
     211,   209,   207,     0,   173,   191
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -579,  -579,  -579,  -579,  -579,   -65,   956,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   960,   -36,  -333,  -579,  -579,   -64,
     965,   -35,   981,    42,   -70,   -40,   820,  -578,   -48,   171,
    -252,   -78,  -579,  -579,  -579,   118,  -562,  -579,  -579,  -579,
    -579,  -206,  -579,   272,  -246,  -243,  -218,  -486,  -579,  -283,
    -579,  -579,   346,  -579,   313,   318,   983,   -37,  -579,  -579,
    -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   556,    55,   507,   722,  -541,  -564,
    -146,  -579,   651,  -579,   788,    -4,  -579,   -63,  -579,  -579,
    -319,  -579,  -183,   251,  -579,  -579,   567,  -579,   -16,  -579,
    -579,   -42,  -192,   710,  -579,   -38,  1015,  -579,  -579,  -579,
    -579,  -579,  -579,  -579,   479,  -508,   476,  -579,  -579,  -579,
    -579,  -579,   501,  1127,   -85,  -169,  -106,  -579,   840
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     251,   564,   694,    37,    39,    40,   486,   816,   621,   453,
      54,    55,    41,    42,   103,   152,   460,   461,   105,   178,
     362,   179,   880,   913,   492,   518,   300,   471,   722,   904,
     180,   181,   182,   183,   655,   455,   456,   308,   309,   310,
     618,   950,   812,   483,   872,   873,    45,    46,    28,   643,
     833,   341,   926,   957,   881,   939,   822,   917,   853,   854,
     892,   944,   943,   942,    77,   188,   332,   333,   640,   510,
     284,   440,   441,   292,   293,   311,    29,    50,   195,    51,
     522,   523,   106,   107,   108,   536,   537,   109,   153,   111,
     112,   154,   358,   359,   114,   155,   379,   116,   117,   118,
     119,   526,   764,   656,   657,   874,   659,   120,   121,   122,
     123,   203,   361,   157,   158,   159,   160,   257,   258
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   176,   190,    72,   511,   363,   628,   175,    78,   113,
     176,   104,   343,   115,    47,    48,   175,   285,   658,   127,
     713,   476,   336,   177,   369,   637,   199,    64,   931,   524,
     205,   376,   177,   527,   252,   110,   212,   714,   507,   260,
     703,    47,   447,    33,    11,   825,    85,   234,   770,   201,
     454,   457,    95,   202,   641,   176,   253,   254,    75,   507,
      30,   175,   832,    97,   150,    31,   742,   481,   579,    76,
     337,   185,   736,   495,   493,   494,   193,   177,   191,     8,
      68,    73,   294,   102,    81,   561,   756,   482,   363,   602,
     743,   771,   744,   338,   746,   638,   176,    34,   508,   932,
     175,   175,   496,   497,   498,   499,   422,   424,   426,   428,
     430,   431,   432,   826,   102,   541,   184,   867,   177,   714,
     232,   642,   326,   784,   785,   726,   364,   365,   580,   642,
     211,    13,   737,    69,    74,   187,   385,    79,    74,   194,
     327,   176,  -248,   348,  -248,   734,   328,   175,    85,   210,
     329,   294,   620,   113,    56,   104,   792,   115,   793,   298,
     795,   363,   649,   177,    57,   262,   113,   113,   104,   104,
     115,   115,   360,    86,   540,    85,   433,   821,   360,   110,
     532,   211,   391,   334,   349,   360,   417,   418,   419,   420,
     331,   298,   110,   110,   612,   386,    79,    85,   616,   211,
     351,   488,   263,   135,    58,   588,   586,   587,   264,   265,
     266,   267,   268,   269,   294,    85,   849,    85,   210,   533,
     392,   326,   666,   301,   176,   748,   858,   299,   868,   176,
     175,   388,   613,   302,   667,   489,   731,   395,   396,   327,
     670,   270,   847,    10,    47,   328,   177,   434,    52,   329,
     211,   177,   569,   658,   570,  -248,   571,  -248,   572,   668,
     573,   289,   211,   761,   434,   435,    53,   885,   732,    38,
     669,   436,   176,   304,   762,   437,   515,   305,   330,   442,
     102,   459,   435,   625,    82,   627,   306,   307,   436,   331,
      12,   898,   437,    43,   177,    83,   472,   176,   516,   763,
     102,   535,   538,   175,   438,    85,    44,   201,   463,   320,
     321,   525,   464,   593,   594,   439,   176,   102,   465,   177,
     916,   322,   502,   346,   504,   595,   664,   506,   360,   921,
     922,   923,   439,   817,   759,   463,   928,   290,   177,   798,
     654,   767,   323,   706,   818,   465,   596,   707,   708,    49,
     124,   674,   676,   317,   318,   125,   679,   632,   466,   634,
    -320,    72,   467,   946,    78,   319,    35,    59,   127,    61,
      36,   468,   469,   470,   176,   709,   710,   711,   712,    62,
     501,  -320,   219,  -320,  -320,   466,   548,   590,   591,   467,
    -320,  -320,  -320,  -320,   603,   514,   177,    47,   468,   469,
     470,   814,   815,   220,   604,   221,   222,   324,   223,   339,
     624,   405,   626,   301,   384,    79,   406,    79,   176,   176,
     176,   176,    79,   302,   175,   175,   175,   175,   301,   698,
     574,   700,   576,   590,   591,   578,   287,   288,   302,   176,
     177,   177,   177,   177,   451,   760,   313,   842,   557,   558,
     552,   845,   168,   169,   170,   171,   452,   559,    79,   355,
     356,   177,   176,   304,   958,    79,   303,   305,   175,   314,
     462,   315,   680,    60,   316,   138,   306,   307,   304,   139,
     176,   113,   305,   104,   177,   115,   175,   347,     1,    63,
     360,   306,   307,   590,   591,     2,   275,   276,   277,   691,
     692,    65,   177,   334,   597,   360,   360,   110,   278,   279,
     360,   176,   263,   517,   836,   888,   889,    70,   264,   265,
     266,   267,   268,   269,    71,   590,   591,   560,   176,   280,
     723,   585,   819,   177,   281,    79,   705,    66,   176,   532,
     733,   590,   591,   910,   175,   911,   445,    79,   176,    80,
     177,   270,   797,   446,   175,   629,   672,   462,    67,   271,
     177,   176,   583,   176,   532,   176,   272,   273,   646,   528,
     177,   156,   919,   590,   591,   442,   529,   176,   752,   528,
     126,   528,   592,   177,   545,   177,   542,   177,   547,   546,
     274,   200,  -294,   204,  -294,   206,   207,   216,   545,   177,
     598,   213,    34,   549,   779,    84,   773,   599,   683,   684,
     685,   686,   176,   128,   129,   517,   528,   598,   175,   528,
     695,   696,   130,   671,   681,   653,   774,   176,   528,   176,
     788,   176,   738,   528,   177,   775,   876,   739,   161,   162,
     776,   131,   344,   877,   256,   259,   345,   132,   163,   177,
     263,   177,   261,   177,   226,   227,   264,   265,   266,   267,
     268,   269,   133,   134,   724,   725,   164,   165,   176,   729,
     730,   186,   189,   166,   175,   176,   167,   196,   948,   187,
     639,   197,   949,   176,   168,   169,   170,   171,   172,   270,
     177,   244,   173,   245,   192,   174,   246,   177,   198,   838,
     839,   220,   214,   221,   222,   177,   223,   102,   208,   247,
     472,   176,   176,   176,   217,   248,   218,   334,   235,   249,
     371,   176,   250,   374,   176,   377,   215,   236,   374,   380,
     381,   382,   383,   177,   177,   177,   176,   261,   176,   226,
     530,   237,   325,   177,   786,   787,   177,  -292,   340,  -292,
     238,    79,   639,   484,    79,   239,   509,   882,   177,  -293,
     177,  -293,   240,   241,    79,   754,   581,   242,   887,   462,
     799,   800,   243,   216,   803,   804,   168,   169,   170,   171,
     286,   282,   810,   811,   283,    79,   176,   608,   896,   897,
     291,    79,   176,   631,   312,   393,   394,    79,   335,   633,
     176,   176,   639,    79,   639,   635,   639,    79,   177,   697,
     164,   165,   342,   487,   177,    79,   914,   699,   517,    79,
     167,   701,   177,   177,   765,   367,   766,   920,   168,   169,
     170,   171,   172,   176,   936,   937,   366,   765,   487,   908,
     370,   372,   176,   176,   378,   519,   520,   521,   175,   175,
     834,   102,   940,   387,   389,   177,   863,   864,   390,   400,
     402,   945,   408,   824,   177,   177,   443,   444,   639,   297,
     639,   830,   639,   448,   951,   263,   458,   953,   954,   955,
     500,   264,   265,   266,   267,   268,   269,   224,   473,   474,
     475,   477,   963,   478,   479,   513,   835,   480,   490,   850,
     851,   852,   491,   503,   505,   567,   256,   512,   225,   859,
     226,   227,   860,   534,   270,   543,   517,   228,   229,   230,
     231,   263,   544,   548,   517,   550,   869,   264,   265,   266,
     267,   268,   269,   575,   611,   589,   577,   651,   582,   600,
     601,   652,   605,   623,   606,   553,   554,   555,   556,   607,
     630,   617,   517,   517,   517,   644,   565,   645,   566,   647,
     270,   263,   517,   449,   450,   517,   648,   264,   265,   266,
     267,   268,   269,   451,   899,    87,   650,   462,   660,   517,
     901,   168,   169,   170,   171,   452,   622,  -327,   905,   906,
     665,    88,    89,    90,   682,   687,    91,    92,   688,   693,
     270,   704,   717,   718,   721,   735,   740,   741,  -327,    93,
    -327,  -327,    94,   210,   745,   747,   749,  -327,  -327,  -327,
    -327,   930,   750,   751,   753,   777,   778,   517,   780,   609,
     610,   662,   663,   517,   614,   615,    95,   781,   755,   782,
     783,   517,   517,    85,    96,   211,   677,    97,    98,   789,
     790,   791,   794,   -50,   -50,   806,    99,   294,   796,   100,
     101,   807,    87,   -50,   -50,   -50,   808,   102,   295,   809,
     813,   296,   820,   297,   517,   -50,   642,   823,    88,    89,
      90,   827,   829,    91,    92,   840,   -50,   831,   846,   848,
     856,   855,    14,    15,   857,    16,    93,   861,   875,    94,
      14,    15,   862,    16,   865,   211,   866,   562,   870,   871,
     878,    17,   884,   886,   879,   689,   690,    18,  -327,    17,
      19,    20,    21,    95,    22,    18,   891,   893,    19,    20,
     563,   894,    22,   895,    97,   354,   900,   902,   903,  -327,
     209,  -327,  -327,    99,   210,   907,   100,   101,  -327,  -327,
    -327,  -327,   757,   758,   102,   915,   715,   716,   912,   918,
     924,   719,   720,   925,   927,    23,   934,   929,   727,   728,
     161,   162,   938,    23,    24,    25,   211,   941,    26,   947,
     163,   952,    24,    25,   263,   956,    26,   959,   960,   350,
     264,   265,   266,   267,   268,   269,   961,   962,   164,   165,
     964,   965,   399,   263,   397,   166,   551,   353,   167,   264,
     265,   266,   267,   268,   269,   403,   168,   169,   170,   171,
     172,   909,   935,   270,   233,   933,   398,   174,   805,   636,
     702,   401,   140,   584,   772,   487,   487,   373,   661,   102,
     837,   841,   270,   141,   142,   568,     0,     0,     0,     0,
     828,   143,   255,   144,     0,     0,   145,     0,     0,     0,
       0,   146,   147,   148,   149,   150,     0,   140,     0,   843,
     844,     0,     0,   801,   802,     0,     0,   151,   141,   142,
       0,     0,     0,     0,   102,     0,   143,     0,   144,   357,
       0,   145,     0,     0,     0,     0,   146,   147,   148,   149,
     150,     0,     0,     0,   140,     0,     0,     0,     0,     0,
       0,     0,   151,     0,     0,   141,   142,     0,     0,   102,
       0,     0,     0,   143,   883,   144,   368,     0,   145,   140,
       0,     0,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   375,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   539,   487,   145,   487,
       0,   102,     0,   146,   147,   148,   149,   150,     0,   140,
     409,   410,   411,   412,   413,   414,   415,   416,     0,   151,
     141,   142,     0,     0,     0,     0,   102,     0,   143,     0,
     144,   673,     0,   145,   140,     0,     0,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   675,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   678,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     485,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   485,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   619,     0,     0,
       0,   421,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   423,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   425,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   429,     0,     0,     0,     0,   140,     0,   143,   151,
     144,     0,     0,   145,     0,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   143,     0,   144,     0,     0,
     145,     0,     0,     0,   151,   146,   147,   148,   149,   150,
       0,   102,     0,     0,   352,     0,     0,     0,     0,     0,
       0,   151,     0,   263,     0,     0,     0,     0,   102,   264,
     265,   266,   267,   268,   269,   263,     0,     0,     0,     0,
       0,   264,   265,   266,   267,   268,   269,   263,     0,     0,
       0,     0,     0,   264,   265,   266,   267,   268,   269,   404,
       0,     0,   270,     0,     0,     0,     0,     0,     0,   263,
     407,     0,     0,     0,   270,   264,   265,   266,   267,   268,
     269,     0,   263,     0,     0,     0,   270,   531,   264,   265,
     266,   267,   268,   269,   263,     0,     0,     0,     0,     0,
     264,   265,   266,   267,   268,   269,   768,   263,   270,     0,
       0,     0,   769,   264,   265,   266,   267,   268,   269,     0,
       0,   270,     0,     0,     0,     0,     0,     0,   890,   263,
       0,     0,     0,   270,     0,   264,   265,   266,   267,   268,
     269,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   270
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,    39,   337,   211,   492,    71,    45,    51,
      80,    51,   195,    51,    18,    19,    80,   163,   526,    54,
     598,   304,     5,    71,   216,     6,    89,    31,     5,   348,
      93,   223,    80,   352,   140,    51,    99,   599,     5,   145,
     581,    45,   294,     5,     2,     8,     5,   125,    17,    91,
     296,   297,    64,    91,     9,   125,   141,   142,    48,     5,
      59,   125,     9,    75,    76,    64,   630,     5,     5,    59,
      53,    75,     5,   319,   317,   318,     5,   125,    82,    76,
       5,     5,    48,    95,     5,     5,   650,    25,   294,     5,
     631,    60,   633,    76,   635,    76,   166,    59,    65,    76,
     164,   165,   320,   321,   322,   323,   275,   276,   277,   278,
     279,   280,   281,    76,    95,   367,    74,    63,   166,   681,
     124,    76,     8,   687,   688,   611,    85,    86,    65,    76,
      96,    59,    65,    58,    58,    64,     5,    58,    58,    84,
      26,   211,    58,    17,    60,    60,    32,   211,     5,    64,
      36,    48,   485,   195,    26,   195,   697,   195,   699,    28,
     701,   367,    59,   211,    36,     5,   208,   209,   208,   209,
     208,   209,   210,    30,   366,     5,   282,   741,   216,   195,
      58,    96,    38,   187,    58,   223,   271,   272,   273,   274,
      76,    28,   208,   209,    38,    64,    58,     5,   481,    96,
      30,    28,    42,    65,    76,   451,   449,   450,    48,    49,
      50,    51,    52,    53,    48,     5,   780,     5,    64,    97,
      76,     8,    30,    26,   294,    59,   790,    64,   806,   299,
     294,   235,    76,    36,     5,    62,    28,   241,   242,    26,
      30,    81,    30,    76,   248,    32,   294,     8,    76,    36,
      96,   299,   421,   761,   423,    58,   425,    60,   427,    30,
     429,    76,    96,     5,     8,    26,    94,   831,    60,    76,
      41,    32,   342,    76,    16,    36,   341,    80,    65,   283,
      95,    76,    26,   489,    48,   491,    89,    90,    32,    76,
       0,   855,    36,    76,   342,    59,   300,   367,    76,    41,
      95,   364,   365,   367,    65,     5,    76,   349,    26,    44,
      45,   349,    30,    44,    45,    76,   386,    95,    36,   367,
     884,    56,   326,    23,   328,    56,   532,   331,   366,   893,
     894,   895,    76,    65,   653,    26,   900,   166,   386,    30,
     523,   660,    77,   589,    76,    36,    77,   590,   591,    76,
      48,   543,   544,    54,    55,    53,   548,   503,    76,   505,
      38,   397,    80,   927,   401,    66,    72,    76,   403,    26,
      76,    89,    90,    91,   444,   593,   594,   595,   596,    36,
     325,    59,    38,    61,    62,    76,    64,    54,    55,    80,
      68,    69,    70,    71,    26,   340,   444,   401,    89,    90,
      91,   734,   735,    59,    36,    61,    62,    50,    64,    50,
     488,    58,   490,    26,    50,    58,    63,    58,   488,   489,
     490,   491,    58,    36,   488,   489,   490,   491,    26,   575,
     434,   577,   436,    54,    55,   439,   164,   165,    36,   509,
     488,   489,   490,   491,    64,    66,     6,   766,   393,   394,
      50,   770,    72,    73,    74,    75,    76,    50,    58,   208,
     209,   509,   532,    76,   950,    58,    64,    80,   532,    29,
     299,    31,   550,    76,    34,    72,    89,    90,    76,    76,
     550,   523,    80,   523,   532,   523,   550,    17,     4,    76,
     528,    89,    90,    54,    55,    11,    44,    45,    46,   562,
     563,     5,   550,   507,    65,   543,   544,   523,    56,    57,
     548,   581,    42,   342,   760,   834,   835,    53,    48,    49,
      50,    51,    52,    53,    60,    54,    55,    50,   598,    77,
     608,    50,   738,   581,    82,    58,    65,    12,   608,    58,
     618,    54,    55,   876,   608,   878,    58,    58,   618,    60,
     598,    81,    65,    65,   618,   500,    50,   386,    76,    47,
     608,   631,   444,   633,    58,   635,    54,    55,   513,    58,
     618,    70,   891,    54,    55,   579,    65,   647,   643,    58,
      76,    58,    63,   631,    58,   633,    65,   635,    65,    63,
      78,    90,    59,    92,    61,    94,    95,    64,    58,   647,
      58,   100,    59,    63,   682,    49,   669,    65,   553,   554,
     555,   556,   682,    76,    76,   444,    58,    58,   682,    58,
     565,   566,    76,    65,    65,    17,    65,   697,    58,   699,
     693,   701,    58,    58,   682,    65,    58,    63,    26,    27,
      65,    59,    38,    65,   143,   144,    42,    59,    36,   697,
      42,   699,    59,   701,    61,    62,    48,    49,    50,    51,
      52,    53,    76,    76,   609,   610,    54,    55,   738,   614,
     615,    76,    76,    61,   738,   745,    64,    72,    72,    64,
     509,    76,    76,   753,    72,    73,    74,    75,    76,    81,
     738,     8,    80,    10,    76,    83,    13,   745,    76,   762,
     763,    59,    76,    61,    62,   753,    64,    95,    60,    26,
     714,   781,   782,   783,    59,    32,    61,   721,    48,    36,
     219,   791,    39,   222,   794,   224,    76,    59,   227,   228,
     229,   230,   231,   781,   782,   783,   806,    59,   808,    61,
      62,    59,   186,   791,   689,   690,   794,    59,   192,    61,
      59,    58,   581,    60,    58,    76,    60,   822,   806,    59,
     808,    61,    76,    48,    58,   647,    60,    48,   833,   598,
     715,   716,    59,    64,   719,   720,    72,    73,    74,    75,
      76,    79,   727,   728,    64,    58,   856,    60,   853,   854,
      76,    58,   862,    60,    53,   239,   240,    58,     5,    60,
     870,   871,   631,    58,   633,    60,   635,    58,   856,    60,
      54,    55,    60,   312,   862,    58,   881,    60,   647,    58,
      64,    60,   870,   871,    58,    48,    60,   892,    72,    73,
      74,    75,    76,   903,   912,   913,    64,    58,   337,    60,
      76,    76,   912,   913,    76,   344,   345,   346,   912,   913,
      17,    95,   917,     5,    76,   903,   801,   802,    76,    76,
      76,   926,    76,   745,   912,   913,    28,    60,   697,    64,
     699,   753,   701,    76,   939,    42,    76,   942,   943,   944,
     324,    48,    49,    50,    51,    52,    53,    38,    76,    76,
      76,    76,   957,    76,    76,   339,    17,    30,    28,   781,
     782,   783,    28,    76,    76,   404,   405,    76,    59,   791,
      61,    62,   794,    92,    81,    64,   745,    68,    69,    70,
      71,    42,    64,    64,   753,    53,   808,    48,    49,    50,
      51,    52,    53,    76,    65,    64,    76,    18,    76,    76,
      76,    22,    76,     5,    76,   389,   390,   391,   392,    76,
       5,    76,   781,   782,   783,     5,   400,     5,   402,    60,
      81,    42,   791,    54,    55,   794,     5,    48,    49,    50,
      51,    52,    53,    64,   856,     3,     5,   806,    41,   808,
     862,    72,    73,    74,    75,    76,   485,    38,   870,   871,
      76,    19,    20,    21,    53,     5,    24,    25,     5,    15,
      81,    28,    76,    76,    62,    58,    30,     5,    59,    37,
      61,    62,    40,    64,    60,    28,    53,    68,    69,    70,
      71,   903,     5,     5,    60,     5,     5,   856,     5,   473,
     474,   530,   531,   862,   478,   479,    64,    60,    76,    60,
      60,   870,   871,     5,    72,    96,   545,    75,    76,    30,
       5,    60,    60,    44,    45,    60,    84,    48,    76,    87,
      88,     5,     3,    54,    55,    56,    60,    95,    59,    30,
      76,    62,    28,    64,   903,    66,    76,    53,    19,    20,
      21,    76,     5,    24,    25,    30,    77,     5,    76,     5,
      60,     5,     7,     8,    59,    10,    37,     5,    28,    40,
       7,     8,    60,    10,     5,    96,     5,    14,    60,    60,
      60,    26,     5,     5,    63,   559,   560,    32,    38,    26,
      35,    36,    37,    64,    39,    32,    17,     5,    35,    36,
      37,     5,    39,     5,    75,    76,     5,     5,    60,    59,
      60,    61,    62,    84,    64,     5,    87,    88,    68,    69,
      70,    71,   651,   652,    95,     5,   600,   601,    28,     5,
       5,   605,   606,     5,     5,    80,    64,     5,   612,   613,
      26,    27,     5,    80,    89,    90,    96,     5,    93,     5,
      36,     5,    89,    90,    42,     5,    93,     5,     5,    28,
      48,    49,    50,    51,    52,    53,     5,     5,    54,    55,
      65,     5,   246,    42,   244,    61,   386,    65,    64,    48,
      49,    50,    51,    52,    53,   250,    72,    73,    74,    75,
      76,   875,   909,    81,    80,   907,   245,    83,   721,   507,
     579,   248,    43,   445,   667,   734,   735,   222,   528,    95,
     761,   765,    81,    54,    55,   405,    -1,    -1,    -1,    -1,
     749,    62,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    72,    73,    74,    75,    76,    -1,    43,    -1,   768,
     769,    -1,    -1,   717,   718,    -1,    -1,    88,    54,    55,
      -1,    -1,    -1,    -1,    95,    -1,    62,    -1,    64,    65,
      -1,    67,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    54,    55,    -1,    -1,    95,
      -1,    -1,    -1,    62,   823,    64,    65,    -1,    67,    43,
      -1,    -1,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    65,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,   876,    67,   878,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    43,
     263,   264,   265,   266,   267,   268,   269,   270,    -1,    88,
      54,    55,    -1,    -1,    -1,    -1,    95,    -1,    62,    -1,
      64,    65,    -1,    67,    43,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    65,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    65,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    54,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    62,    88,
      64,    -1,    -1,    67,    43,    -1,    95,    -1,    72,    73,
      74,    75,    76,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    88,    64,    -1,    -1,    67,    43,
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
      -1,    48,    49,    50,    51,    52,    53,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    66,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    42,
      65,    -1,    -1,    -1,    81,    48,    49,    50,    51,    52,
      53,    -1,    42,    -1,    -1,    -1,    81,    60,    48,    49,
      50,    51,    52,    53,    42,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    52,    53,    66,    42,    81,    -1,
      -1,    -1,    60,    48,    49,    50,    51,    52,    53,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    63,    42,
      -1,    -1,    -1,    81,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
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
     127,    76,   181,   182,    48,    59,    62,    64,    28,    64,
     134,    26,    36,    64,    76,    80,    89,    90,   145,   146,
     147,   183,    53,     6,    29,    31,    34,    54,    55,    66,
      44,    45,    56,    77,    50,   172,     8,    26,    32,    36,
      65,    76,   174,   175,   183,     5,     5,    53,    76,    50,
     172,   159,    60,   190,    38,    42,    23,    17,    17,    58,
      28,    30,    33,    65,    76,   191,   191,    65,   200,   201,
     203,   220,   128,   139,    85,    86,    64,    48,    65,   200,
      76,   220,    76,   204,   220,    65,   200,   220,    76,   204,
     220,   220,   220,   220,    50,     5,    64,     5,   183,    76,
      76,    38,    76,   172,   172,   183,   183,   112,   120,   104,
      76,   154,    76,   118,    66,    58,    63,    65,    76,   221,
     221,   221,   221,   221,   221,   221,   221,   222,   222,   222,
     222,    55,   223,    55,   223,    55,   223,    55,   223,    55,
     223,   223,   223,   224,     8,    26,    32,    36,    65,    76,
     179,   180,   183,    28,    60,    58,    65,   128,    76,    54,
      55,    64,    76,   117,   142,   143,   144,   142,    76,    76,
     124,   125,   127,    26,    30,    36,    76,    80,    89,    90,
      91,   135,   183,    76,    76,    76,   147,    76,    76,    76,
      30,     5,    25,   151,    60,    64,   114,   220,    28,    62,
      28,    28,   132,   143,   143,   142,   144,   144,   144,   144,
     172,   173,   183,    76,   183,    76,   183,     5,    65,    60,
     177,   114,    76,   172,   173,   103,    76,   127,   133,   220,
     220,   220,   188,   189,   188,   203,   209,   188,    58,    65,
      62,    60,    58,    97,    92,   185,   193,   194,   185,    65,
     200,   128,    65,    64,    64,    58,    63,    65,    64,    63,
      53,   124,    50,   172,   172,   172,   172,   173,   173,    50,
      50,     5,    14,    37,   109,   172,   172,   220,   226,   223,
     223,   223,   223,   223,   183,    76,   183,    76,   183,     5,
      65,    60,    76,   133,   182,    50,   143,   143,   142,    64,
      54,    55,    63,    44,    45,    56,    77,    65,    58,    65,
      76,    76,     5,    26,    36,    76,    76,    76,    60,   172,
     172,    65,    38,    76,   172,   172,   147,    76,   148,    76,
     114,   116,   220,     5,   129,   139,   129,   139,   145,   173,
       5,    60,   178,    60,   178,    60,   175,     6,    76,   127,
     176,     9,    76,   157,     5,     5,   173,    60,     5,    59,
       5,    18,    22,    17,   190,   142,   211,   212,   213,   214,
      41,   201,   220,   220,   139,    76,    30,     5,    30,    41,
      30,    65,    50,    65,   200,    65,   200,   220,    65,   200,
     129,    65,    53,   173,   173,   173,   173,     5,     5,   172,
     172,   185,   185,    15,   110,   173,   173,    60,   178,    60,
     178,    60,   180,   176,    28,    65,   142,   143,   143,   144,
     144,   144,   144,   125,   134,   172,   172,    76,    76,   172,
     172,    62,   136,   129,   173,   173,   145,   172,   172,   173,
     173,    28,    60,   129,    60,    58,     5,    65,    58,    63,
      30,     5,   177,   176,   176,    60,   176,    28,    59,    53,
       5,     5,   103,    60,   133,    76,   177,   220,   220,   188,
      66,     5,    16,    41,   210,    58,    60,   188,    66,    60,
      17,    60,   194,   185,    65,    65,    65,     5,     5,   129,
       5,    60,    60,    60,   177,   177,   173,   173,   185,    30,
       5,    60,   176,   176,    60,   176,    76,    65,    30,   173,
     173,   172,   172,   173,   173,   174,    60,     5,    60,    30,
     173,   173,   150,    76,   114,   114,   115,    65,    76,   139,
      28,   177,   164,    53,   133,     8,    76,    76,   220,     5,
     133,     5,     9,   158,    17,    17,   142,   212,   185,   185,
      30,   214,   188,   220,   220,   188,    76,    30,     5,   177,
     133,   133,   133,   166,   167,     5,    60,    59,   177,   133,
     133,     5,    60,   173,   173,     5,     5,    63,   125,   133,
      60,    60,   152,   153,   213,    28,    58,    65,    60,    63,
     130,   162,   103,   220,     5,   177,     5,   103,   188,   188,
      63,    17,   168,     5,     5,     5,   103,   103,   177,   133,
       5,   133,     5,    60,   137,   133,   133,     5,    60,   150,
     114,   114,    28,   131,   103,     5,   177,   165,     5,   188,
     103,   177,   177,   177,     5,     5,   160,     5,   177,     5,
     133,     5,    76,   153,    64,   152,   129,   129,     5,   163,
     103,     5,   171,   170,   169,   103,   177,     5,    72,    76,
     149,   103,     5,   103,   103,   103,     5,   161,   145,     5,
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
     119,   119,   120,   120,   121,   121,   122,   123,   123,   124,
     124,   125,   125,   126,   127,   127,   127,   128,   128,   129,
     129,   129,   129,   130,   131,   129,   129,   129,   129,   129,
     129,   129,   132,   129,   129,   129,   129,   129,   129,   129,
     129,   133,   133,   133,   134,   134,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   136,   136,   137,   137,
     137,   138,   138,   139,   139,   139,   139,   139,   139,   140,
     140,   140,   140,   140,   141,   141,   141,   141,   141,   141,
     142,   142,   142,   142,   142,   143,   143,   143,   143,   143,
     144,   144,   144,   145,   146,   146,   148,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   149,   150,   151,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   155,
     156,   156,   157,   156,   158,   156,   159,   156,   160,   156,
     161,   156,   162,   156,   163,   156,   164,   156,   165,   156,
     166,   156,   167,   156,   168,   156,   169,   156,   170,   156,
     171,   156,   172,   173,   173,   173,   174,   174,   175,   175,
     175,   175,   175,   175,   175,   175,   176,   176,   176,   176,
     176,   177,   177,   178,   178,   178,   179,   179,   180,   180,
     180,   180,   180,   180,   181,   181,   182,   183,   183,   184,
     186,   185,   187,   185,   189,   188,   190,   190,   190,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   192,   192,   192,   192,   193,   193,
     193,   194,   194,   195,   195,   196,   196,   196,   197,   197,
     197,   197,   198,   198,   198,   199,   199,   199,   199,   199,
     199,   199,   199,   199,   199,   199,   199,   200,   200,   201,
     201,   201,   201,   202,   202,   202,   202,   202,   202,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   204,   204,
     205,   206,   206,   207,   207,   209,   208,   210,   210,   210,
     211,   211,   212,   212,   213,   213,   214,   214,   215,   216,
     217,   217,   217,   218,   219,   219,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   223,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   225,
     225,   226,   226
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
       2,     3,     2,     0,     0,     9,     4,     2,     4,     4,
       6,     9,     0,     5,     4,     2,     4,     3,     6,     3,
       6,     1,     3,     1,     0,     2,     1,     2,     4,     5,
       7,     5,     5,     6,     8,     7,     0,     3,     0,     3,
       3,     1,     0,     3,     1,     4,     3,     1,     3,     1,
       2,     2,     3,     3,     1,     4,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     1,     3,     3,     3,     3,
       1,     4,     3,     2,     3,     1,     0,     4,     4,     6,
       4,     4,     7,     2,     7,     0,     0,     0,     7,     5,
       0,     3,     1,     6,     0,     2,     1,     4,     6,     6,
       8,    10,     0,     9,     0,    11,     0,     7,     0,    13,
       0,    15,     0,    12,     0,    14,     0,    11,     0,    13,
       0,    11,     0,    11,     0,    12,     0,    14,     0,    14,
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
#line 2294 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 225 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2300 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 228 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2306 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 229 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2312 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 232 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2318 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 233 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2324 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 236 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2330 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 237 "pascal.y"
     { (yyval.node) = NULL; }
#line 2336 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 240 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2342 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2348 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 244 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2354 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 247 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2360 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 248 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2366 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 249 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2372 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 250 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2378 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 251 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2384 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 252 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2390 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 253 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2396 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 256 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2402 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 259 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2408 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 260 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2414 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 263 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2420 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 264 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2426 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 265 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2432 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 266 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2438 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 267 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2444 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 268 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2450 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 271 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2456 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 272 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2462 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 273 "pascal.y"
     { (yyval.node) = NULL; }
#line 2468 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 276 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2474 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 277 "pascal.y"
     { (yyval.node) = NULL; }
#line 2480 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 280 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2486 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 281 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2492 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 282 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2498 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 283 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2504 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 286 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2510 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 287 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2516 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 290 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2522 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 291 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2528 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 294 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2534 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 295 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2540 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 296 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2546 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 297 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2552 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 300 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2558 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 301 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2564 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 304 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2570 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 305 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2576 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 308 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2582 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 309 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2588 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 310 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2594 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 311 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2600 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 312 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2606 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 315 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2612 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 316 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2618 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 319 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2624 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 320 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2630 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 321 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2636 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 322 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2642 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 323 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2648 "pascal.tab.c"
    break;

  case 62: /* uses_list: uses_name  */
#line 326 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2654 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_list COMMA uses_name  */
#line 327 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2660 "pascal.tab.c"
    break;

  case 64: /* uses_name: IDENT  */
#line 330 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2666 "pascal.tab.c"
    break;

  case 65: /* uses_name: uses_name PERIOD IDENT  */
#line 331 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2672 "pascal.tab.c"
    break;

  case 66: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 334 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2678 "pascal.tab.c"
    break;

  case 67: /* spec_expr: spec_kw  */
#line 337 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2684 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_gen  */
#line 338 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2690 "pascal.tab.c"
    break;

  case 69: /* class_parents: parent_item  */
#line 341 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2696 "pascal.tab.c"
    break;

  case 70: /* class_parents: class_parents COMMA parent_item  */
#line 342 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2702 "pascal.tab.c"
    break;

  case 71: /* parent_item: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2708 "pascal.tab.c"
    break;

  case 72: /* parent_item: spec_ref  */
#line 346 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2714 "pascal.tab.c"
    break;

  case 73: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 349 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2720 "pascal.tab.c"
    break;

  case 74: /* spec_ref: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2726 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2732 "pascal.tab.c"
    break;

  case 76: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 354 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2738 "pascal.tab.c"
    break;

  case 77: /* type_arg_list: simple_type  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2744 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2750 "pascal.tab.c"
    break;

  case 79: /* type: simple_type  */
#line 361 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2756 "pascal.tab.c"
    break;

  case 80: /* type: ARROW IDENT  */
#line 362 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2762 "pascal.tab.c"
    break;

  case 81: /* type: CLASSSY OFSY IDENT  */
#line 363 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2768 "pascal.tab.c"
    break;

  case 82: /* type: ARROW spec_ref  */
#line 364 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2774 "pascal.tab.c"
    break;

  case 83: /* $@1: %empty  */
#line 365 "pascal.y"
                                                       { }
#line 2780 "pascal.tab.c"
    break;

  case 84: /* $@2: %empty  */
#line 365 "pascal.y"
                                                           { }
#line 2786 "pascal.tab.c"
    break;

  case 85: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 365 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2792 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY OFSY type  */
#line 366 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2798 "pascal.tab.c"
    break;

  case 87: /* type: PROCEDURESY pf_params  */
#line 367 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2804 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 368 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2810 "pascal.tab.c"
    break;

  case 89: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 369 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2816 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 370 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2822 "pascal.tab.c"
    break;

  case 91: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 371 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2828 "pascal.tab.c"
    break;

  case 92: /* $@3: %empty  */
#line 372 "pascal.y"
                         { }
#line 2834 "pascal.tab.c"
    break;

  case 93: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 372 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2840 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt SETSY OFSY simple_type  */
#line 373 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2846 "pascal.tab.c"
    break;

  case 95: /* type: packed_opt FILESY  */
#line 374 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2852 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt FILESY OFSY type  */
#line 375 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2858 "pascal.tab.c"
    break;

  case 97: /* type: OBJECTSY record_body ENDSY  */
#line 376 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2864 "pascal.tab.c"
    break;

  case 98: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 377 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2870 "pascal.tab.c"
    break;

  case 99: /* type: CLASSSY class_members ENDSY  */
#line 378 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2876 "pascal.tab.c"
    break;

  case 100: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 379 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2882 "pascal.tab.c"
    break;

  case 101: /* result_type: IDENT  */
#line 382 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2888 "pascal.tab.c"
    break;

  case 102: /* result_type: IDENT PERIOD IDENT  */
#line 383 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2894 "pascal.tab.c"
    break;

  case 103: /* result_type: spec_ref  */
#line 384 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2900 "pascal.tab.c"
    break;

  case 104: /* class_members: %empty  */
#line 387 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2906 "pascal.tab.c"
    break;

  case 105: /* class_members: class_members class_member  */
#line 388 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2912 "pascal.tab.c"
    break;

  case 106: /* class_member: IDENT  */
#line 391 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2918 "pascal.tab.c"
    break;

  case 107: /* class_member: IDENT SEMICOLON  */
#line 392 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2924 "pascal.tab.c"
    break;

  case 108: /* class_member: id_list COLON type SEMICOLON  */
#line 393 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2930 "pascal.tab.c"
    break;

  case 109: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 394 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2936 "pascal.tab.c"
    break;

  case 110: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 395 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2942 "pascal.tab.c"
    break;

  case 111: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 396 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2948 "pascal.tab.c"
    break;

  case 112: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 397 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2954 "pascal.tab.c"
    break;

  case 113: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 398 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2960 "pascal.tab.c"
    break;

  case 114: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 399 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2966 "pascal.tab.c"
    break;

  case 115: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 400 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2972 "pascal.tab.c"
    break;

  case 116: /* prop_index_opt: %empty  */
#line 403 "pascal.y"
    { (yyval.node) = NULL; }
#line 2978 "pascal.tab.c"
    break;

  case 117: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 404 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2984 "pascal.tab.c"
    break;

  case 118: /* prop_specs: %empty  */
#line 407 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2990 "pascal.tab.c"
    break;

  case 119: /* prop_specs: prop_specs IDENT IDENT  */
#line 408 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2996 "pascal.tab.c"
    break;

  case 120: /* prop_specs: prop_specs IDENT INTCONST  */
#line 409 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3002 "pascal.tab.c"
    break;

  case 121: /* packed_opt: PACKEDSY  */
#line 412 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3008 "pascal.tab.c"
    break;

  case 122: /* packed_opt: %empty  */
#line 413 "pascal.y"
     { (yyval.node) = NULL; }
#line 3014 "pascal.tab.c"
    break;

  case 123: /* simple_type: LPARENT enum_list RPARENT  */
#line 416 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3020 "pascal.tab.c"
    break;

  case 124: /* simple_type: IDENT  */
#line 417 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3026 "pascal.tab.c"
    break;

  case 125: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 418 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3032 "pascal.tab.c"
    break;

  case 126: /* simple_type: IDENT PERIOD IDENT  */
#line 419 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3038 "pascal.tab.c"
    break;

  case 127: /* simple_type: spec_ref  */
#line 420 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3044 "pascal.tab.c"
    break;

  case 128: /* simple_type: lo_bound DOTDOT b_expr  */
#line 421 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3050 "pascal.tab.c"
    break;

  case 129: /* lo_bound: lo_term  */
#line 424 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3056 "pascal.tab.c"
    break;

  case 130: /* lo_bound: PLUS lo_term  */
#line 425 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3062 "pascal.tab.c"
    break;

  case 131: /* lo_bound: MINUS lo_term  */
#line 426 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3068 "pascal.tab.c"
    break;

  case 132: /* lo_bound: lo_bound PLUS b_term  */
#line 427 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3074 "pascal.tab.c"
    break;

  case 133: /* lo_bound: lo_bound MINUS b_term  */
#line 428 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3080 "pascal.tab.c"
    break;

  case 134: /* lo_term: scalar_constant  */
#line 431 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3086 "pascal.tab.c"
    break;

  case 135: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 432 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3092 "pascal.tab.c"
    break;

  case 136: /* lo_term: lo_term MUL b_factor  */
#line 433 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3098 "pascal.tab.c"
    break;

  case 137: /* lo_term: lo_term IDIV b_factor  */
#line 434 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3104 "pascal.tab.c"
    break;

  case 138: /* lo_term: lo_term IMOD b_factor  */
#line 435 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3110 "pascal.tab.c"
    break;

  case 139: /* lo_term: lo_term SHIFTOP b_factor  */
#line 436 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3116 "pascal.tab.c"
    break;

  case 140: /* b_expr: b_term  */
#line 439 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3122 "pascal.tab.c"
    break;

  case 141: /* b_expr: PLUS b_term  */
#line 440 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3128 "pascal.tab.c"
    break;

  case 142: /* b_expr: MINUS b_term  */
#line 441 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3134 "pascal.tab.c"
    break;

  case 143: /* b_expr: b_expr PLUS b_term  */
#line 442 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3140 "pascal.tab.c"
    break;

  case 144: /* b_expr: b_expr MINUS b_term  */
#line 443 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3146 "pascal.tab.c"
    break;

  case 145: /* b_term: b_factor  */
#line 446 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3152 "pascal.tab.c"
    break;

  case 146: /* b_term: b_term MUL b_factor  */
#line 447 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3158 "pascal.tab.c"
    break;

  case 147: /* b_term: b_term IDIV b_factor  */
#line 448 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3164 "pascal.tab.c"
    break;

  case 148: /* b_term: b_term IMOD b_factor  */
#line 449 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3170 "pascal.tab.c"
    break;

  case 149: /* b_term: b_term SHIFTOP b_factor  */
#line 450 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3176 "pascal.tab.c"
    break;

  case 150: /* b_factor: scalar_constant  */
#line 453 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3182 "pascal.tab.c"
    break;

  case 151: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 454 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3188 "pascal.tab.c"
    break;

  case 152: /* b_factor: LPARENT b_expr RPARENT  */
#line 455 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3194 "pascal.tab.c"
    break;

  case 153: /* record_body: record_field_list record_case_opt  */
#line 458 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3200 "pascal.tab.c"
    break;

  case 154: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 461 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3206 "pascal.tab.c"
    break;

  case 155: /* record_field_list: record_field  */
#line 462 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3212 "pascal.tab.c"
    break;

  case 156: /* $@4: %empty  */
#line 465 "pascal.y"
                  { }
#line 3218 "pascal.tab.c"
    break;

  case 157: /* record_field: id_list COLON $@4 type  */
#line 465 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3224 "pascal.tab.c"
    break;

  case 158: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 466 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3230 "pascal.tab.c"
    break;

  case 159: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 467 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3236 "pascal.tab.c"
    break;

  case 160: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 468 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3242 "pascal.tab.c"
    break;

  case 161: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 469 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3248 "pascal.tab.c"
    break;

  case 162: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 470 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3254 "pascal.tab.c"
    break;

  case 163: /* record_field: IDENT record_field  */
#line 471 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3260 "pascal.tab.c"
    break;

  case 164: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 472 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3266 "pascal.tab.c"
    break;

  case 165: /* record_field: %empty  */
#line 473 "pascal.y"
     { (yyval.node) = NULL; }
#line 3272 "pascal.tab.c"
    break;

  case 166: /* case_arm_mark: %empty  */
#line 476 "pascal.y"
    { }
#line 3278 "pascal.tab.c"
    break;

  case 167: /* case_open: %empty  */
#line 479 "pascal.y"
    { }
#line 3284 "pascal.tab.c"
    break;

  case 168: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 482 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3290 "pascal.tab.c"
    break;

  case 169: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 483 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3296 "pascal.tab.c"
    break;

  case 170: /* record_case_opt: %empty  */
#line 484 "pascal.y"
     { (yyval.node) = NULL; }
#line 3302 "pascal.tab.c"
    break;

  case 171: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 487 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3308 "pascal.tab.c"
    break;

  case 172: /* record_case_list: record_case_arm  */
#line 488 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3314 "pascal.tab.c"
    break;

  case 173: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 491 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3320 "pascal.tab.c"
    break;

  case 174: /* record_case_arm: %empty  */
#line 492 "pascal.y"
     { (yyval.node) = NULL; }
#line 3326 "pascal.tab.c"
    break;

  case 175: /* var_decl_list: var_decl_list var_decl  */
#line 495 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3332 "pascal.tab.c"
    break;

  case 176: /* var_decl_list: var_decl  */
#line 496 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3338 "pascal.tab.c"
    break;

  case 177: /* var_decl: id_list COLON type SEMICOLON  */
#line 499 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3344 "pascal.tab.c"
    break;

  case 178: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 500 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3350 "pascal.tab.c"
    break;

  case 179: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 501 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3356 "pascal.tab.c"
    break;

  case 180: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 504 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3362 "pascal.tab.c"
    break;

  case 181: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 505 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3368 "pascal.tab.c"
    break;

  case 182: /* $@5: %empty  */
#line 506 "pascal.y"
                                                                               { }
#line 3374 "pascal.tab.c"
    break;

  case 183: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 506 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3380 "pascal.tab.c"
    break;

  case 184: /* $@6: %empty  */
#line 507 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3386 "pascal.tab.c"
    break;

  case 185: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 507 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3392 "pascal.tab.c"
    break;

  case 186: /* $@7: %empty  */
#line 508 "pascal.y"
                                        { }
#line 3398 "pascal.tab.c"
    break;

  case 187: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 508 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3404 "pascal.tab.c"
    break;

  case 188: /* $@8: %empty  */
#line 509 "pascal.y"
                                                                                                           { }
#line 3410 "pascal.tab.c"
    break;

  case 189: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 509 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3416 "pascal.tab.c"
    break;

  case 190: /* $@9: %empty  */
#line 510 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3422 "pascal.tab.c"
    break;

  case 191: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 510 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3428 "pascal.tab.c"
    break;

  case 192: /* $@10: %empty  */
#line 511 "pascal.y"
                                                                                                 { }
#line 3434 "pascal.tab.c"
    break;

  case 193: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 511 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3440 "pascal.tab.c"
    break;

  case 194: /* $@11: %empty  */
#line 512 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3446 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 512 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3452 "pascal.tab.c"
    break;

  case 196: /* $@12: %empty  */
#line 513 "pascal.y"
                                                                                            { }
#line 3458 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 513 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3464 "pascal.tab.c"
    break;

  case 198: /* $@13: %empty  */
#line 514 "pascal.y"
                                                                                                             { }
#line 3470 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 514 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3478 "pascal.tab.c"
    break;

  case 200: /* $@14: %empty  */
#line 517 "pascal.y"
                                                                                              { }
#line 3484 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 517 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3492 "pascal.tab.c"
    break;

  case 202: /* $@15: %empty  */
#line 520 "pascal.y"
                                                                                             { }
#line 3498 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 520 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3506 "pascal.tab.c"
    break;

  case 204: /* $@16: %empty  */
#line 523 "pascal.y"
                                                                                                    { }
#line 3512 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 523 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3520 "pascal.tab.c"
    break;

  case 206: /* $@17: %empty  */
#line 526 "pascal.y"
                                                                                                                { }
#line 3526 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 526 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3535 "pascal.tab.c"
    break;

  case 208: /* $@18: %empty  */
#line 530 "pascal.y"
                                                                                                                  { }
#line 3541 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 530 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3550 "pascal.tab.c"
    break;

  case 210: /* $@19: %empty  */
#line 534 "pascal.y"
                                                                                                                     { }
#line 3556 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 534 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3564 "pascal.tab.c"
    break;

  case 212: /* pv_mark: %empty  */
#line 539 "pascal.y"
    { }
#line 3570 "pascal.tab.c"
    break;

  case 213: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 542 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3576 "pascal.tab.c"
    break;

  case 214: /* parameter_list_opt: LPARENT RPARENT  */
#line 543 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3582 "pascal.tab.c"
    break;

  case 215: /* parameter_list_opt: %empty  */
#line 544 "pascal.y"
     { (yyval.node) = NULL; }
#line 3588 "pascal.tab.c"
    break;

  case 216: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 547 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3594 "pascal.tab.c"
    break;

  case 217: /* parameter_decl_list: parameter_decl  */
#line 548 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3600 "pascal.tab.c"
    break;

  case 218: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 551 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3606 "pascal.tab.c"
    break;

  case 219: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 552 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3612 "pascal.tab.c"
    break;

  case 220: /* parameter_decl: VARSY id_list COLON param_type  */
#line 553 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3618 "pascal.tab.c"
    break;

  case 221: /* parameter_decl: id_list COLON param_type  */
#line 554 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3624 "pascal.tab.c"
    break;

  case 222: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 555 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3630 "pascal.tab.c"
    break;

  case 223: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 556 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3636 "pascal.tab.c"
    break;

  case 224: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 557 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3642 "pascal.tab.c"
    break;

  case 225: /* parameter_decl: IDENT id_list COLON param_type  */
#line 558 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3648 "pascal.tab.c"
    break;

  case 226: /* param_type: IDENT  */
#line 561 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3654 "pascal.tab.c"
    break;

  case 227: /* param_type: IDENT PERIOD IDENT  */
#line 562 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3660 "pascal.tab.c"
    break;

  case 228: /* param_type: ARRAYSY OFSY IDENT  */
#line 563 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3666 "pascal.tab.c"
    break;

  case 229: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 564 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3672 "pascal.tab.c"
    break;

  case 230: /* param_type: spec_ref  */
#line 565 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3678 "pascal.tab.c"
    break;

  case 231: /* routine_directives: %empty  */
#line 568 "pascal.y"
    { (yyval.node) = NULL; }
#line 3684 "pascal.tab.c"
    break;

  case 232: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 569 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3697 "pascal.tab.c"
    break;

  case 233: /* pf_params: LPARENT pf_sections RPARENT  */
#line 579 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3703 "pascal.tab.c"
    break;

  case 234: /* pf_params: LPARENT RPARENT  */
#line 580 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3709 "pascal.tab.c"
    break;

  case 235: /* pf_params: %empty  */
#line 581 "pascal.y"
     { (yyval.node) = NULL; }
#line 3715 "pascal.tab.c"
    break;

  case 236: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 584 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3721 "pascal.tab.c"
    break;

  case 237: /* pf_sections: pf_section  */
#line 585 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3727 "pascal.tab.c"
    break;

  case 238: /* pf_section: id_list COLON param_type  */
#line 588 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3733 "pascal.tab.c"
    break;

  case 239: /* pf_section: VARSY id_list COLON param_type  */
#line 589 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3739 "pascal.tab.c"
    break;

  case 240: /* pf_section: CONSTSY id_list COLON param_type  */
#line 590 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3745 "pascal.tab.c"
    break;

  case 241: /* pf_section: IDENT id_list COLON param_type  */
#line 591 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3751 "pascal.tab.c"
    break;

  case 242: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 592 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3757 "pascal.tab.c"
    break;

  case 243: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 593 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3763 "pascal.tab.c"
    break;

  case 244: /* enum_list: enum_list COMMA enum_member  */
#line 596 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3769 "pascal.tab.c"
    break;

  case 245: /* enum_list: enum_member  */
#line 597 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3775 "pascal.tab.c"
    break;

  case 246: /* enum_member: IDENT  */
#line 600 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3781 "pascal.tab.c"
    break;

  case 247: /* id_list: id_list COMMA IDENT  */
#line 603 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3787 "pascal.tab.c"
    break;

  case 248: /* id_list: IDENT  */
#line 604 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3793 "pascal.tab.c"
    break;

  case 249: /* body: BEGINSY statement_list ENDSY  */
#line 607 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3799 "pascal.tab.c"
    break;

  case 250: /* $@20: %empty  */
#line 610 "pascal.y"
                             { pas_stmt_mark(); }
#line 3805 "pascal.tab.c"
    break;

  case 251: /* statement_list: statement_list SEMICOLON $@20 statement  */
#line 610 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3811 "pascal.tab.c"
    break;

  case 252: /* $@21: %empty  */
#line 611 "pascal.y"
     { pas_stmt_mark(); }
#line 3817 "pascal.tab.c"
    break;

  case 253: /* statement_list: $@21 statement  */
#line 611 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3823 "pascal.tab.c"
    break;

  case 254: /* $@22: %empty  */
#line 614 "pascal.y"
    { pas_stmt_mark(); }
#line 3829 "pascal.tab.c"
    break;

  case 255: /* body_stmt: $@22 statement  */
#line 614 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3835 "pascal.tab.c"
    break;

  case 256: /* statement: statement_no_label  */
#line 617 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3841 "pascal.tab.c"
    break;

  case 257: /* statement: INTCONST COLON statement_no_label  */
#line 618 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3847 "pascal.tab.c"
    break;

  case 258: /* statement: IDENT COLON statement_no_label  */
#line 619 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3853 "pascal.tab.c"
    break;

  case 259: /* statement_no_label: assignment  */
#line 622 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3859 "pascal.tab.c"
    break;

  case 260: /* statement_no_label: call  */
#line 623 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3865 "pascal.tab.c"
    break;

  case 261: /* statement_no_label: compound_statement  */
#line 624 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3871 "pascal.tab.c"
    break;

  case 262: /* statement_no_label: goto_statement  */
#line 625 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3877 "pascal.tab.c"
    break;

  case 263: /* statement_no_label: if_statement  */
#line 626 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3883 "pascal.tab.c"
    break;

  case 264: /* statement_no_label: case_statement  */
#line 627 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3889 "pascal.tab.c"
    break;

  case 265: /* statement_no_label: while_statement  */
#line 628 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3895 "pascal.tab.c"
    break;

  case 266: /* statement_no_label: repeat_statement  */
#line 629 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3901 "pascal.tab.c"
    break;

  case 267: /* statement_no_label: for_statement  */
#line 630 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3907 "pascal.tab.c"
    break;

  case 268: /* statement_no_label: with_statement  */
#line 631 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3913 "pascal.tab.c"
    break;

  case 269: /* statement_no_label: try_statement  */
#line 632 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3919 "pascal.tab.c"
    break;

  case 270: /* statement_no_label: raise_statement  */
#line 633 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3925 "pascal.tab.c"
    break;

  case 271: /* statement_no_label: inherited_call  */
#line 634 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3931 "pascal.tab.c"
    break;

  case 272: /* statement_no_label: INHERITEDSY  */
#line 635 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3937 "pascal.tab.c"
    break;

  case 273: /* statement_no_label: %empty  */
#line 636 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3943 "pascal.tab.c"
    break;

  case 274: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 639 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3949 "pascal.tab.c"
    break;

  case 275: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 640 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3955 "pascal.tab.c"
    break;

  case 276: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 641 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3961 "pascal.tab.c"
    break;

  case 277: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 642 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3967 "pascal.tab.c"
    break;

  case 278: /* handler_list: handler  */
#line 645 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3973 "pascal.tab.c"
    break;

  case 279: /* handler_list: handler_list SEMICOLON handler  */
#line 646 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3979 "pascal.tab.c"
    break;

  case 280: /* handler_list: handler_list SEMICOLON  */
#line 647 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3985 "pascal.tab.c"
    break;

  case 281: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 650 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3991 "pascal.tab.c"
    break;

  case 282: /* handler: ONSY IDENT DOSY body_stmt  */
#line 651 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3997 "pascal.tab.c"
    break;

  case 283: /* raise_statement: RAISESY  */
#line 654 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4003 "pascal.tab.c"
    break;

  case 284: /* raise_statement: RAISESY expression  */
#line 655 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4009 "pascal.tab.c"
    break;

  case 285: /* inherited_call: INHERITEDSY IDENT  */
#line 658 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4015 "pascal.tab.c"
    break;

  case 286: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 659 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4021 "pascal.tab.c"
    break;

  case 287: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 660 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4027 "pascal.tab.c"
    break;

  case 288: /* call: IDENT  */
#line 663 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4033 "pascal.tab.c"
    break;

  case 289: /* call: spec_expr  */
#line 664 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4039 "pascal.tab.c"
    break;

  case 290: /* call: selector PERIOD IDENT  */
#line 665 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4045 "pascal.tab.c"
    break;

  case 291: /* call: call_with_args  */
#line 666 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4051 "pascal.tab.c"
    break;

  case 292: /* recv_primary: STRINGCONST  */
#line 669 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4057 "pascal.tab.c"
    break;

  case 293: /* recv_primary: LPARENT expression RPARENT  */
#line 670 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4063 "pascal.tab.c"
    break;

  case 294: /* recv_primary: spec_expr  */
#line 671 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4069 "pascal.tab.c"
    break;

  case 295: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 674 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4075 "pascal.tab.c"
    break;

  case 296: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 675 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4081 "pascal.tab.c"
    break;

  case 297: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 676 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4087 "pascal.tab.c"
    break;

  case 298: /* call_with_args: IDENT LPARENT RPARENT  */
#line 677 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4093 "pascal.tab.c"
    break;

  case 299: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 678 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4099 "pascal.tab.c"
    break;

  case 300: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 679 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4105 "pascal.tab.c"
    break;

  case 301: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 680 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4111 "pascal.tab.c"
    break;

  case 302: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 681 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4117 "pascal.tab.c"
    break;

  case 303: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 682 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4123 "pascal.tab.c"
    break;

  case 304: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 683 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4129 "pascal.tab.c"
    break;

  case 305: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 684 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4135 "pascal.tab.c"
    break;

  case 306: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 685 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4141 "pascal.tab.c"
    break;

  case 307: /* argument_list: argument_list COMMA argument  */
#line 688 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4147 "pascal.tab.c"
    break;

  case 308: /* argument_list: argument  */
#line 689 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4153 "pascal.tab.c"
    break;

  case 309: /* argument: expression  */
#line 692 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4159 "pascal.tab.c"
    break;

  case 310: /* argument: expression COLON expression  */
#line 693 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4165 "pascal.tab.c"
    break;

  case 311: /* argument: expression COLON expression COLON expression  */
#line 694 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4171 "pascal.tab.c"
    break;

  case 312: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 695 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4177 "pascal.tab.c"
    break;

  case 313: /* assignment: selector BECOMES expression  */
#line 698 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4183 "pascal.tab.c"
    break;

  case 314: /* assignment: selector ADDBECOMES expression  */
#line 699 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4189 "pascal.tab.c"
    break;

  case 315: /* assignment: selector SUBBECOMES expression  */
#line 700 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4195 "pascal.tab.c"
    break;

  case 316: /* assignment: selector MULBECOMES expression  */
#line 701 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4201 "pascal.tab.c"
    break;

  case 317: /* assignment: selector DIVBECOMES expression  */
#line 702 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4207 "pascal.tab.c"
    break;

  case 318: /* assignment: call_with_args BECOMES expression  */
#line 703 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4213 "pascal.tab.c"
    break;

  case 319: /* selector: selector LBRACK expression_list RBRACK  */
#line 706 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4219 "pascal.tab.c"
    break;

  case 320: /* selector: selector PERIOD IDENT  */
#line 707 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4225 "pascal.tab.c"
    break;

  case 321: /* selector: selector ARROW  */
#line 708 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4231 "pascal.tab.c"
    break;

  case 322: /* selector: call_with_args PERIOD IDENT  */
#line 709 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4237 "pascal.tab.c"
    break;

  case 323: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 710 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4243 "pascal.tab.c"
    break;

  case 324: /* selector: call_with_args ARROW  */
#line 711 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4249 "pascal.tab.c"
    break;

  case 325: /* selector: recv_primary PERIOD IDENT  */
#line 712 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4255 "pascal.tab.c"
    break;

  case 326: /* selector: recv_primary ARROW  */
#line 713 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4261 "pascal.tab.c"
    break;

  case 327: /* selector: IDENT  */
#line 714 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4267 "pascal.tab.c"
    break;

  case 328: /* expression_list: expression_list COMMA expression  */
#line 717 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4273 "pascal.tab.c"
    break;

  case 329: /* expression_list: expression  */
#line 718 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4279 "pascal.tab.c"
    break;

  case 330: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 721 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4285 "pascal.tab.c"
    break;

  case 331: /* goto_statement: GOTOSY INTCONST  */
#line 724 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4291 "pascal.tab.c"
    break;

  case 332: /* goto_statement: GOTOSY IDENT  */
#line 725 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4297 "pascal.tab.c"
    break;

  case 333: /* if_statement: IFSY expression THENSY body_stmt  */
#line 728 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4303 "pascal.tab.c"
    break;

  case 334: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 729 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4309 "pascal.tab.c"
    break;

  case 335: /* $@23: %empty  */
#line 732 "pascal.y"
                           { }
#line 4315 "pascal.tab.c"
    break;

  case 336: /* case_statement: CASESY expression OFSY $@23 case_list case_tail ENDSY  */
#line 732 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4321 "pascal.tab.c"
    break;

  case 337: /* case_tail: %empty  */
#line 735 "pascal.y"
    { (yyval.node) = NULL; }
#line 4327 "pascal.tab.c"
    break;

  case 338: /* case_tail: ELSESY statement_list  */
#line 736 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4333 "pascal.tab.c"
    break;

  case 339: /* case_tail: OTHERWISESY statement_list  */
#line 737 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4339 "pascal.tab.c"
    break;

  case 340: /* case_list: case_list SEMICOLON case_elem  */
#line 740 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4345 "pascal.tab.c"
    break;

  case 341: /* case_list: case_elem  */
#line 741 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4351 "pascal.tab.c"
    break;

  case 342: /* case_elem: constant_list COLON body_stmt  */
#line 744 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4357 "pascal.tab.c"
    break;

  case 343: /* case_elem: %empty  */
#line 745 "pascal.y"
     { (yyval.node) = NULL; }
#line 4363 "pascal.tab.c"
    break;

  case 344: /* constant_list: constant_list COMMA case_label  */
#line 748 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4369 "pascal.tab.c"
    break;

  case 345: /* constant_list: case_label  */
#line 749 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4375 "pascal.tab.c"
    break;

  case 346: /* case_label: b_expr  */
#line 752 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4381 "pascal.tab.c"
    break;

  case 347: /* case_label: b_expr DOTDOT b_expr  */
#line 753 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4387 "pascal.tab.c"
    break;

  case 348: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 756 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4393 "pascal.tab.c"
    break;

  case 349: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 759 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4399 "pascal.tab.c"
    break;

  case 350: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 762 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4405 "pascal.tab.c"
    break;

  case 351: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 763 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4411 "pascal.tab.c"
    break;

  case 352: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 764 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4417 "pascal.tab.c"
    break;

  case 353: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 767 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4423 "pascal.tab.c"
    break;

  case 354: /* with_open: with_open COMMA selector  */
#line 770 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4429 "pascal.tab.c"
    break;

  case 355: /* with_open: selector  */
#line 771 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4435 "pascal.tab.c"
    break;

  case 356: /* expression: simple_expression  */
#line 778 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4441 "pascal.tab.c"
    break;

  case 357: /* expression: expression INOP simple_expression  */
#line 779 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4447 "pascal.tab.c"
    break;

  case 358: /* expression: expression LTOP simple_expression  */
#line 780 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4453 "pascal.tab.c"
    break;

  case 359: /* expression: expression LEOP simple_expression  */
#line 781 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4459 "pascal.tab.c"
    break;

  case 360: /* expression: expression GTOP simple_expression  */
#line 782 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4465 "pascal.tab.c"
    break;

  case 361: /* expression: expression GEOP simple_expression  */
#line 783 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4471 "pascal.tab.c"
    break;

  case 362: /* expression: expression NEOP simple_expression  */
#line 784 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4477 "pascal.tab.c"
    break;

  case 363: /* expression: expression EQOP simple_expression  */
#line 785 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4483 "pascal.tab.c"
    break;

  case 364: /* expression: expression ISOP simple_expression  */
#line 786 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4489 "pascal.tab.c"
    break;

  case 365: /* simple_expression: term  */
#line 789 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4495 "pascal.tab.c"
    break;

  case 366: /* simple_expression: PLUS term  */
#line 790 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4501 "pascal.tab.c"
    break;

  case 367: /* simple_expression: MINUS term  */
#line 791 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4507 "pascal.tab.c"
    break;

  case 368: /* simple_expression: simple_expression PLUS term  */
#line 792 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4513 "pascal.tab.c"
    break;

  case 369: /* simple_expression: simple_expression MINUS term  */
#line 793 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4519 "pascal.tab.c"
    break;

  case 370: /* simple_expression: simple_expression OROP term  */
#line 794 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4525 "pascal.tab.c"
    break;

  case 371: /* simple_expression: simple_expression XOROP term  */
#line 795 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4531 "pascal.tab.c"
    break;

  case 372: /* term: power  */
#line 798 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4537 "pascal.tab.c"
    break;

  case 373: /* term: term MUL power  */
#line 799 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4543 "pascal.tab.c"
    break;

  case 374: /* term: term RDIV power  */
#line 800 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4549 "pascal.tab.c"
    break;

  case 375: /* term: term IDIV power  */
#line 801 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4555 "pascal.tab.c"
    break;

  case 376: /* term: term IMOD power  */
#line 802 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4561 "pascal.tab.c"
    break;

  case 377: /* term: term ANDOP power  */
#line 803 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4567 "pascal.tab.c"
    break;

  case 378: /* term: term MUL MINUS power  */
#line 804 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4573 "pascal.tab.c"
    break;

  case 379: /* term: term RDIV MINUS power  */
#line 805 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4579 "pascal.tab.c"
    break;

  case 380: /* term: term IDIV MINUS power  */
#line 806 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4585 "pascal.tab.c"
    break;

  case 381: /* term: term IMOD MINUS power  */
#line 807 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4591 "pascal.tab.c"
    break;

  case 382: /* term: term ANDOP MINUS power  */
#line 808 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4597 "pascal.tab.c"
    break;

  case 383: /* term: term SHIFTOP power  */
#line 809 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4603 "pascal.tab.c"
    break;

  case 384: /* term: term ASOP power  */
#line 810 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4609 "pascal.tab.c"
    break;

  case 385: /* power: factor  */
#line 813 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4615 "pascal.tab.c"
    break;

  case 386: /* power: power POWOP factor  */
#line 814 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4621 "pascal.tab.c"
    break;

  case 387: /* factor: selector  */
#line 817 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4627 "pascal.tab.c"
    break;

  case 388: /* factor: call_with_args  */
#line 818 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4633 "pascal.tab.c"
    break;

  case 389: /* factor: INTCONST  */
#line 819 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4639 "pascal.tab.c"
    break;

  case 390: /* factor: REALCONST  */
#line 820 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4645 "pascal.tab.c"
    break;

  case 391: /* factor: STRINGCONST  */
#line 821 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4651 "pascal.tab.c"
    break;

  case 392: /* factor: CHARCODE  */
#line 822 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4657 "pascal.tab.c"
    break;

  case 393: /* factor: LPARENT expression RPARENT  */
#line 823 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4663 "pascal.tab.c"
    break;

  case 394: /* factor: inherited_call  */
#line 824 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4669 "pascal.tab.c"
    break;

  case 395: /* factor: NOTSY factor  */
#line 825 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4675 "pascal.tab.c"
    break;

  case 396: /* factor: ATSIGN factor  */
#line 826 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4681 "pascal.tab.c"
    break;

  case 397: /* factor: LBRACK RBRACK  */
#line 827 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4687 "pascal.tab.c"
    break;

  case 398: /* factor: LBRACK set_member_list RBRACK  */
#line 828 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4693 "pascal.tab.c"
    break;

  case 399: /* set_member_list: set_member  */
#line 831 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4699 "pascal.tab.c"
    break;

  case 400: /* set_member_list: set_member_list COMMA set_member  */
#line 832 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4705 "pascal.tab.c"
    break;

  case 401: /* set_member: expression  */
#line 835 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4711 "pascal.tab.c"
    break;

  case 402: /* set_member: expression DOTDOT expression  */
#line 836 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4717 "pascal.tab.c"
    break;


#line 4721 "pascal.tab.c"

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

#line 842 "pascal.y"

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
