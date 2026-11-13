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
static int pt_is_helper_word(tree_t *w) { return w && w->v.sval && !strcasecmp(w->v.sval, "helper"); }
static tree_t *pt_helper_mark(tree_t *target, tree_t *parent) { tree_t *m = pt_add(pt_sval(pt_new(TT_DECL), "helper"), target); return parent ? pt_add(m, parent) : m; }
static int pt_first_is_helper(tree_t *fl) { return fl && fl->n > 0 && fl->c[0] && fl->c[0]->t == TT_DECL && fl->c[0]->v.sval && !strcmp(fl->c[0]->v.sval, "helper"); }
static tree_t *pt_record_or_helper(tree_t *packed, tree_t *rb) {
    if (!pt_first_is_helper(rb)) return pt_add(pt_add(pt_new(TT_RECORD), packed), rb);
    tree_t *mem = pt_new(TT_VLIST);
    for (int i = 0; i < rb->n; i++) pt_add(mem, rb->c[i]);
    return pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), mem);
}
static tree_t *pt_type_helper(tree_t *target, tree_t *members) {
    tree_t *mem = pt_add(pt_new(TT_VLIST), pt_helper_mark(target, NULL));
    for (int i = 0; members && i < members->n; i++) pt_add(mem, members->c[i]);
    return pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), mem);
}
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

#line 258 "pascal.tab.c"

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
#define YYLAST   1893

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  422
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1018

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
       0,   238,   238,   239,   242,   243,   246,   247,   250,   251,
     254,   257,   258,   261,   262,   263,   264,   265,   266,   267,
     270,   273,   274,   277,   278,   279,   280,   281,   282,   285,
     286,   287,   290,   291,   294,   295,   296,   297,   300,   301,
     304,   305,   308,   309,   310,   311,   314,   315,   318,   319,
     322,   323,   324,   325,   326,   329,   330,   333,   334,   335,
     336,   337,   338,   341,   342,   345,   346,   349,   352,   353,
     356,   357,   360,   361,   364,   367,   368,   369,   372,   373,
     376,   377,   378,   379,   380,   380,   380,   381,   382,   383,
     384,   385,   386,   387,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   400,   401,   402,   405,   406,
     409,   410,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   427,   428,   431,   432,   433,   436,
     437,   440,   441,   442,   443,   444,   445,   448,   449,   450,
     451,   452,   455,   456,   457,   458,   459,   460,   461,   464,
     465,   466,   467,   468,   471,   472,   473,   474,   475,   478,
     479,   480,   481,   484,   487,   488,   491,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   505,
     508,   511,   512,   513,   514,   517,   518,   521,   522,   525,
     526,   529,   530,   531,   534,   535,   536,   536,   537,   537,
     538,   538,   539,   539,   540,   540,   541,   541,   542,   542,
     543,   543,   544,   544,   547,   547,   550,   550,   553,   553,
     556,   556,   560,   560,   564,   564,   568,   568,   573,   576,
     577,   578,   581,   582,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   598,   599,   600,   601,   602,
     605,   606,   616,   617,   618,   621,   622,   625,   626,   627,
     628,   629,   630,   633,   634,   637,   640,   641,   644,   647,
     647,   648,   648,   651,   651,   654,   655,   656,   659,   660,
     661,   662,   663,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,   676,   677,   678,   679,   682,   683,   684,
     687,   688,   691,   692,   695,   696,   697,   700,   701,   702,
     703,   704,   707,   708,   709,   712,   713,   714,   715,   716,
     717,   718,   719,   720,   721,   722,   723,   726,   727,   730,
     731,   732,   733,   736,   737,   738,   739,   740,   741,   744,
     745,   746,   747,   748,   749,   750,   751,   752,   755,   756,
     759,   762,   763,   766,   767,   770,   770,   773,   774,   775,
     778,   779,   782,   783,   786,   787,   790,   791,   794,   797,
     800,   801,   802,   805,   808,   809,   816,   817,   818,   819,
     820,   821,   822,   823,   824,   827,   828,   829,   830,   831,
     832,   833,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,   848,   851,   852,   855,   856,   857,
     858,   859,   860,   861,   862,   863,   864,   865,   866,   869,
     870,   873,   874
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

#define YYPACT_NINF (-903)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-348)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     580,    -1,   143,   189,  -903,   154,   641,  -903,  -903,   375,
    -903,    63,  -903,  -903,     5,   185,   143,   193,   207,   207,
     221,  -903,   220,   175,   224,   227,   337,  -903,  -903,  -903,
     254,   207,   236,   348,   264,  -903,  -903,    87,   542,   185,
    -903,    89,   336,   398,  -903,   207,  -903,   361,   163,   462,
      54,   999,   589,   357,   220,  -903,   378,   384,   400,   495,
     506,   511,   545,  -903,   551,  -903,  -903,  -903,  -903,   401,
    1420,   218,  -903,  -903,   143,   207,   593,   533,  -903,   643,
     218,  -903,   207,   664,   127,  -903,  -903,   666,   682,  -903,
    1420,   471,  1420,  -903,  1420,  1420,   619,  -903,  1160,  -903,
    1420,   710,   715,  -903,   707,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,   515,   402,  -903,  1799,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,   207,  1072,   735,  -903,   579,   721,
     746,   718,   730,   763,   766,  -903,   768,   673,  -903,  -903,
    1628,  1628,  1628,  1210,  1420,  1628,  -903,  -903,  -903,   751,
     152,   710,   772,  -903,   806,   626,    77,   437,   460,   762,
    -903,   778,   773,  -903,   773,   754,  1065,  1065,    98,   780,
    -903,  -903,  -903,  -903,  1505,   211,   176,  -903,  -903,  -903,
    -903,   816,   539,  -903,   420,   355,   336,    21,  -903,   110,
     854,  -903,    30,   343,  -903,  -903,   812,   999,  -903,  -903,
     732,   304,   364,   806,   626,   145,  1199,   162,  1676,  1688,
     383,   383,  1245,   283,    28,  1804,   824,   832,  1270,   821,
    -903,  1420,   825,  -903,  1420,  1295,  1420,   826,  -903,  1420,
    1420,  1420,  1420,  1420,   468,    73,    84,   900,   207,   838,
     839,   138,  -903,  -903,   207,   207,  -903,   185,   143,  -903,
     845,   207,   849,   220,  -903,  -903,   460,   460,  -903,  1710,
     583,  -903,  1722,  -903,   868,   871,  -903,  1420,  1420,  1420,
    1420,  1420,  1420,  1420,  1420,  1628,  1628,  1628,  1628,  1511,
    1536,  1561,  1586,  1611,  1628,  1628,  1628,   863,   186,  -903,
     242,   896,   899,   941,   588,   355,   355,    37,  -903,  -903,
     562,  -903,   283,   887,  1817,  1817,   897,   186,   392,   906,
     912,   915,   881,   919,   920,   929,   942,    62,  -903,   787,
    1445,   172,   979,   982,  -903,   957,   957,  1817,   957,   957,
     957,   957,  -903,   533,   207,   935,   207,   938,  -903,   207,
      35,  -903,   788,  -903,  -903,  1445,   939,  -903,   533,  -903,
     285,  -903,  1420,  1420,  1420,  -903,  -903,   471,  -903,  -903,
    -903,  -903,  1175,  -903,  -903,  -903,   578,  -903,   758,  1744,
     113,  -903,   884,  -903,  1320,   283,  -903,   592,   952,  1804,
     793,   674,  1804,  -903,   603,  1804,  1651,   683,  1804,  1804,
    1804,  1804,   964,  -903,  -903,   186,  -903,   473,  -903,  -903,
    -903,  -903,   533,   533,   483,   498,   185,   165,   948,  -903,
     207,  -903,   220,  1420,  1420,  -903,   792,   958,   961,   437,
     437,   437,   437,   437,   437,   437,   437,   460,   460,   460,
     460,  1628,   762,  1628,   762,  1628,   762,  1628,   762,  1628,
     762,   762,   762,  -903,   963,    37,   605,  -903,  -903,   523,
     207,   951,   207,   959,  -903,   207,    36,  -903,   817,   967,
     285,   218,   968,   780,  -903,   502,   563,   957,   957,  1817,
     693,  -903,   571,   443,  -903,   496,  -903,   607,   969,  -903,
     972,   310,   454,   973,   975,   977,  -903,   818,  -903,  -903,
     991,   981,   983,  -903,   209,  -903,  -903,  -903,   286,   988,
    -903,  -903,  1470,  1029,  1804,   218,   283,   218,   283,   286,
     443,   443,   722,  -903,  -903,  -903,  -903,   533,  1035,   848,
     773,   852,   773,   862,    44,  -903,    26,    48,  1060,  1061,
     533,  1008,  1064,   137,  -903,  1067,  1151,   642,  1804,  -903,
     999,  -903,   626,  1817,  1032,  1420,  -903,  1420,  1420,   283,
    -903,   994,   237,   230,  -903,   312,  -903,   624,   521,  -903,
    1345,  1370,  1420,  -903,  -903,  1395,  -903,   218,   639,  1023,
     533,   533,   533,   533,  1074,  1075,  -903,  -903,  -903,  -903,
    -903,  1066,   533,   533,  1804,  -903,   762,   762,   762,   762,
     762,  -903,   186,  1026,  -903,   870,   773,   875,   773,   893,
     223,  -903,    26,  -903,  1069,  -903,  -903,  -903,  -903,   443,
     443,   528,  1013,  1817,   957,   957,  -903,   957,   957,   957,
     957,  -903,  -903,  -903,  -903,  -903,  1017,  1019,  1024,  1025,
    -903,  -903,  1040,   218,   533,   533,   286,  -903,  1038,  -903,
    -903,   533,   533,  -903,   235,   218,   328,  1051,    69,  1722,
    -903,  -903,   704,  -903,  -903,  1082,  1109,  -903,    26,  -903,
      26,  1055,    26,  -903,  1094,   140,  -903,  1070,  1119,  1120,
    -903,  -903,  -903,  1071,   285,  -903,  1052,  -903,  1420,  1420,
    -903,  -903,   470,   248,  -903,   894,  -903,  -903,  -903,  1757,
    1769,  -903,   124,  -903,   884,  -903,  -903,  -903,  -903,  -903,
    -903,   648,  -903,   659,  1804,  -903,   660,  1125,  1127,   218,
    1129,  1091,  1093,    45,  -903,  -903,   533,   533,  1130,  1130,
    -903,  1124,  1152,  1096,  -903,  -903,    26,  -903,    26,  1098,
      26,  -903,  -903,  1083,   713,  -903,  -903,   568,   443,   443,
    -903,  -903,  -903,  -903,   910,   533,   533,  -903,  1095,  -903,
    -903,   533,   533,    44,  1101,  1158,  -903,  1104,  1135,  1147,
     533,   533,  -903,  -903,  -903,   318,  -903,  1445,  1445,   322,
    -903,   283,  1140,  -903,  -903,  1100,  1117,  -903,   285,  -903,
      40,  1102,  1420,  -903,  -903,  1172,   285,  1182,  -903,    55,
    1068,  1133,  -903,  1817,  1817,  -903,  -903,  1150,  1817,  -903,
    -903,  1420,  1420,  -903,  1112,  -903,   339,  -903,  -903,  -903,
    -903,  -903,  1186,  -903,   285,   285,  -903,   285,  1100,  1100,
    1187,  1134,  1130,  1136,  -903,   285,  1016,  -903,  -903,   285,
    -903,  -903,  -903,  -903,  -903,  1202,  1148,  1191,   533,   533,
    1206,  1207,    60,   186,  -903,   285,  -903,  1139,  1157,  1163,
    1817,   780,  1190,  -903,  -903,   670,  -903,  1165,  1170,  -903,
    1100,  -903,  1420,  -903,  -903,  -903,  -903,  1804,  -903,  1233,
    -903,  1235,  -903,  -903,  -903,   722,  -903,  1130,  1130,  -903,
    -903,  -903,  1782,  1804,  -903,  1225,  -903,  -903,  1100,  1249,
    1250,  1100,  1252,  -903,  -903,  -903,   285,  -903,  1100,  1253,
    -903,  -903,  -903,   285,  1183,  1255,  1201,  -903,  -903,  -903,
    -903,  -903,  -903,   285,   285,  1257,  -903,   934,   671,  -903,
    1445,  -903,  1445,  1238,  -903,  -903,  1258,  1804,  -903,  1100,
    -903,  1262,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  1263,  1264,  1100,  1265,  -903,  1271,  -903,  -903,   285,
      33,  -903,  -903,  1817,  1211,  1251,  1817,  -903,  -903,   218,
     218,  1273,  -903,  1100,  -903,  -903,  -903,  1276,  1100,  1100,
    1282,  1100,  -903,  -903,  -903,  -903,  1100,  -903,  1292,  -903,
     716,  -903,  -903,  -903,  1257,  -903,  -903,  -903,  -903,  1296,
    -903,  -903,  -903,  -903,  -903,  1297,  1100,  -903,  -903,  -903,
     286,  1817,  1298,  -903,  1299,  1301,  1306,  -903,  -903,  1261,
    1257,  -903,  -903,  -903,  -903,  1309,  -903,  -903
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   228,   267,    16,   190,     0,     0,   228,
       0,   292,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   130,    38,    18,     0,     0,     0,   231,   189,     0,
     130,    19,     0,     0,   231,   269,   268,     0,     0,   271,
       0,     0,     0,   271,     0,     0,     0,   312,   307,   271,
     302,   291,     0,    69,   308,    68,   272,   275,   288,   289,
     290,   279,     0,   311,   278,     0,   280,   281,   282,   283,
     284,   285,   286,   287,     0,   130,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   409,   412,   410,   411,
     347,     0,   314,   414,   408,   407,     0,   376,   385,   392,
     405,   108,   254,   129,   254,     0,     0,     0,     0,     0,
      51,    54,    52,    53,   132,   110,   178,   142,    76,    75,
     135,     0,     0,    80,     0,   137,    64,     0,   228,     0,
       0,   266,     0,     0,   228,   200,     0,   292,   351,   352,
       0,     0,     0,     0,   375,     0,     0,     0,     0,     0,
     292,   292,     0,     0,     0,   303,   304,     0,     0,     0,
     346,     0,     0,   344,     0,     0,     0,     0,   341,     0,
       0,     0,     0,     0,     0,   108,   110,     0,     0,     0,
       0,     0,   228,   228,     0,     0,     4,     0,     0,    12,
       0,     0,     0,     0,    21,   415,   386,   387,   417,   421,
       0,   419,     0,   416,     0,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,    88,     0,     0,    50,   138,   139,    81,    83,   265,
       0,   264,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,     0,     0,   184,   165,     0,
       0,     0,    97,     0,    93,     0,     0,     0,     0,     0,
       0,     0,   228,   231,     0,     0,     0,     0,   230,   267,
       0,   233,     0,   250,   191,     0,     0,   228,   231,    12,
       0,   270,     0,     0,     0,   273,   273,     0,   355,   350,
     273,   313,   307,   276,   277,   318,     0,   328,   407,   329,
       0,    78,   271,   271,     0,     0,   317,     0,   345,   338,
     310,     0,   349,   320,     0,   333,   309,     0,   334,   335,
     336,   337,     0,    61,    60,     0,    57,     0,   228,   228,
     228,   228,   231,   231,     0,     0,    24,     0,    31,   228,
      26,   228,    25,     0,     0,   418,   413,   342,   340,   377,
     378,   379,   380,   381,   382,   383,   384,   390,   388,   389,
     391,     0,   395,     0,   396,     0,   397,     0,   393,     0,
     394,   403,   404,   406,     0,    72,     0,    70,    73,     0,
       0,     0,     0,     0,   253,   267,     0,   256,     0,     0,
       0,   130,     0,     0,   131,     0,   134,     0,     0,     0,
      50,   159,     0,   149,   154,     0,    82,     0,     0,   101,
       0,   112,     0,     0,     0,     0,   111,     0,   228,   228,
       0,     0,     0,   175,     0,   228,   228,    99,   178,     0,
     163,   166,     0,     0,    42,   130,     0,   130,     0,   178,
     140,   141,   136,   146,   147,   145,   148,   231,     0,   243,
     254,   242,   254,   244,     0,   229,     0,   196,     0,     0,
     231,     0,     0,   105,   107,     0,     0,     0,   369,   368,
     292,   373,   374,   363,   353,     0,   315,     0,     0,     0,
      67,     0,     0,     0,   297,     0,   305,     0,     0,   316,
       0,     0,     0,   343,   319,     0,   339,   130,     0,     0,
     231,   231,   231,   231,     0,     0,   228,   228,    23,   271,
     271,    33,   231,   231,   422,   420,   400,   401,   402,   398,
     399,   109,     0,   108,   103,     0,   254,     0,   254,     0,
       0,   252,     0,    89,    90,   110,   143,   263,    77,   150,
     151,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,   144,   110,   228,   228,   113,     0,     0,     0,     0,
     228,   228,   124,   130,   231,   231,   178,   168,     0,   228,
     228,   231,   231,   164,     0,   130,   347,     0,     0,    42,
      41,    87,     0,    98,    96,     0,     0,   250,     0,   234,
       0,     0,     0,   232,     0,   245,   249,   237,     0,     0,
      12,   192,   193,     0,     0,   201,     0,   250,     0,     0,
     273,   274,   366,   357,   361,     0,   365,   273,   327,   349,
     330,    79,     0,   293,   299,   294,   271,   296,   306,    74,
     326,     0,   324,     0,   348,   322,     0,     0,   110,   130,
       0,     0,     0,     0,   250,   250,   231,   231,    29,    30,
     271,     0,     0,     0,    71,   110,     0,   261,     0,     0,
       0,   255,   257,     0,     0,   162,   160,     0,   152,   153,
     156,   157,   155,   158,     0,   231,   231,   114,     0,   228,
     228,   231,   231,     0,     0,     0,   170,     0,     0,     0,
     231,   231,   172,   173,   180,     0,   167,     0,     0,     0,
      44,     0,     0,    94,   250,   210,   238,   236,     0,   241,
       0,     0,     0,   194,   251,     0,     0,     0,   106,   198,
       0,     0,   372,     0,   363,   271,   271,     0,     0,   273,
     354,     0,     0,   273,     0,   298,     0,   325,   323,   321,
      59,    62,     0,   250,     0,     0,   250,     0,   214,   216,
       0,     0,    32,     0,   250,     0,     0,   259,   258,     0,
     260,    91,    95,   161,   102,     0,     0,     0,   231,   231,
       0,     0,     0,     0,   116,     0,   100,     0,     0,   177,
     188,     0,     0,    48,    46,     0,    45,     0,     0,    84,
     206,    12,     0,   235,   248,   247,   246,   239,   197,     0,
     250,     0,    12,   273,   273,   367,   360,   359,   358,   356,
     364,   362,     0,   331,   301,     0,   295,    58,   218,     0,
       0,   224,     0,    12,    12,   250,     0,    20,    27,     0,
     104,   262,   117,     0,     0,     0,     0,   119,   120,   125,
     126,   171,   169,     0,     0,   183,   186,     0,     0,   180,
       0,    43,     0,     0,    85,    12,     0,   240,   250,   212,
     195,     0,   371,   370,   332,   273,    12,   250,   250,    12,
     250,     0,     0,   202,     0,   250,     0,   115,   121,     0,
       0,   176,   174,   188,     0,     0,   188,    47,    49,   130,
     130,     0,   211,   208,    12,   199,   300,     0,   226,   222,
       0,   220,   215,   217,    12,   250,    28,   118,     0,   123,
       0,   185,   179,   180,   181,    92,    86,   207,    12,     0,
     219,    12,    12,   225,    12,     0,   204,   122,   128,   127,
     178,   188,     0,   213,     0,     0,     0,   203,    12,     0,
     182,   209,   227,   223,   221,     0,   187,   205
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -903,  -903,  -903,  -903,  -903,   -65,  1073,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  1076,   -34,  -342,  -903,  -903,   -56,
    1077,   -45,  1079,    34,   -70,   -38,  -280,  -573,   -64,   -30,
    -268,   -78,  -903,  -903,  -903,   164,   736,  -277,  -903,  -903,
    -903,  -903,  -185,  -903,   536,  -274,  -263,  -285,  -492,  -903,
    -283,  -903,  -903,  -880,  -903,  -902,   386,  1080,   -39,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,   566,   252,   575,   823,
    -565,   234,  -143,  -903,   741,   485,   885,   482,  -903,   -85,
    -903,  -903,  -334,  -903,  -186,   590,  -903,  -903,   657,  -903,
     -28,  -903,  -903,   -33,  -176,   807,  -903,   -31,  1131,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,   559,  -518,   556,  -903,
    -903,  -903,  -903,  -903,    -4,  1022,   -69,  -129,   -89,  -903,
     947
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     254,   581,   721,    37,    39,    40,   503,   855,   648,   471,
      54,    55,    41,    42,   103,   152,   446,   447,   105,   180,
     370,   181,   924,   960,   509,   535,   289,   308,   486,   754,
     950,   182,   183,   184,   185,   682,   473,   474,   316,   317,
     318,   645,  1000,   850,   500,   915,   916,    45,    46,    28,
     670,   872,   349,   974,  1008,   925,   988,   861,   964,   893,
     894,   936,   994,   992,   939,   991,    77,   190,   340,   341,
     667,   527,   291,   456,   457,   300,   301,   319,    29,    50,
     197,    51,   539,   540,   106,   107,   108,   553,   554,   109,
     153,   111,   112,   154,   366,   367,   114,   155,   387,   116,
     117,   118,   119,   543,   797,   683,   684,   917,   686,   120,
     121,   122,   123,   205,   369,   157,   158,   159,   160,   260,
     261
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   178,   192,   528,   201,    72,    78,   179,   207,   127,
     178,   351,   449,   104,   214,   177,   179,   655,   113,   724,
     115,   292,   541,   110,   177,   685,   544,   477,   371,   493,
     472,   475,   664,    85,   465,   344,    11,   732,   979,   956,
     524,   600,   377,   513,   514,   515,   516,   237,   864,   384,
     816,   255,   334,   512,   984,   178,   263,   668,   203,    85,
     204,   179,   510,   511,   871,   524,   156,   498,    33,   177,
     335,   332,   256,   257,   769,     8,   336,    35,   393,    79,
     337,    36,   266,   345,    86,   302,   202,   499,   206,   394,
     208,   209,    68,   776,    73,   777,   215,   779,   178,  1010,
     525,   601,   665,  1001,   179,   817,   346,   558,   186,   980,
     177,   177,   306,   372,   373,   568,   865,   371,   334,   267,
     339,   102,    34,   909,   669,   268,   269,   270,   271,   272,
     273,   669,   195,   213,   770,   287,   335,   288,   298,   259,
     262,   803,   336,   178,   758,    69,   337,    74,   395,   179,
     432,   434,   436,   438,   440,   441,   442,   177,   274,   104,
     647,   827,   356,   828,   113,   830,   115,    85,    81,   110,
     578,   549,   104,   104,   297,   338,   400,   113,   113,   115,
     115,   368,   110,   110,   804,   302,   339,   368,   302,    12,
     371,   189,   359,   102,   368,   611,   676,   443,   557,   781,
     505,    56,   309,   357,   609,   610,   427,   428,   429,   430,
     550,    57,   310,    13,   401,   643,   212,   379,   178,    10,
     382,    79,   385,    74,   179,   382,   388,   389,   390,   391,
     161,   450,   178,   213,   506,   694,   213,   178,   179,   306,
     311,    65,    85,   179,   162,   163,   177,   639,   213,   451,
     450,    58,   312,   794,   164,   452,   313,   165,   448,   453,
     695,    38,   445,   764,   795,   314,   315,   693,   451,    43,
     910,   696,   166,   167,   452,   307,   685,   448,   453,   168,
     178,   102,   169,    44,   532,   640,   179,   552,   555,   796,
     170,   171,   172,   173,   174,   765,    52,    49,   175,   455,
      59,   176,   586,    60,   587,   178,   588,   454,   589,    85,
     590,   179,   309,   102,    53,   625,   504,    85,   455,   177,
     534,   652,   310,   654,   203,   178,   542,   354,   734,   626,
      63,   179,   740,   741,   742,   743,   196,   166,   167,   737,
      67,   504,   697,   368,    85,   744,   792,   169,   536,   537,
     538,   738,   739,   800,   681,   170,   171,   172,   173,   174,
      66,   533,   312,    61,   691,   448,   313,   127,  -267,   886,
    -267,    78,    72,    62,   627,   314,   315,   659,   102,   661,
     102,   355,   851,   605,   701,   703,    87,   856,   767,   706,
     178,   178,   212,   347,   852,    34,   179,   179,   857,   328,
     329,    79,    88,    89,    90,   177,   267,    91,    92,   584,
     259,   330,   268,   269,   270,   271,   272,   273,   478,    79,
      93,    80,   479,    94,   213,   853,   854,   651,   480,   653,
     534,   744,   331,   126,    30,   178,   178,   178,   178,    31,
     221,   179,   179,   179,   179,   274,    75,    95,   826,   177,
     177,   177,   177,   727,   128,   729,   178,    76,    97,   362,
     129,   222,   179,   223,   224,   881,   225,    99,   481,   884,
     100,   101,   482,   138,   325,   326,   130,   139,   102,   178,
     628,   483,   484,   485,   275,   179,   327,   617,   618,   707,
     629,   276,   277,   177,   718,   719,   666,   178,   649,   619,
      47,    48,   104,   179,   279,   280,   281,   113,  1009,   115,
      82,   177,   110,    64,   368,   278,   282,   283,   392,   875,
     620,    83,   178,   569,   614,   615,    79,    47,   179,   368,
     368,    79,   178,   576,   368,    95,   793,   284,   179,   932,
     933,    79,   285,   689,   690,   321,    97,   150,   577,   478,
     614,   615,   608,   594,   131,   755,    79,   187,   704,   480,
     549,   621,   448,   178,   193,   132,   102,   766,   322,   179,
     323,   699,   666,   324,   219,   178,   220,   177,   957,   549,
     958,   179,   614,   615,     1,   518,   858,   133,   178,   177,
     178,     2,   178,   735,   179,    70,   179,   189,   179,   481,
     531,   966,    71,   482,   178,   785,   234,  -143,  -143,    79,
     179,   806,   483,   484,   485,    84,   135,  -143,  -143,  -143,
     463,   134,   614,   615,   604,   614,   615,   464,   666,  -143,
     666,   812,   666,   833,   616,   822,   545,   124,   239,   178,
    -143,   414,   125,   546,   534,   179,   415,   462,    14,    15,
     545,    16,   305,   177,   574,   575,   178,   559,   178,   680,
     178,   545,   179,   592,   179,   592,   179,    17,   564,   188,
     593,   342,   622,    18,   790,   791,    19,    20,    21,   210,
      22,   247,   545,   248,   267,   265,   249,   228,   229,   698,
     268,   269,   270,   271,   272,   273,   666,   592,   666,   250,
     666,   178,   295,   296,   708,   251,   545,   179,   178,   252,
     877,   878,   253,   807,   179,   177,   178,   545,   545,   191,
     397,    23,   179,   274,   808,   809,   404,   405,   920,   463,
      24,    25,   562,    47,    26,   921,   955,   563,   198,   478,
     194,   562,   199,   832,   178,   178,   566,   178,   534,   480,
     179,   179,   612,   179,   333,   178,   534,   613,   200,   178,
     348,   179,   771,   504,   504,   179,  -314,   772,  -314,   656,
     352,   218,   458,   178,   353,   178,   614,   615,   867,   179,
     240,   179,   673,   238,   534,   534,   216,   534,   998,   481,
     487,   217,   999,   482,   242,   534,   926,   882,   883,   534,
     363,   364,   483,   484,   485,   241,   243,   931,   402,   403,
    -312,   244,  -312,   448,   245,   534,   519,   265,   521,   228,
     547,   523,   710,   711,   712,   713,   178,   246,   941,   942,
     293,  -342,   179,   178,   722,   723,   218,   290,   787,   179,
     287,   286,   288,   178,   178,    79,    79,   501,   526,   179,
     179,  -313,  -342,  -313,  -342,  -342,   299,   561,   927,   343,
     961,  -342,  -342,  -342,  -342,   264,   534,   223,   224,   320,
     225,   967,   350,   534,   970,    79,    79,   602,   633,   178,
     375,   985,   986,   534,   534,   179,   756,   757,   374,   178,
     178,   775,    47,   762,   763,   179,   179,   378,   517,   989,
     491,   380,   386,   177,   177,   396,    79,   309,   658,   995,
      79,   789,   660,   530,   398,   399,   504,   310,   504,   534,
      79,   409,   662,  1002,   459,   411,  1004,  1005,    79,  1006,
     726,   487,   595,    79,   597,   728,   478,   599,   444,  -267,
     834,  -267,   863,  1015,   417,   492,   480,   418,   818,   819,
     869,    79,   798,   730,   799,    14,    15,   312,    16,   460,
     461,   313,   579,   466,   570,   571,   572,   573,   820,   821,
     314,   315,   497,   476,    17,   582,   551,   583,   889,   890,
      18,   892,   488,    19,    20,   580,   481,    22,   489,   899,
     482,   490,   798,   901,   954,   494,   495,   835,   836,   483,
     484,   485,    87,   840,   841,   496,   342,   507,   860,   911,
     508,   520,   848,   849,   522,   529,   560,   567,    88,    89,
      90,   469,   561,    91,    92,   565,   591,   596,    23,   170,
     171,   172,   173,   470,   650,   598,    93,    24,    25,    94,
     657,    26,   478,   603,   606,   623,   900,   888,   624,   630,
     891,   631,   480,   632,   634,   635,   636,   637,   898,   638,
     944,   641,   642,    95,   644,   671,   672,   946,   674,   675,
     692,    96,   677,   687,    97,    98,   709,   951,   952,   714,
     715,   720,   458,    99,   235,   873,   100,   101,   287,   736,
     905,   906,   481,   747,   102,   748,   482,   733,   162,   163,
     749,   750,   753,   759,   929,   483,   484,   485,   164,   768,
     267,   165,   773,   978,   774,   778,   268,   269,   270,   271,
     272,   273,   780,   782,   783,   784,   166,   167,   788,   943,
     810,   786,   811,   168,   813,    85,   169,   170,   171,   172,
     173,   294,   716,   717,   170,   171,   172,   173,   174,   274,
     874,   814,   236,   815,   823,   176,   825,   824,   829,   831,
     837,   843,   963,   844,   845,   846,   847,   102,   859,   678,
     862,   968,   969,   679,   971,   267,   669,   868,   866,   976,
     879,   268,   269,   270,   271,   272,   273,   870,   885,   745,
     746,   887,   895,   267,   896,   897,   751,   752,  -347,   268,
     269,   270,   271,   272,   273,   760,   761,   902,   903,   996,
     904,   907,   908,  -347,   274,   912,   487,   913,   919,  -347,
     211,  -347,  -347,   914,   212,   922,   487,   358,  -347,  -347,
    -347,  -347,   274,   923,  -347,   342,  -347,  -347,   928,   212,
     930,   267,   935,  -347,  -347,  -347,  -347,   268,   269,   270,
     271,   272,   273,   140,   937,   938,   213,   940,   945,   947,
     948,   949,   953,   962,   141,   142,   959,   965,   972,   973,
     975,   213,   143,   258,   144,   982,   977,   145,   987,   983,
     274,   990,   146,   147,   148,   149,   150,   993,   140,   419,
     420,   421,   422,   423,   424,   425,   426,   997,   151,   141,
     142,  1003,  1007,  1011,  1012,   102,  1013,   143,   487,   144,
     365,  1014,   145,   140,  1017,   838,   839,   146,   147,   148,
     149,   150,   408,   406,   141,   142,  1016,   407,   842,   725,
     412,   410,   143,   151,   144,   376,   918,   145,   140,   981,
     102,   731,   146,   147,   148,   149,   150,   663,   607,   141,
     142,   805,   688,   876,   880,   381,     0,   143,   151,   144,
     383,   585,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   556,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     700,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,   702,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   144,
     705,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   144,     0,     0,   145,   140,     0,
     102,     0,   146,   147,   148,   149,   150,     0,     0,   141,
     142,     0,     0,     0,     0,     0,     0,   143,   151,   502,
       0,     0,   145,   140,     0,   102,     0,   146,   147,   148,
     149,   150,     0,     0,   141,   142,     0,     0,     0,     0,
       0,     0,   143,   151,   502,     0,     0,   145,     0,     0,
     102,     0,   146,   147,   148,   149,   646,     0,     0,   -50,
     -50,     0,     0,   302,   140,     0,     0,     0,   151,   -50,
     -50,   -50,     0,     0,   303,   102,   431,   304,     0,   305,
       0,   -50,     0,   143,     0,   144,     0,     0,   145,   140,
       0,     0,   -50,   146,   147,   148,   149,   150,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   213,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,   143,   151,   144,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   437,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,     0,   439,     0,     0,     0,
       0,   140,     0,   143,   151,   144,     0,     0,   145,     0,
       0,   102,     0,   146,   147,   148,   149,   150,     0,  -340,
     143,     0,   144,     0,     0,   145,     0,     0,     0,   151,
     146,   147,   148,   149,   150,     0,   102,     0,     0,   360,
    -340,     0,  -340,  -340,     0,   565,   151,     0,   267,  -340,
    -340,  -340,  -340,   102,   268,   269,   270,   271,   272,   273,
     267,     0,     0,     0,     0,     0,   268,   269,   270,   271,
     272,   273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   267,   361,     0,     0,     0,   274,   268,   269,
     270,   271,   272,   273,   267,     0,     0,     0,     0,   274,
     268,   269,   270,   271,   272,   273,   413,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   267,   416,     0,     0,
       0,   274,   268,   269,   270,   271,   272,   273,     0,   267,
       0,     0,     0,   274,   548,   268,   269,   270,   271,   272,
     273,   267,     0,     0,     0,     0,     0,   268,   269,   270,
     271,   272,   273,   801,   267,   274,     0,     0,     0,   802,
     268,   269,   270,   271,   272,   273,     0,   226,   274,     0,
       0,     0,     0,     0,     0,   934,   267,     0,     0,     0,
     274,     0,   268,   269,   270,   271,   272,   273,   227,     0,
     228,   229,     0,   274,     0,     0,     0,   230,   231,   232,
     233,   467,   468,     0,     0,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,   274,     0,     0,     0,   170,
     171,   172,   173,   470
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   345,    89,    39,    45,    71,    93,    54,
      80,   197,   289,    51,    99,    71,    80,   509,    51,   592,
      51,   164,   356,    51,    80,   543,   360,   307,   213,   312,
     304,   305,     6,     5,   302,     5,     2,   602,     5,   919,
       5,     5,   218,   328,   329,   330,   331,   125,     8,   225,
       5,   140,     8,   327,   956,   125,   145,     9,    91,     5,
      91,   125,   325,   326,     9,     5,    70,     5,     5,   125,
      26,    50,   141,   142,     5,    76,    32,    72,     5,    58,
      36,    76,     5,    53,    30,    48,    90,    25,    92,     5,
      94,    95,     5,   658,     5,   660,   100,   662,   168,  1001,
      65,    65,    76,   983,   168,    60,    76,   375,    74,    76,
     166,   167,    28,    85,    86,   395,    76,   302,     8,    42,
      76,    95,    59,    63,    76,    48,    49,    50,    51,    52,
      53,    76,     5,    96,    65,    62,    26,    64,   168,   143,
     144,    17,    32,   213,   636,    58,    36,    58,    64,   213,
     279,   280,   281,   282,   283,   284,   285,   213,    81,   197,
     502,   726,    17,   728,   197,   730,   197,     5,     5,   197,
       5,    58,   210,   211,    76,    65,    38,   210,   211,   210,
     211,   212,   210,   211,    60,    48,    76,   218,    48,     0,
     375,    64,    30,    95,   225,   469,    59,   286,   374,    59,
      28,    26,    26,    58,   467,   468,   275,   276,   277,   278,
      97,    36,    36,    59,    76,   498,    64,   221,   288,    76,
     224,    58,   226,    58,   288,   229,   230,   231,   232,   233,
      12,     8,   302,    96,    62,     5,    96,   307,   302,    28,
      64,     5,     5,   307,    26,    27,   302,    38,    96,    26,
       8,    76,    76,     5,    36,    32,    80,    39,   288,    36,
      30,    76,    76,    28,    16,    89,    90,    30,    26,    76,
     843,    41,    54,    55,    32,    64,   794,   307,    36,    61,
     350,    95,    64,    76,   349,    76,   350,   372,   373,    41,
      72,    73,    74,    75,    76,    60,    76,    76,    80,    76,
      76,    83,   431,    76,   433,   375,   435,    65,   437,     5,
     439,   375,    26,    95,    94,     5,   320,     5,    76,   375,
     350,   506,    36,   508,   357,   395,   357,    23,   605,    19,
      76,   395,   617,   618,   619,   620,    84,    54,    55,   613,
      76,   345,    30,   374,     5,   622,   680,    64,   352,   353,
     354,   614,   615,   687,   540,    72,    73,    74,    75,    76,
      12,    76,    76,    26,   549,   395,    80,   412,    58,    30,
      60,   410,   406,    36,    64,    89,    90,   520,    95,   522,
      95,    17,    64,   461,   560,   561,     3,    65,    60,   565,
     460,   461,    64,    50,    76,    59,   460,   461,    76,    44,
      45,    58,    19,    20,    21,   461,    42,    24,    25,   413,
     414,    56,    48,    49,    50,    51,    52,    53,    26,    58,
      37,    60,    30,    40,    96,   767,   768,   505,    36,   507,
     460,   708,    77,    76,    59,   505,   506,   507,   508,    64,
      38,   505,   506,   507,   508,    81,    48,    64,   725,   505,
     506,   507,   508,   596,    76,   598,   526,    59,    75,    76,
      76,    59,   526,    61,    62,   799,    64,    84,    76,   803,
      87,    88,    80,    72,    54,    55,    76,    76,    95,   549,
      26,    89,    90,    91,    47,   549,    66,    44,    45,   567,
      36,    54,    55,   549,   579,   580,   526,   567,   502,    56,
      18,    19,   540,   567,    44,    45,    46,   540,  1000,   540,
      48,   567,   540,    31,   545,    78,    56,    57,    50,   793,
      77,    59,   592,    50,    54,    55,    58,    45,   592,   560,
     561,    58,   602,    50,   565,    64,    66,    77,   602,   873,
     874,    58,    82,   547,   548,     6,    75,    76,    50,    26,
      54,    55,    50,    30,    59,   633,    58,    75,   562,    36,
      58,    65,   592,   633,    82,    59,    95,   645,    29,   633,
      31,    50,   602,    34,    59,   645,    61,   633,   920,    58,
     922,   645,    54,    55,     4,   333,   771,    76,   658,   645,
     660,    11,   662,    65,   658,    53,   660,    64,   662,    76,
     348,   935,    60,    80,   674,   670,   124,    44,    45,    58,
     674,   696,    89,    90,    91,    49,    65,    54,    55,    56,
      58,    76,    54,    55,   460,    54,    55,    65,   658,    66,
     660,   709,   662,    65,    63,   720,    58,    48,    59,   709,
      77,    58,    53,    65,   674,   709,    63,    59,     7,     8,
      58,    10,    64,   709,   402,   403,   726,    65,   728,    17,
     730,    58,   726,    58,   728,    58,   730,    26,    65,    76,
      65,   189,    65,    32,   678,   679,    35,    36,    37,    60,
      39,     8,    58,    10,    42,    59,    13,    61,    62,    65,
      48,    49,    50,    51,    52,    53,   726,    58,   728,    26,
     730,   771,   166,   167,    65,    32,    58,   771,   778,    36,
     795,   796,    39,    65,   778,   771,   786,    58,    58,    76,
     238,    80,   786,    81,    65,    65,   244,   245,    58,    58,
      89,    90,    58,   251,    93,    65,    65,    63,    72,    26,
      76,    58,    76,    30,   814,   815,    63,   817,   778,    36,
     814,   815,    59,   817,   188,   825,   786,    64,    76,   829,
     194,   825,    58,   767,   768,   829,    59,    63,    61,   517,
      38,    64,   290,   843,    42,   845,    54,    55,   782,   843,
      59,   845,   530,    48,   814,   815,    76,   817,    72,    76,
     308,    76,    76,    80,    76,   825,   861,   801,   802,   829,
     210,   211,    89,    90,    91,    59,    76,   872,   242,   243,
      59,    48,    61,   843,    48,   845,   334,    59,   336,    61,
      62,   339,   570,   571,   572,   573,   896,    59,   893,   894,
      76,    38,   896,   903,   582,   583,    64,    64,   674,   903,
      62,    79,    64,   913,   914,    58,    58,    60,    60,   913,
     914,    59,    59,    61,    61,    62,    76,    64,   862,     5,
     925,    68,    69,    70,    71,    59,   896,    61,    62,    53,
      64,   936,    60,   903,   939,    58,    58,    60,    60,   949,
      48,   959,   960,   913,   914,   949,   634,   635,    64,   959,
     960,   657,   410,   641,   642,   959,   960,    76,   332,   964,
      19,    76,    76,   959,   960,     5,    58,    26,    60,   974,
      58,   677,    60,   347,    76,    76,   920,    36,   922,   949,
      58,    76,    60,   988,    28,    76,   991,   992,    58,   994,
      60,   449,   450,    58,   452,    60,    26,   455,    75,    58,
      30,    60,   778,  1008,    76,    64,    36,    76,   714,   715,
     786,    58,    58,    60,    60,     7,     8,    76,    10,    60,
      19,    80,    14,    76,   398,   399,   400,   401,   716,   717,
      89,    90,    30,    76,    26,   409,    92,   411,   814,   815,
      32,   817,    76,    35,    36,    37,    76,    39,    76,   825,
      80,    76,    58,   829,    60,    76,    76,   745,   746,    89,
      90,    91,     3,   751,   752,    76,   524,    28,   774,   845,
      28,    76,   760,   761,    76,    76,    64,    53,    19,    20,
      21,    64,    64,    24,    25,    64,    63,    76,    80,    72,
      73,    74,    75,    76,     5,    76,    37,    89,    90,    40,
       5,    93,    26,    76,    76,    76,    30,   813,    76,    76,
     816,    76,    36,    76,   488,   489,    65,    76,   824,    76,
     896,   495,   496,    64,    76,     5,     5,   903,    60,     5,
      76,    72,     5,    41,    75,    76,    53,   913,   914,     5,
       5,    15,   600,    84,    12,    17,    87,    88,    62,    76,
     838,   839,    76,    76,    95,    76,    80,    28,    26,    27,
      76,    76,    62,    65,   870,    89,    90,    91,    36,    58,
      42,    39,    30,   949,     5,    60,    48,    49,    50,    51,
      52,    53,    28,    53,     5,     5,    54,    55,    76,   895,
       5,    60,     5,    61,     5,     5,    64,    72,    73,    74,
      75,    76,   576,   577,    72,    73,    74,    75,    76,    81,
      17,    60,    80,    60,    30,    83,    60,     5,    60,    76,
      65,    60,   928,     5,    60,    30,    19,    95,    28,    18,
      53,   937,   938,    22,   940,    42,    76,     5,    76,   945,
      30,    48,    49,    50,    51,    52,    53,     5,    76,   623,
     624,     5,     5,    42,    60,    59,   630,   631,    38,    48,
      49,    50,    51,    52,    53,   639,   640,     5,    60,   975,
      19,     5,     5,    38,    81,    76,   734,    60,    28,    59,
      60,    61,    62,    60,    64,    60,   744,    28,    68,    69,
      70,    71,    81,    63,    59,   753,    61,    62,     5,    64,
       5,    42,    17,    68,    69,    70,    71,    48,    49,    50,
      51,    52,    53,    43,     5,     5,    96,     5,     5,    76,
       5,    60,     5,     5,    54,    55,    28,     5,     5,     5,
       5,    96,    62,    63,    64,    64,     5,    67,     5,    28,
      81,     5,    72,    73,    74,    75,    76,     5,    43,   267,
     268,   269,   270,   271,   272,   273,   274,     5,    88,    54,
      55,     5,     5,     5,     5,    95,     5,    62,   826,    64,
      65,     5,    67,    43,     5,   749,   750,    72,    73,    74,
      75,    76,   249,   247,    54,    55,    65,   248,   753,   593,
     253,   251,    62,    88,    64,    65,   851,    67,    43,   953,
      95,   600,    72,    73,    74,    75,    76,   524,   463,    54,
      55,   694,   545,   794,   798,   224,    -1,    62,    88,    64,
      65,   414,    67,    43,    -1,    95,    -1,    72,    73,    74,
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
      -1,    95,    -1,    72,    73,    74,    75,    76,    -1,    38,
      62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,    88,
      72,    73,    74,    75,    76,    -1,    95,    -1,    -1,    33,
      59,    -1,    61,    62,    -1,    64,    88,    -1,    42,    68,
      69,    70,    71,    95,    48,    49,    50,    51,    52,    53,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    65,    -1,    -1,    -1,    81,    48,    49,
      50,    51,    52,    53,    42,    -1,    -1,    -1,    -1,    81,
      48,    49,    50,    51,    52,    53,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    42,    65,    -1,    -1,
      -1,    81,    48,    49,    50,    51,    52,    53,    -1,    42,
      -1,    -1,    -1,    81,    60,    48,    49,    50,    51,    52,
      53,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    66,    42,    81,    -1,    -1,    -1,    60,
      48,    49,    50,    51,    52,    53,    -1,    38,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    42,    -1,    -1,    -1,
      81,    -1,    48,    49,    50,    51,    52,    53,    59,    -1,
      61,    62,    -1,    81,    -1,    -1,    -1,    68,    69,    70,
      71,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    81,    -1,    -1,    -1,    72,
      73,    74,    75,    76
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
     226,    12,    26,    27,    36,    39,    54,    55,    61,    64,
      72,    73,    74,    75,    76,    80,    83,   117,   122,   126,
     127,   129,   139,   140,   141,   142,   121,   185,    76,    64,
     175,    76,   129,   185,    76,     5,   175,   188,    72,    76,
      76,   187,   222,   201,   205,   221,   222,   187,   222,   222,
      60,    60,    64,    96,   187,   222,    76,    76,    64,    59,
      61,    38,    59,    61,    62,    64,    38,    59,    61,    62,
      68,    69,    70,    71,   185,    12,    80,   129,    48,    59,
      59,    59,    76,    76,    48,    48,    59,     8,    10,    13,
      26,    32,    36,    39,   108,   226,   224,   224,    63,   222,
     227,   228,   222,   226,    59,    59,     5,    42,    48,    49,
      50,    51,    52,    53,    81,    47,    54,    55,    78,    44,
      45,    46,    56,    57,    77,    82,    79,    62,    64,   134,
      64,   180,   180,    76,    76,   142,   142,    76,   127,    76,
     183,   184,    48,    59,    62,    64,    28,    64,   135,    26,
      36,    64,    76,    80,    89,    90,   146,   147,   148,   185,
      53,     6,    29,    31,    34,    54,    55,    66,    44,    45,
      56,    77,    50,   174,     8,    26,    32,    36,    65,    76,
     176,   177,   185,     5,     5,    53,    76,    50,   174,   160,
      60,   192,    38,    42,    23,    17,    17,    58,    28,    30,
      33,    65,    76,   193,   193,    65,   202,   203,   205,   222,
     128,   140,    85,    86,    64,    48,    65,   202,    76,   222,
      76,   206,   222,    65,   202,   222,    76,   206,   222,   222,
     222,   222,    50,     5,     5,    64,     5,   185,    76,    76,
      38,    76,   174,   174,   185,   185,   112,   120,   104,    76,
     155,    76,   118,    66,    58,    63,    65,    76,    76,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   224,   224,
     224,    55,   225,    55,   225,    55,   225,    55,   225,    55,
     225,   225,   225,   226,    75,    76,   124,   125,   127,   135,
       8,    26,    32,    36,    65,    76,   181,   182,   185,    28,
      60,    19,    59,    58,    65,   128,    76,    54,    55,    64,
      76,   117,   143,   144,   145,   143,    76,   124,    26,    30,
      36,    76,    80,    89,    90,    91,   136,   185,    76,    76,
      76,    19,    64,   148,    76,    76,    76,    30,     5,    25,
     152,    60,    64,   114,   222,    28,    62,    28,    28,   132,
     144,   144,   143,   145,   145,   145,   145,   174,   175,   185,
      76,   185,    76,   185,     5,    65,    60,   179,   114,    76,
     174,   175,   103,    76,   127,   133,   222,   222,   222,   190,
     191,   190,   205,   211,   190,    58,    65,    62,    60,    58,
      97,    92,   187,   195,   196,   187,    65,   202,   128,    65,
      64,    64,    58,    63,    65,    64,    63,    53,   124,    50,
     174,   174,   174,   174,   175,   175,    50,    50,     5,    14,
      37,   109,   174,   174,   222,   228,   225,   225,   225,   225,
     225,    63,    58,    65,    30,   185,    76,   185,    76,   185,
       5,    65,    60,    76,   133,   129,    76,   184,    50,   144,
     144,   143,    59,    64,    54,    55,    63,    44,    45,    56,
      77,    65,    65,    76,    76,     5,    19,    64,    26,    36,
      76,    76,    76,    60,   174,   174,    65,    76,    76,    38,
      76,   174,   174,   148,    76,   149,    76,   114,   116,   222,
       5,   129,   140,   129,   140,   146,   175,     5,    60,   180,
      60,   180,    60,   177,     6,    76,   127,   178,     9,    76,
     158,     5,     5,   175,    60,     5,    59,     5,    18,    22,
      17,   192,   143,   213,   214,   215,   216,    41,   203,   222,
     222,   140,    76,    30,     5,    30,    41,    30,    65,    50,
      65,   202,    65,   202,   222,    65,   202,   129,    65,    53,
     175,   175,   175,   175,     5,     5,   174,   174,   187,   187,
      15,   110,   175,   175,   125,   134,    60,   180,    60,   180,
      60,   182,   178,    28,   135,    65,    76,   143,   144,   144,
     145,   145,   145,   145,   135,   174,   174,    76,    76,    76,
      76,   174,   174,    62,   137,   129,   175,   175,   146,    65,
     174,   174,   175,   175,    28,    60,   129,    60,    58,     5,
      65,    58,    63,    30,     5,   179,   178,   178,    60,   178,
      28,    59,    53,     5,     5,   103,    60,   133,    76,   179,
     222,   222,   190,    66,     5,    16,    41,   212,    58,    60,
     190,    66,    60,    17,    60,   196,   187,    65,    65,    65,
       5,     5,   129,     5,    60,    60,     5,    60,   179,   179,
     175,   175,   187,    30,     5,    60,   135,   178,   178,    60,
     178,    76,    30,    65,    30,   175,   175,    65,   174,   174,
     175,   175,   176,    60,     5,    60,    30,    19,   175,   175,
     151,    64,    76,   114,   114,   115,    65,    76,   140,    28,
     179,   165,    53,   133,     8,    76,    76,   222,     5,   133,
       5,     9,   159,    17,    17,   143,   214,   187,   187,    30,
     216,   190,   222,   222,   190,    76,    30,     5,   179,   133,
     133,   179,   133,   167,   168,     5,    60,    59,   179,   133,
      30,   133,     5,    60,    19,   175,   175,     5,     5,    63,
     125,   133,    76,    60,    60,   153,   154,   215,   183,    28,
      58,    65,    60,    63,   130,   163,   103,   222,     5,   179,
       5,   103,   190,   190,    63,    17,   169,     5,     5,   172,
       5,   103,   103,   179,   133,     5,   133,    76,     5,    60,
     138,   133,   133,     5,    60,    65,   151,   114,   114,    28,
     131,   103,     5,   179,   166,     5,   190,   103,   179,   179,
     103,   179,     5,     5,   161,     5,   179,     5,   133,     5,
      76,   154,    64,    28,   153,   129,   129,     5,   164,   103,
       5,   173,   171,     5,   170,   103,   179,     5,    72,    76,
     150,   151,   103,     5,   103,   103,   103,     5,   162,   146,
     153,     5,     5,     5,     5,   103,    65,     5
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
     129,   129,   129,   129,   129,   133,   133,   133,   134,   134,
     135,   135,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   137,   138,   138,   138,   139,
     139,   140,   140,   140,   140,   140,   140,   141,   141,   141,
     141,   141,   142,   142,   142,   142,   142,   142,   142,   143,
     143,   143,   143,   143,   144,   144,   144,   144,   144,   145,
     145,   145,   145,   146,   147,   147,   149,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   150,
     151,   152,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   156,   156,   156,   157,   157,   158,   157,   159,   157,
     160,   157,   161,   157,   162,   157,   163,   157,   164,   157,
     165,   157,   166,   157,   167,   157,   168,   157,   169,   157,
     170,   157,   171,   157,   172,   157,   173,   157,   174,   175,
     175,   175,   176,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   178,   178,   178,   178,   178,
     179,   179,   180,   180,   180,   181,   181,   182,   182,   182,
     182,   182,   182,   183,   183,   184,   185,   185,   186,   188,
     187,   189,   187,   191,   190,   192,   192,   192,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   194,   194,   194,   194,   195,   195,   195,
     196,   196,   197,   197,   198,   198,   198,   199,   199,   199,
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
       4,     6,     9,     0,     5,     6,     4,     2,     4,     3,
       6,     3,     6,     4,     7,     1,     3,     1,     0,     3,
       0,     2,     1,     2,     3,     6,     4,     5,     7,     5,
       5,     6,     8,     7,     0,     3,     0,     3,     3,     1,
       0,     3,     1,     4,     3,     1,     3,     1,     2,     2,
       3,     3,     1,     3,     4,     3,     3,     3,     3,     1,
       2,     2,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     4,     3,     2,     3,     1,     0,     4,     3,     6,
       4,     6,     4,     4,     7,     2,     7,     5,     0,     0,
       0,     7,     9,     5,     0,     3,     1,     6,     0,     2,
       1,     4,     6,     6,     8,    10,     0,     9,     0,    11,
       0,     7,     0,    13,     0,    15,     0,    12,     0,    14,
       0,    11,     0,    13,     0,    11,     0,    11,     0,    12,
       0,    14,     0,    14,     0,    12,     0,    14,     0,     3,
       2,     0,     3,     1,     3,     5,     4,     3,     4,     5,
       6,     4,     2,     2,     2,     1,     3,     3,     3,     1,
       0,     3,     3,     2,     0,     3,     1,     3,     4,     4,
       4,     3,     5,     3,     1,     1,     3,     1,     3,     0,
       4,     0,     2,     0,     2,     1,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     5,     5,     7,     5,     1,     3,     2,
       6,     4,     1,     2,     2,     4,     5,     1,     1,     3,
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
#line 238 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2343 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 239 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2349 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 242 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2355 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 243 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2361 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 246 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2367 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 247 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2373 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 250 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2379 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 251 "pascal.y"
     { (yyval.node) = NULL; }
#line 2385 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 254 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2391 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 257 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2397 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 258 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2403 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 261 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2409 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 262 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2415 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 263 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2421 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 264 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2427 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 265 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2433 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 266 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2439 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 267 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2445 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 270 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2451 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 273 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2457 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 274 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2463 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 277 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2469 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 278 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2475 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 279 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2481 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 280 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2487 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 281 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2493 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 282 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2499 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 285 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2505 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 286 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2511 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 287 "pascal.y"
     { (yyval.node) = NULL; }
#line 2517 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 290 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2523 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 291 "pascal.y"
     { (yyval.node) = NULL; }
#line 2529 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 294 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2535 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 295 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2541 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 296 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2547 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 297 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2553 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 300 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2559 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 301 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2565 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 304 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2571 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 305 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2577 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 308 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2583 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 309 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2589 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 310 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2595 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 311 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2601 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 314 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2607 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 315 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2613 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 318 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2619 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 319 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2625 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 322 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2631 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 323 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2637 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 324 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2643 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 325 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2649 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 326 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2655 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 329 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2661 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 330 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2667 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 333 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2673 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 334 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2679 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 335 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2685 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 336 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2691 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 337 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2697 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 338 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2703 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 341 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2709 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 342 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2715 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 345 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2721 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 346 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2727 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 349 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2733 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 352 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2739 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 353 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2745 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 356 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2751 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 357 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2757 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 360 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2763 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 361 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2769 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 364 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2775 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 367 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2781 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 368 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2787 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 369 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2793 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 372 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2799 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 373 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2805 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 376 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2811 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 377 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2817 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 378 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2823 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 379 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2829 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 380 "pascal.y"
                                                       { }
#line 2835 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 380 "pascal.y"
                                                           { }
#line 2841 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 380 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2847 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 381 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2853 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 382 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2859 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 383 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2865 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 384 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2871 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 385 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2877 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 386 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2883 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 387 "pascal.y"
                         { }
#line 2889 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 387 "pascal.y"
                                               { (yyval.node) = pt_record_or_helper((yyvsp[-4].node), (yyvsp[-1].node)); }
#line 2895 "pascal.tab.c"
    break;

  case 95: /* type: TYPESY IDENT FORSY type class_members ENDSY  */
#line 388 "pascal.y"
                                                 { if (!pt_is_helper_word((yyvsp[-4].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_type_helper((yyvsp[-2].node), (yyvsp[-1].node)); }
#line 2901 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt SETSY OFSY simple_type  */
#line 389 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2907 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY  */
#line 390 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2913 "pascal.tab.c"
    break;

  case 98: /* type: packed_opt FILESY OFSY type  */
#line 391 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2919 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY record_body ENDSY  */
#line 392 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2925 "pascal.tab.c"
    break;

  case 100: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 393 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2931 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY class_members ENDSY  */
#line 394 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2937 "pascal.tab.c"
    break;

  case 102: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 395 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2943 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 396 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2949 "pascal.tab.c"
    break;

  case 104: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 397 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2955 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT  */
#line 400 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2961 "pascal.tab.c"
    break;

  case 106: /* result_type: IDENT PERIOD IDENT  */
#line 401 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2967 "pascal.tab.c"
    break;

  case 107: /* result_type: spec_ref  */
#line 402 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2973 "pascal.tab.c"
    break;

  case 108: /* guid_opt: %empty  */
#line 405 "pascal.y"
    { (yyval.node) = NULL; }
#line 2979 "pascal.tab.c"
    break;

  case 109: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 406 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2985 "pascal.tab.c"
    break;

  case 110: /* class_members: %empty  */
#line 409 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2991 "pascal.tab.c"
    break;

  case 111: /* class_members: class_members class_member  */
#line 410 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2997 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT  */
#line 413 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 3003 "pascal.tab.c"
    break;

  case 113: /* class_member: IDENT SEMICOLON  */
#line 414 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 3009 "pascal.tab.c"
    break;

  case 114: /* class_member: IDENT FORSY IDENT  */
#line 415 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3015 "pascal.tab.c"
    break;

  case 115: /* class_member: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 416 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3021 "pascal.tab.c"
    break;

  case 116: /* class_member: id_list COLON type SEMICOLON  */
#line 417 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3027 "pascal.tab.c"
    break;

  case 117: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 418 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3033 "pascal.tab.c"
    break;

  case 118: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 419 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3039 "pascal.tab.c"
    break;

  case 119: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 420 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 3045 "pascal.tab.c"
    break;

  case 120: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 421 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3051 "pascal.tab.c"
    break;

  case 121: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 422 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3057 "pascal.tab.c"
    break;

  case 122: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 423 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3063 "pascal.tab.c"
    break;

  case 123: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 424 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3069 "pascal.tab.c"
    break;

  case 124: /* prop_index_opt: %empty  */
#line 427 "pascal.y"
    { (yyval.node) = NULL; }
#line 3075 "pascal.tab.c"
    break;

  case 125: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 428 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3081 "pascal.tab.c"
    break;

  case 126: /* prop_specs: %empty  */
#line 431 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3087 "pascal.tab.c"
    break;

  case 127: /* prop_specs: prop_specs IDENT IDENT  */
#line 432 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3093 "pascal.tab.c"
    break;

  case 128: /* prop_specs: prop_specs IDENT INTCONST  */
#line 433 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3099 "pascal.tab.c"
    break;

  case 129: /* packed_opt: PACKEDSY  */
#line 436 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3105 "pascal.tab.c"
    break;

  case 130: /* packed_opt: %empty  */
#line 437 "pascal.y"
     { (yyval.node) = NULL; }
#line 3111 "pascal.tab.c"
    break;

  case 131: /* simple_type: LPARENT enum_list RPARENT  */
#line 440 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3117 "pascal.tab.c"
    break;

  case 132: /* simple_type: IDENT  */
#line 441 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3123 "pascal.tab.c"
    break;

  case 133: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 442 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3129 "pascal.tab.c"
    break;

  case 134: /* simple_type: IDENT PERIOD IDENT  */
#line 443 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3135 "pascal.tab.c"
    break;

  case 135: /* simple_type: spec_ref  */
#line 444 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3141 "pascal.tab.c"
    break;

  case 136: /* simple_type: lo_bound DOTDOT b_expr  */
#line 445 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3147 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_term  */
#line 448 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3153 "pascal.tab.c"
    break;

  case 138: /* lo_bound: PLUS lo_term  */
#line 449 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3159 "pascal.tab.c"
    break;

  case 139: /* lo_bound: MINUS lo_term  */
#line 450 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3165 "pascal.tab.c"
    break;

  case 140: /* lo_bound: lo_bound PLUS b_term  */
#line 451 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3171 "pascal.tab.c"
    break;

  case 141: /* lo_bound: lo_bound MINUS b_term  */
#line 452 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3177 "pascal.tab.c"
    break;

  case 142: /* lo_term: scalar_constant  */
#line 455 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3183 "pascal.tab.c"
    break;

  case 143: /* lo_term: IDENT PERIOD IDENT  */
#line 456 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3189 "pascal.tab.c"
    break;

  case 144: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 457 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3195 "pascal.tab.c"
    break;

  case 145: /* lo_term: lo_term MUL b_factor  */
#line 458 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3201 "pascal.tab.c"
    break;

  case 146: /* lo_term: lo_term IDIV b_factor  */
#line 459 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3207 "pascal.tab.c"
    break;

  case 147: /* lo_term: lo_term IMOD b_factor  */
#line 460 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3213 "pascal.tab.c"
    break;

  case 148: /* lo_term: lo_term SHIFTOP b_factor  */
#line 461 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3219 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_term  */
#line 464 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3225 "pascal.tab.c"
    break;

  case 150: /* b_expr: PLUS b_term  */
#line 465 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3231 "pascal.tab.c"
    break;

  case 151: /* b_expr: MINUS b_term  */
#line 466 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3237 "pascal.tab.c"
    break;

  case 152: /* b_expr: b_expr PLUS b_term  */
#line 467 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3243 "pascal.tab.c"
    break;

  case 153: /* b_expr: b_expr MINUS b_term  */
#line 468 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3249 "pascal.tab.c"
    break;

  case 154: /* b_term: b_factor  */
#line 471 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3255 "pascal.tab.c"
    break;

  case 155: /* b_term: b_term MUL b_factor  */
#line 472 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3261 "pascal.tab.c"
    break;

  case 156: /* b_term: b_term IDIV b_factor  */
#line 473 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3267 "pascal.tab.c"
    break;

  case 157: /* b_term: b_term IMOD b_factor  */
#line 474 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3273 "pascal.tab.c"
    break;

  case 158: /* b_term: b_term SHIFTOP b_factor  */
#line 475 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3279 "pascal.tab.c"
    break;

  case 159: /* b_factor: scalar_constant  */
#line 478 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3285 "pascal.tab.c"
    break;

  case 160: /* b_factor: IDENT PERIOD IDENT  */
#line 479 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3291 "pascal.tab.c"
    break;

  case 161: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 480 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3297 "pascal.tab.c"
    break;

  case 162: /* b_factor: LPARENT b_expr RPARENT  */
#line 481 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3303 "pascal.tab.c"
    break;

  case 163: /* record_body: record_field_list record_case_opt  */
#line 484 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3309 "pascal.tab.c"
    break;

  case 164: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 487 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3315 "pascal.tab.c"
    break;

  case 165: /* record_field_list: record_field  */
#line 488 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3321 "pascal.tab.c"
    break;

  case 166: /* $@4: %empty  */
#line 491 "pascal.y"
                  { }
#line 3327 "pascal.tab.c"
    break;

  case 167: /* record_field: id_list COLON $@4 type  */
#line 491 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3333 "pascal.tab.c"
    break;

  case 168: /* record_field: IDENT FORSY IDENT  */
#line 492 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3339 "pascal.tab.c"
    break;

  case 169: /* record_field: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 493 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3345 "pascal.tab.c"
    break;

  case 170: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 494 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3351 "pascal.tab.c"
    break;

  case 171: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 495 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3357 "pascal.tab.c"
    break;

  case 172: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 496 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3363 "pascal.tab.c"
    break;

  case 173: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 497 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3369 "pascal.tab.c"
    break;

  case 174: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 498 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3375 "pascal.tab.c"
    break;

  case 175: /* record_field: IDENT record_field  */
#line 499 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3381 "pascal.tab.c"
    break;

  case 176: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 500 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3387 "pascal.tab.c"
    break;

  case 177: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 501 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3393 "pascal.tab.c"
    break;

  case 178: /* record_field: %empty  */
#line 502 "pascal.y"
     { (yyval.node) = NULL; }
#line 3399 "pascal.tab.c"
    break;

  case 179: /* case_arm_mark: %empty  */
#line 505 "pascal.y"
    { }
#line 3405 "pascal.tab.c"
    break;

  case 180: /* case_open: %empty  */
#line 508 "pascal.y"
    { }
#line 3411 "pascal.tab.c"
    break;

  case 181: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 511 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3417 "pascal.tab.c"
    break;

  case 182: /* record_case_opt: CASESY IDENT COLON LPARENT enum_list RPARENT OFSY case_open record_case_list  */
#line 512 "pascal.y"
                                                                                  { tree_t *_e = pt_retag((yyvsp[-4].node), TT_ENUM_TYPE); for (int _i = 0; _e && _i < _e->n; _i++) cenv_int(_e->c[_i]->v.sval, _i); (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-7].node)), _e), (yyvsp[0].node)); }
#line 3423 "pascal.tab.c"
    break;

  case 183: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 513 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3429 "pascal.tab.c"
    break;

  case 184: /* record_case_opt: %empty  */
#line 514 "pascal.y"
     { (yyval.node) = NULL; }
#line 3435 "pascal.tab.c"
    break;

  case 185: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 517 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3441 "pascal.tab.c"
    break;

  case 186: /* record_case_list: record_case_arm  */
#line 518 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3447 "pascal.tab.c"
    break;

  case 187: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 521 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3453 "pascal.tab.c"
    break;

  case 188: /* record_case_arm: %empty  */
#line 522 "pascal.y"
     { (yyval.node) = NULL; }
#line 3459 "pascal.tab.c"
    break;

  case 189: /* var_decl_list: var_decl_list var_decl  */
#line 525 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3465 "pascal.tab.c"
    break;

  case 190: /* var_decl_list: var_decl  */
#line 526 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3471 "pascal.tab.c"
    break;

  case 191: /* var_decl: id_list COLON type SEMICOLON  */
#line 529 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3477 "pascal.tab.c"
    break;

  case 192: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 530 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3483 "pascal.tab.c"
    break;

  case 193: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 531 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3489 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 534 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3495 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 535 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3501 "pascal.tab.c"
    break;

  case 196: /* $@5: %empty  */
#line 536 "pascal.y"
                                                                               { }
#line 3507 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 536 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3513 "pascal.tab.c"
    break;

  case 198: /* $@6: %empty  */
#line 537 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3519 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 537 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3525 "pascal.tab.c"
    break;

  case 200: /* $@7: %empty  */
#line 538 "pascal.y"
                                        { }
#line 3531 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 538 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3537 "pascal.tab.c"
    break;

  case 202: /* $@8: %empty  */
#line 539 "pascal.y"
                                                                                                           { }
#line 3543 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 539 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3549 "pascal.tab.c"
    break;

  case 204: /* $@9: %empty  */
#line 540 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3555 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 540 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3561 "pascal.tab.c"
    break;

  case 206: /* $@10: %empty  */
#line 541 "pascal.y"
                                                                                                 { }
#line 3567 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3573 "pascal.tab.c"
    break;

  case 208: /* $@11: %empty  */
#line 542 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3579 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 542 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3585 "pascal.tab.c"
    break;

  case 210: /* $@12: %empty  */
#line 543 "pascal.y"
                                                                                            { }
#line 3591 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 543 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3597 "pascal.tab.c"
    break;

  case 212: /* $@13: %empty  */
#line 544 "pascal.y"
                                                                                                             { }
#line 3603 "pascal.tab.c"
    break;

  case 213: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 544 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3611 "pascal.tab.c"
    break;

  case 214: /* $@14: %empty  */
#line 547 "pascal.y"
                                                                                              { }
#line 3617 "pascal.tab.c"
    break;

  case 215: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 547 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3625 "pascal.tab.c"
    break;

  case 216: /* $@15: %empty  */
#line 550 "pascal.y"
                                                                                             { }
#line 3631 "pascal.tab.c"
    break;

  case 217: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 550 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3639 "pascal.tab.c"
    break;

  case 218: /* $@16: %empty  */
#line 553 "pascal.y"
                                                                                                    { }
#line 3645 "pascal.tab.c"
    break;

  case 219: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 553 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3653 "pascal.tab.c"
    break;

  case 220: /* $@17: %empty  */
#line 556 "pascal.y"
                                                                                                                { }
#line 3659 "pascal.tab.c"
    break;

  case 221: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 556 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3668 "pascal.tab.c"
    break;

  case 222: /* $@18: %empty  */
#line 560 "pascal.y"
                                                                                                                  { }
#line 3674 "pascal.tab.c"
    break;

  case 223: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 560 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3683 "pascal.tab.c"
    break;

  case 224: /* $@19: %empty  */
#line 564 "pascal.y"
                                                                                              { }
#line 3689 "pascal.tab.c"
    break;

  case 225: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 564 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3698 "pascal.tab.c"
    break;

  case 226: /* $@20: %empty  */
#line 568 "pascal.y"
                                                                                                                     { }
#line 3704 "pascal.tab.c"
    break;

  case 227: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 568 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3712 "pascal.tab.c"
    break;

  case 228: /* pv_mark: %empty  */
#line 573 "pascal.y"
    { }
#line 3718 "pascal.tab.c"
    break;

  case 229: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 576 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3724 "pascal.tab.c"
    break;

  case 230: /* parameter_list_opt: LPARENT RPARENT  */
#line 577 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3730 "pascal.tab.c"
    break;

  case 231: /* parameter_list_opt: %empty  */
#line 578 "pascal.y"
     { (yyval.node) = NULL; }
#line 3736 "pascal.tab.c"
    break;

  case 232: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 581 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3742 "pascal.tab.c"
    break;

  case 233: /* parameter_decl_list: parameter_decl  */
#line 582 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3748 "pascal.tab.c"
    break;

  case 234: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 585 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3754 "pascal.tab.c"
    break;

  case 235: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 586 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3760 "pascal.tab.c"
    break;

  case 236: /* parameter_decl: VARSY id_list COLON param_type  */
#line 587 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3766 "pascal.tab.c"
    break;

  case 237: /* parameter_decl: id_list COLON param_type  */
#line 588 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3772 "pascal.tab.c"
    break;

  case 238: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 589 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3778 "pascal.tab.c"
    break;

  case 239: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 590 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3784 "pascal.tab.c"
    break;

  case 240: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 591 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3790 "pascal.tab.c"
    break;

  case 241: /* parameter_decl: IDENT id_list COLON param_type  */
#line 592 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3796 "pascal.tab.c"
    break;

  case 242: /* parameter_decl: VARSY id_list  */
#line 593 "pascal.y"
                   { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "var"); }
#line 3802 "pascal.tab.c"
    break;

  case 243: /* parameter_decl: CONSTSY id_list  */
#line 594 "pascal.y"
                     { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "const"); }
#line 3808 "pascal.tab.c"
    break;

  case 244: /* parameter_decl: IDENT id_list  */
#line 595 "pascal.y"
                   { int cr = (yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, "constref"); if (!(yyvsp[-1].node)->v.sval || (strcasecmp((yyvsp[-1].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3814 "pascal.tab.c"
    break;

  case 245: /* param_type: IDENT  */
#line 598 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3820 "pascal.tab.c"
    break;

  case 246: /* param_type: IDENT PERIOD IDENT  */
#line 599 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3826 "pascal.tab.c"
    break;

  case 247: /* param_type: ARRAYSY OFSY IDENT  */
#line 600 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3832 "pascal.tab.c"
    break;

  case 248: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 601 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3838 "pascal.tab.c"
    break;

  case 249: /* param_type: spec_ref  */
#line 602 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3844 "pascal.tab.c"
    break;

  case 250: /* routine_directives: %empty  */
#line 605 "pascal.y"
    { (yyval.node) = NULL; }
#line 3850 "pascal.tab.c"
    break;

  case 251: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 606 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3863 "pascal.tab.c"
    break;

  case 252: /* pf_params: LPARENT pf_sections RPARENT  */
#line 616 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3869 "pascal.tab.c"
    break;

  case 253: /* pf_params: LPARENT RPARENT  */
#line 617 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3875 "pascal.tab.c"
    break;

  case 254: /* pf_params: %empty  */
#line 618 "pascal.y"
     { (yyval.node) = NULL; }
#line 3881 "pascal.tab.c"
    break;

  case 255: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 621 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3887 "pascal.tab.c"
    break;

  case 256: /* pf_sections: pf_section  */
#line 622 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3893 "pascal.tab.c"
    break;

  case 257: /* pf_section: id_list COLON param_type  */
#line 625 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3899 "pascal.tab.c"
    break;

  case 258: /* pf_section: VARSY id_list COLON param_type  */
#line 626 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3905 "pascal.tab.c"
    break;

  case 259: /* pf_section: CONSTSY id_list COLON param_type  */
#line 627 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3911 "pascal.tab.c"
    break;

  case 260: /* pf_section: IDENT id_list COLON param_type  */
#line 628 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3917 "pascal.tab.c"
    break;

  case 261: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 629 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3923 "pascal.tab.c"
    break;

  case 262: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 630 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3929 "pascal.tab.c"
    break;

  case 263: /* enum_list: enum_list COMMA enum_member  */
#line 633 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3935 "pascal.tab.c"
    break;

  case 264: /* enum_list: enum_member  */
#line 634 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3941 "pascal.tab.c"
    break;

  case 265: /* enum_member: IDENT  */
#line 637 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3947 "pascal.tab.c"
    break;

  case 266: /* id_list: id_list COMMA IDENT  */
#line 640 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3953 "pascal.tab.c"
    break;

  case 267: /* id_list: IDENT  */
#line 641 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3959 "pascal.tab.c"
    break;

  case 268: /* body: BEGINSY statement_list ENDSY  */
#line 644 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3965 "pascal.tab.c"
    break;

  case 269: /* $@21: %empty  */
#line 647 "pascal.y"
                             { pas_stmt_mark(); }
#line 3971 "pascal.tab.c"
    break;

  case 270: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 647 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3977 "pascal.tab.c"
    break;

  case 271: /* $@22: %empty  */
#line 648 "pascal.y"
     { pas_stmt_mark(); }
#line 3983 "pascal.tab.c"
    break;

  case 272: /* statement_list: $@22 statement  */
#line 648 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3989 "pascal.tab.c"
    break;

  case 273: /* $@23: %empty  */
#line 651 "pascal.y"
    { pas_stmt_mark(); }
#line 3995 "pascal.tab.c"
    break;

  case 274: /* body_stmt: $@23 statement  */
#line 651 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 4001 "pascal.tab.c"
    break;

  case 275: /* statement: statement_no_label  */
#line 654 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 4007 "pascal.tab.c"
    break;

  case 276: /* statement: INTCONST COLON statement_no_label  */
#line 655 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4013 "pascal.tab.c"
    break;

  case 277: /* statement: IDENT COLON statement_no_label  */
#line 656 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4019 "pascal.tab.c"
    break;

  case 278: /* statement_no_label: assignment  */
#line 659 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4025 "pascal.tab.c"
    break;

  case 279: /* statement_no_label: call  */
#line 660 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4031 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: compound_statement  */
#line 661 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 4037 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: goto_statement  */
#line 662 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4043 "pascal.tab.c"
    break;

  case 282: /* statement_no_label: if_statement  */
#line 663 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 4049 "pascal.tab.c"
    break;

  case 283: /* statement_no_label: case_statement  */
#line 664 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4055 "pascal.tab.c"
    break;

  case 284: /* statement_no_label: while_statement  */
#line 665 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4061 "pascal.tab.c"
    break;

  case 285: /* statement_no_label: repeat_statement  */
#line 666 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4067 "pascal.tab.c"
    break;

  case 286: /* statement_no_label: for_statement  */
#line 667 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4073 "pascal.tab.c"
    break;

  case 287: /* statement_no_label: with_statement  */
#line 668 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4079 "pascal.tab.c"
    break;

  case 288: /* statement_no_label: try_statement  */
#line 669 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4085 "pascal.tab.c"
    break;

  case 289: /* statement_no_label: raise_statement  */
#line 670 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4091 "pascal.tab.c"
    break;

  case 290: /* statement_no_label: inherited_call  */
#line 671 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4097 "pascal.tab.c"
    break;

  case 291: /* statement_no_label: INHERITEDSY  */
#line 672 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4103 "pascal.tab.c"
    break;

  case 292: /* statement_no_label: %empty  */
#line 673 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4109 "pascal.tab.c"
    break;

  case 293: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 676 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4115 "pascal.tab.c"
    break;

  case 294: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 677 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4121 "pascal.tab.c"
    break;

  case 295: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 678 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4127 "pascal.tab.c"
    break;

  case 296: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 679 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4133 "pascal.tab.c"
    break;

  case 297: /* handler_list: handler  */
#line 682 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4139 "pascal.tab.c"
    break;

  case 298: /* handler_list: handler_list SEMICOLON handler  */
#line 683 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4145 "pascal.tab.c"
    break;

  case 299: /* handler_list: handler_list SEMICOLON  */
#line 684 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4151 "pascal.tab.c"
    break;

  case 300: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 687 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4157 "pascal.tab.c"
    break;

  case 301: /* handler: ONSY IDENT DOSY body_stmt  */
#line 688 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4163 "pascal.tab.c"
    break;

  case 302: /* raise_statement: RAISESY  */
#line 691 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4169 "pascal.tab.c"
    break;

  case 303: /* raise_statement: RAISESY expression  */
#line 692 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4175 "pascal.tab.c"
    break;

  case 304: /* inherited_call: INHERITEDSY IDENT  */
#line 695 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4181 "pascal.tab.c"
    break;

  case 305: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 696 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4187 "pascal.tab.c"
    break;

  case 306: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 697 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4193 "pascal.tab.c"
    break;

  case 307: /* call: IDENT  */
#line 700 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4199 "pascal.tab.c"
    break;

  case 308: /* call: spec_expr  */
#line 701 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4205 "pascal.tab.c"
    break;

  case 309: /* call: selector PERIOD IDENT  */
#line 702 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4211 "pascal.tab.c"
    break;

  case 310: /* call: call_with_args PERIOD IDENT  */
#line 703 "pascal.y"
                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4217 "pascal.tab.c"
    break;

  case 311: /* call: call_with_args  */
#line 704 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4223 "pascal.tab.c"
    break;

  case 312: /* recv_primary: STRINGCONST  */
#line 707 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4229 "pascal.tab.c"
    break;

  case 313: /* recv_primary: LPARENT expression RPARENT  */
#line 708 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4235 "pascal.tab.c"
    break;

  case 314: /* recv_primary: spec_expr  */
#line 709 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4241 "pascal.tab.c"
    break;

  case 315: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 712 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4247 "pascal.tab.c"
    break;

  case 316: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 713 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4253 "pascal.tab.c"
    break;

  case 317: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 714 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4259 "pascal.tab.c"
    break;

  case 318: /* call_with_args: IDENT LPARENT RPARENT  */
#line 715 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4265 "pascal.tab.c"
    break;

  case 319: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 716 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4271 "pascal.tab.c"
    break;

  case 320: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 717 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4277 "pascal.tab.c"
    break;

  case 321: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 718 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4283 "pascal.tab.c"
    break;

  case 322: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 719 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4289 "pascal.tab.c"
    break;

  case 323: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 720 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4295 "pascal.tab.c"
    break;

  case 324: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 721 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4301 "pascal.tab.c"
    break;

  case 325: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 722 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4307 "pascal.tab.c"
    break;

  case 326: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 723 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4313 "pascal.tab.c"
    break;

  case 327: /* argument_list: argument_list COMMA argument  */
#line 726 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4319 "pascal.tab.c"
    break;

  case 328: /* argument_list: argument  */
#line 727 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4325 "pascal.tab.c"
    break;

  case 329: /* argument: expression  */
#line 730 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4331 "pascal.tab.c"
    break;

  case 330: /* argument: expression COLON expression  */
#line 731 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4337 "pascal.tab.c"
    break;

  case 331: /* argument: expression COLON expression COLON expression  */
#line 732 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4343 "pascal.tab.c"
    break;

  case 332: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 733 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4349 "pascal.tab.c"
    break;

  case 333: /* assignment: selector BECOMES expression  */
#line 736 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4355 "pascal.tab.c"
    break;

  case 334: /* assignment: selector ADDBECOMES expression  */
#line 737 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4361 "pascal.tab.c"
    break;

  case 335: /* assignment: selector SUBBECOMES expression  */
#line 738 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4367 "pascal.tab.c"
    break;

  case 336: /* assignment: selector MULBECOMES expression  */
#line 739 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4373 "pascal.tab.c"
    break;

  case 337: /* assignment: selector DIVBECOMES expression  */
#line 740 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4379 "pascal.tab.c"
    break;

  case 338: /* assignment: call_with_args BECOMES expression  */
#line 741 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4385 "pascal.tab.c"
    break;

  case 339: /* selector: selector LBRACK expression_list RBRACK  */
#line 744 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4391 "pascal.tab.c"
    break;

  case 340: /* selector: selector PERIOD IDENT  */
#line 745 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4397 "pascal.tab.c"
    break;

  case 341: /* selector: selector ARROW  */
#line 746 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4403 "pascal.tab.c"
    break;

  case 342: /* selector: call_with_args PERIOD IDENT  */
#line 747 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4409 "pascal.tab.c"
    break;

  case 343: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 748 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4415 "pascal.tab.c"
    break;

  case 344: /* selector: call_with_args ARROW  */
#line 749 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4421 "pascal.tab.c"
    break;

  case 345: /* selector: recv_primary PERIOD IDENT  */
#line 750 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4427 "pascal.tab.c"
    break;

  case 346: /* selector: recv_primary ARROW  */
#line 751 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4433 "pascal.tab.c"
    break;

  case 347: /* selector: IDENT  */
#line 752 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4439 "pascal.tab.c"
    break;

  case 348: /* expression_list: expression_list COMMA expression  */
#line 755 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4445 "pascal.tab.c"
    break;

  case 349: /* expression_list: expression  */
#line 756 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4451 "pascal.tab.c"
    break;

  case 350: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 759 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4457 "pascal.tab.c"
    break;

  case 351: /* goto_statement: GOTOSY INTCONST  */
#line 762 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4463 "pascal.tab.c"
    break;

  case 352: /* goto_statement: GOTOSY IDENT  */
#line 763 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4469 "pascal.tab.c"
    break;

  case 353: /* if_statement: IFSY expression THENSY body_stmt  */
#line 766 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4475 "pascal.tab.c"
    break;

  case 354: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 767 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4481 "pascal.tab.c"
    break;

  case 355: /* $@24: %empty  */
#line 770 "pascal.y"
                           { }
#line 4487 "pascal.tab.c"
    break;

  case 356: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 770 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4493 "pascal.tab.c"
    break;

  case 357: /* case_tail: %empty  */
#line 773 "pascal.y"
    { (yyval.node) = NULL; }
#line 4499 "pascal.tab.c"
    break;

  case 358: /* case_tail: ELSESY statement_list  */
#line 774 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4505 "pascal.tab.c"
    break;

  case 359: /* case_tail: OTHERWISESY statement_list  */
#line 775 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4511 "pascal.tab.c"
    break;

  case 360: /* case_list: case_list SEMICOLON case_elem  */
#line 778 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4517 "pascal.tab.c"
    break;

  case 361: /* case_list: case_elem  */
#line 779 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4523 "pascal.tab.c"
    break;

  case 362: /* case_elem: constant_list COLON body_stmt  */
#line 782 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4529 "pascal.tab.c"
    break;

  case 363: /* case_elem: %empty  */
#line 783 "pascal.y"
     { (yyval.node) = NULL; }
#line 4535 "pascal.tab.c"
    break;

  case 364: /* constant_list: constant_list COMMA case_label  */
#line 786 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4541 "pascal.tab.c"
    break;

  case 365: /* constant_list: case_label  */
#line 787 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4547 "pascal.tab.c"
    break;

  case 366: /* case_label: b_expr  */
#line 790 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4553 "pascal.tab.c"
    break;

  case 367: /* case_label: b_expr DOTDOT b_expr  */
#line 791 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4559 "pascal.tab.c"
    break;

  case 368: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 794 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4565 "pascal.tab.c"
    break;

  case 369: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 797 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4571 "pascal.tab.c"
    break;

  case 370: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 800 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4577 "pascal.tab.c"
    break;

  case 371: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 801 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4583 "pascal.tab.c"
    break;

  case 372: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 802 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4589 "pascal.tab.c"
    break;

  case 373: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 805 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4595 "pascal.tab.c"
    break;

  case 374: /* with_open: with_open COMMA selector  */
#line 808 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4601 "pascal.tab.c"
    break;

  case 375: /* with_open: selector  */
#line 809 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4607 "pascal.tab.c"
    break;

  case 376: /* expression: simple_expression  */
#line 816 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4613 "pascal.tab.c"
    break;

  case 377: /* expression: expression INOP simple_expression  */
#line 817 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4619 "pascal.tab.c"
    break;

  case 378: /* expression: expression LTOP simple_expression  */
#line 818 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4625 "pascal.tab.c"
    break;

  case 379: /* expression: expression LEOP simple_expression  */
#line 819 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4631 "pascal.tab.c"
    break;

  case 380: /* expression: expression GTOP simple_expression  */
#line 820 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4637 "pascal.tab.c"
    break;

  case 381: /* expression: expression GEOP simple_expression  */
#line 821 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4643 "pascal.tab.c"
    break;

  case 382: /* expression: expression NEOP simple_expression  */
#line 822 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4649 "pascal.tab.c"
    break;

  case 383: /* expression: expression EQOP simple_expression  */
#line 823 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4655 "pascal.tab.c"
    break;

  case 384: /* expression: expression ISOP simple_expression  */
#line 824 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4661 "pascal.tab.c"
    break;

  case 385: /* simple_expression: term  */
#line 827 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4667 "pascal.tab.c"
    break;

  case 386: /* simple_expression: PLUS term  */
#line 828 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4673 "pascal.tab.c"
    break;

  case 387: /* simple_expression: MINUS term  */
#line 829 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4679 "pascal.tab.c"
    break;

  case 388: /* simple_expression: simple_expression PLUS term  */
#line 830 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4685 "pascal.tab.c"
    break;

  case 389: /* simple_expression: simple_expression MINUS term  */
#line 831 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4691 "pascal.tab.c"
    break;

  case 390: /* simple_expression: simple_expression OROP term  */
#line 832 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4697 "pascal.tab.c"
    break;

  case 391: /* simple_expression: simple_expression XOROP term  */
#line 833 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4703 "pascal.tab.c"
    break;

  case 392: /* term: power  */
#line 836 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4709 "pascal.tab.c"
    break;

  case 393: /* term: term MUL power  */
#line 837 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4715 "pascal.tab.c"
    break;

  case 394: /* term: term RDIV power  */
#line 838 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4721 "pascal.tab.c"
    break;

  case 395: /* term: term IDIV power  */
#line 839 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4727 "pascal.tab.c"
    break;

  case 396: /* term: term IMOD power  */
#line 840 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4733 "pascal.tab.c"
    break;

  case 397: /* term: term ANDOP power  */
#line 841 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4739 "pascal.tab.c"
    break;

  case 398: /* term: term MUL MINUS power  */
#line 842 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4745 "pascal.tab.c"
    break;

  case 399: /* term: term RDIV MINUS power  */
#line 843 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4751 "pascal.tab.c"
    break;

  case 400: /* term: term IDIV MINUS power  */
#line 844 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4757 "pascal.tab.c"
    break;

  case 401: /* term: term IMOD MINUS power  */
#line 845 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4763 "pascal.tab.c"
    break;

  case 402: /* term: term ANDOP MINUS power  */
#line 846 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4769 "pascal.tab.c"
    break;

  case 403: /* term: term SHIFTOP power  */
#line 847 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4775 "pascal.tab.c"
    break;

  case 404: /* term: term ASOP power  */
#line 848 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4781 "pascal.tab.c"
    break;

  case 405: /* power: factor  */
#line 851 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4787 "pascal.tab.c"
    break;

  case 406: /* power: power POWOP factor  */
#line 852 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4793 "pascal.tab.c"
    break;

  case 407: /* factor: selector  */
#line 855 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4799 "pascal.tab.c"
    break;

  case 408: /* factor: call_with_args  */
#line 856 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4805 "pascal.tab.c"
    break;

  case 409: /* factor: INTCONST  */
#line 857 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4811 "pascal.tab.c"
    break;

  case 410: /* factor: REALCONST  */
#line 858 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4817 "pascal.tab.c"
    break;

  case 411: /* factor: STRINGCONST  */
#line 859 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4823 "pascal.tab.c"
    break;

  case 412: /* factor: CHARCODE  */
#line 860 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4829 "pascal.tab.c"
    break;

  case 413: /* factor: LPARENT expression RPARENT  */
#line 861 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4835 "pascal.tab.c"
    break;

  case 414: /* factor: inherited_call  */
#line 862 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4841 "pascal.tab.c"
    break;

  case 415: /* factor: NOTSY factor  */
#line 863 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4847 "pascal.tab.c"
    break;

  case 416: /* factor: ATSIGN factor  */
#line 864 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4853 "pascal.tab.c"
    break;

  case 417: /* factor: LBRACK RBRACK  */
#line 865 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4859 "pascal.tab.c"
    break;

  case 418: /* factor: LBRACK set_member_list RBRACK  */
#line 866 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4865 "pascal.tab.c"
    break;

  case 419: /* set_member_list: set_member  */
#line 869 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4871 "pascal.tab.c"
    break;

  case 420: /* set_member_list: set_member_list COMMA set_member  */
#line 870 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4877 "pascal.tab.c"
    break;

  case 421: /* set_member: expression  */
#line 873 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4883 "pascal.tab.c"
    break;

  case 422: /* set_member: expression DOTDOT expression  */
#line 874 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4889 "pascal.tab.c"
    break;


#line 4893 "pascal.tab.c"

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

#line 880 "pascal.y"

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
