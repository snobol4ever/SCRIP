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
#line 32 "pascal.y"

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
#define YYLAST   1950

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1024

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
       0,   239,   239,   240,   243,   244,   247,   248,   251,   252,
     255,   258,   259,   262,   263,   264,   265,   266,   267,   268,
     271,   274,   275,   278,   279,   280,   281,   282,   283,   286,
     287,   288,   291,   292,   295,   296,   297,   298,   301,   302,
     305,   306,   309,   310,   311,   312,   315,   316,   319,   320,
     323,   324,   325,   326,   327,   330,   331,   334,   335,   336,
     337,   338,   339,   342,   343,   346,   347,   350,   353,   354,
     357,   358,   361,   362,   365,   368,   369,   370,   373,   374,
     377,   378,   379,   380,   381,   381,   381,   382,   383,   384,
     385,   386,   387,   388,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   401,   402,   403,   406,   407,
     410,   411,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   428,   429,   432,   433,   434,   437,
     438,   441,   442,   443,   444,   445,   446,   449,   450,   451,
     452,   453,   456,   457,   458,   459,   460,   461,   462,   465,
     466,   467,   468,   469,   472,   473,   474,   475,   476,   479,
     480,   481,   482,   485,   488,   489,   492,   492,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   506,
     509,   512,   513,   514,   515,   518,   519,   522,   523,   526,
     527,   530,   531,   532,   535,   536,   537,   537,   538,   538,
     539,   539,   540,   540,   541,   541,   542,   542,   543,   543,
     544,   544,   545,   545,   548,   548,   551,   551,   554,   554,
     557,   557,   561,   561,   565,   565,   569,   569,   574,   577,
     578,   579,   582,   583,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   599,   600,   601,   602,   603,
     606,   607,   616,   621,   628,   629,   630,   633,   634,   637,
     638,   639,   640,   641,   642,   645,   646,   649,   652,   653,
     656,   659,   659,   660,   660,   663,   663,   666,   667,   668,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   688,   689,   690,   691,   694,
     695,   696,   699,   700,   703,   704,   707,   708,   709,   712,
     713,   714,   715,   716,   719,   720,   721,   724,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   738,
     739,   742,   743,   744,   745,   748,   749,   750,   751,   752,
     753,   756,   757,   758,   759,   760,   761,   762,   763,   764,
     767,   768,   771,   774,   775,   778,   779,   782,   782,   785,
     786,   787,   790,   791,   794,   795,   798,   799,   802,   803,
     806,   809,   812,   813,   814,   817,   820,   821,   828,   829,
     830,   831,   832,   833,   834,   835,   836,   839,   840,   841,
     842,   843,   844,   845,   848,   849,   850,   851,   852,   853,
     854,   855,   856,   857,   858,   859,   860,   863,   864,   867,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   881,   882,   885,   886
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

#define YYPACT_NINF (-920)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-350)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      42,   -22,     0,   235,  -920,   189,  1075,  -920,  -920,   506,
    -920,    86,  -920,  -920,   515,   214,     0,   272,   408,   408,
     455,  -920,   168,   141,   466,   483,   270,  -920,  -920,  -920,
     509,   408,   305,   265,   525,  -920,  -920,   146,   108,   214,
    -920,   148,   316,   372,  -920,   408,  -920,   513,   154,   386,
      40,   971,   575,   536,   168,  -920,   548,   557,   587,   431,
     482,   617,   620,  -920,   105,  -920,  -920,  -920,  -920,   627,
    1496,  1112,  -920,  -920,     0,   408,   646,   608,  -920,   669,
    1112,  -920,   408,   672,    62,  -920,  -920,   649,   682,  -920,
    1496,   248,  1496,  -920,  1496,  1496,   591,  -920,  1235,  -920,
    1496,   703,   710,  -920,   554,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,   650,   606,  -920,  1166,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,   408,  1194,   662,  -920,   745,   775,
     786,   738,   774,   688,   804,  -920,   797,   792,  -920,  -920,
    1704,  1704,  1704,  1271,  1496,  1704,  -920,  -920,  -920,   719,
     163,   703,   799,  -920,   723,   712,   249,   262,   660,   789,
    -920,   593,   814,  -920,   814,   805,   766,   766,   260,   806,
    -920,  -920,  -920,  -920,  1581,   177,   397,  -920,  -920,  -920,
    -920,   833,   476,  -920,   284,   212,   316,   192,  -920,   346,
     888,  -920,    52,   390,  -920,  -920,   843,   971,  -920,  -920,
     726,   401,   678,   723,   712,   145,  1260,   231,  1148,  1745,
      60,    60,  1321,  1297,    35,  1869,   842,   860,  1346,   841,
    -920,  1496,   849,  -920,  1496,  1371,  1496,   852,  -920,  1496,
    1496,  1496,  1496,  1496,   396,    37,   136,   925,   408,   855,
     861,   116,  -920,  -920,   408,   408,  -920,   214,     0,  -920,
     862,   408,   863,   168,  -920,  -920,   660,   660,  -920,  1763,
     634,  -920,  1782,  -920,   867,   869,  -920,  1496,  1496,  1496,
    1496,  1496,  1496,  1496,  1496,  1704,  1704,  1704,  1704,  1587,
    1612,  1637,  1662,  1687,  1704,  1704,  1704,   859,   290,  -920,
     363,   908,   882,   927,   578,   212,   212,    -4,  -920,  -920,
     503,  -920,  1297,   871,  1764,  1764,   872,   290,   517,   881,
     883,   885,   150,   886,   887,   889,   934,   208,  -920,   755,
    1521,   201,   939,   940,  -920,   461,   461,  1764,   461,   461,
     461,   461,  -920,   608,   408,   895,   408,   901,  -920,   408,
      72,  -920,   762,  -920,  -920,  1521,   903,  -920,   608,  -920,
     292,  -920,  1496,  1496,  1496,  -920,  -920,   248,  -920,  -920,
    -920,  -920,   579,  -920,  -920,  -920,   518,  -920,   731,  1800,
      -2,  -920,   890,  -920,  1396,  1297,  -920,   521,   914,  1869,
     348,   677,  1869,  -920,   537,  1869,  1866,   680,  1869,  1869,
    1869,  1869,   928,  -920,  -920,   290,  -920,   438,  -920,  -920,
    -920,  -920,   608,   608,   463,   502,   214,   161,  1063,  -920,
     408,  -920,   168,  1496,  1496,  -920,   785,   916,   922,   262,
     262,   262,   262,   262,   262,   262,   262,   660,   660,   660,
     660,  1704,   789,  1704,   789,  1704,   789,  1704,   789,  1704,
     789,   789,   789,  -920,   924,    -4,   551,  -920,  -920,   643,
     408,   913,   408,   917,  -920,   408,    77,  -920,   763,   918,
     292,  1112,   923,   806,  -920,   504,   414,   461,   461,  1764,
     692,  -920,   460,   325,  -920,    -5,  -920,   556,   926,  -920,
     929,   206,   377,   931,   933,   936,  -920,   791,  -920,  -920,
     935,   938,   941,  -920,   196,  -920,  -920,  -920,   257,   947,
    -920,  -920,  1546,   996,  1869,  1112,  1297,  1112,  1297,   257,
     325,   325,   445,  -920,  -920,  -920,  -920,   608,  1010,   802,
     814,   813,   814,   816,    57,  -920,    27,    53,  1020,  1021,
     608,   967,  1029,   132,  -920,  1031,  1234,   902,  1869,  -920,
     971,  -920,   712,  1764,   997,  1496,  -920,  1496,  1496,  1297,
    -920,   961,   299,   244,  -920,   321,  -920,   571,   508,  -920,
    1421,  1446,  1496,  -920,  -920,  1471,  -920,  1112,   594,   991,
     608,   608,   608,   608,  1040,  1043,  -920,  -920,  -920,  -920,
    -920,  1034,   608,   608,  1869,  -920,   789,   789,   789,   789,
     789,  -920,   290,   989,  -920,   834,   814,   839,   814,   840,
      70,  -920,    27,  -920,  1025,  -920,  -920,  -920,  -920,   325,
     325,   411,   978,  1764,   461,   461,  -920,   461,   461,   461,
     461,  -920,  -920,  -920,  -920,  -920,   981,   984,   986,   987,
    -920,  -920,   994,  1112,   608,   608,   257,  -920,  1002,  -920,
    -920,   608,   608,  -920,   247,  1112,   218,  1011,    85,  1782,
    -920,  -920,   681,  -920,  -920,  1038,  1069,  -920,    27,  -920,
      27,  1015,    27,  -920,  1048,   199,  -920,  1026,  1073,    63,
    -920,  -920,  -920,  1024,   292,  -920,  1004,  -920,  1496,  1496,
    -920,  -920,   417,   287,  -920,   858,  -920,  -920,  -920,  1816,
    1835,  -920,   139,  -920,   890,  -920,  -920,  -920,  -920,  -920,
    -920,   596,  -920,   613,  1869,  -920,   616,  1076,  1081,  1112,
    1082,  1028,  1030,   133,  -920,  -920,   608,   608,  1086,  1086,
    -920,  1062,  1088,  1036,  -920,  -920,    27,  -920,    27,  1044,
      27,  -920,  -920,  1018,   736,  -920,  -920,   465,   325,   325,
    -920,  -920,  -920,  -920,   831,   608,   608,  -920,  1041,  -920,
    -920,   608,   608,    57,  1045,  1098,  -920,  1049,  1078,  1094,
     608,   608,  -920,  -920,  -920,   289,  -920,  1521,  1521,   416,
    -920,  1297,  1095,  -920,  -920,  1050,  1072,  -920,   292,  -920,
      58,  1051,  1496,  -920,  -920,  1053,  1056,  1128,   292,  1135,
    -920,    55,   980,  1160,  -920,  1764,  1764,  -920,  -920,  1115,
    1764,  -920,  -920,  1496,  1496,  -920,  1070,  -920,   327,  -920,
    -920,  -920,  -920,  -920,  1142,  -920,   292,   292,  -920,   292,
    1050,  1050,  1145,  1097,  1086,  1099,  -920,   292,   930,  -920,
    -920,   292,  -920,  -920,  -920,  -920,  -920,  1149,  1100,  1140,
     608,   608,  1156,  1157,    89,   290,  -920,   292,  -920,  1087,
    1109,  1110,  1764,   806,  1143,  -920,  -920,   618,  -920,  1114,
    1117,  -920,  1050,  -920,  1496,  -920,  -920,  -920,  -920,  1869,
    1167,  1170,  -920,  1173,  -920,  1177,  -920,  -920,  -920,   445,
    -920,  1086,  1086,  -920,  -920,  -920,  1850,  1869,  -920,  1172,
    -920,  -920,  1050,  1178,  1186,  1050,  1188,  -920,  -920,  -920,
     292,  -920,  1050,  1189,  -920,  -920,  -920,   292,  1127,  1200,
    1154,  -920,  -920,  -920,  -920,  -920,  -920,   292,   292,  1210,
    -920,   866,   626,  -920,  1521,  -920,  1521,  1191,  -920,  -920,
    1211,  1869,  -920,  -920,  -920,  1050,  -920,  1217,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  1221,  1226,  1050,
    1227,  -920,  1237,  -920,  -920,   292,    54,  -920,  -920,  1764,
    1175,  1212,  1764,  -920,  -920,  1112,  1112,  1238,  -920,  1050,
    -920,  -920,  -920,  1239,  1050,  1050,  1240,  1050,  -920,  -920,
    -920,  -920,  1050,  -920,  1241,  -920,   693,  -920,  -920,  -920,
    1210,  -920,  -920,  -920,  -920,  1242,  -920,  -920,  -920,  -920,
    -920,  1245,  1050,  -920,  -920,  -920,   257,  1764,  1246,  -920,
    1248,  1249,  1252,  -920,  -920,  1196,  1210,  -920,  -920,  -920,
    -920,  1257,  -920,  -920
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   273,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   228,   269,    16,   190,     0,     0,   228,
       0,   294,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   130,    38,    18,     0,     0,     0,   231,   189,     0,
     130,    19,     0,     0,   231,   271,   270,     0,     0,   273,
       0,     0,     0,   273,     0,     0,     0,   314,   309,   273,
     304,   293,     0,    69,   310,    68,   274,   277,   290,   291,
     292,   281,     0,   313,   280,     0,   282,   283,   284,   285,
     286,   287,   288,   289,     0,   130,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   411,   414,   412,   413,
     349,     0,   316,   416,   410,   409,     0,   378,   387,   394,
     407,   108,   256,   129,   256,     0,     0,     0,     0,     0,
      51,    54,    52,    53,   132,   110,   178,   142,    76,    75,
     135,     0,     0,    80,     0,   137,    64,     0,   228,     0,
       0,   268,     0,     0,   228,   200,     0,   294,   353,   354,
       0,     0,     0,     0,   377,     0,     0,     0,     0,     0,
     294,   294,     0,     0,     0,   305,   306,     0,     0,     0,
     348,     0,     0,   346,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,   108,   110,     0,     0,     0,
       0,     0,   228,   228,     0,     0,     4,     0,     0,    12,
       0,     0,     0,     0,    21,   417,   388,   389,   419,   423,
       0,   421,     0,   418,     0,     0,    40,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
       0,    88,     0,     0,    50,   138,   139,    81,    83,   267,
       0,   266,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,     0,     0,   184,   165,     0,
       0,     0,    97,     0,    93,     0,     0,     0,     0,     0,
       0,     0,   228,   231,     0,     0,     0,     0,   230,   269,
       0,   233,     0,   250,   191,     0,     0,   228,   231,    12,
       0,   272,     0,     0,     0,   275,   275,     0,   357,   352,
     275,   315,   309,   278,   279,   320,     0,   330,   409,   331,
       0,    78,   273,   273,     0,     0,   319,     0,   347,   340,
     312,     0,   351,   322,     0,   335,   311,     0,   336,   337,
     338,   339,     0,    61,    60,     0,    57,     0,   228,   228,
     228,   228,   231,   231,     0,     0,    24,     0,    31,   228,
      26,   228,    25,     0,     0,   420,   415,   344,   342,   379,
     380,   381,   382,   383,   384,   385,   386,   392,   390,   391,
     393,     0,   397,     0,   398,     0,   399,     0,   395,     0,
     396,   405,   406,   408,     0,    72,     0,    70,    73,     0,
       0,     0,     0,     0,   255,   269,     0,   258,     0,     0,
       0,   130,     0,     0,   131,     0,   134,     0,     0,     0,
      50,   159,     0,   149,   154,     0,    82,     0,     0,   101,
       0,   112,     0,     0,     0,     0,   111,     0,   228,   228,
       0,     0,     0,   175,     0,   228,   228,    99,   178,     0,
     163,   166,     0,     0,    42,   130,     0,   130,     0,   178,
     140,   141,   136,   146,   147,   145,   148,   231,     0,   243,
     256,   242,   256,   244,     0,   229,     0,   196,     0,     0,
     231,     0,     0,   105,   107,     0,     0,     0,   371,   370,
     294,   375,   376,   365,   355,     0,   317,     0,     0,     0,
      67,     0,     0,     0,   299,     0,   307,     0,     0,   318,
       0,     0,     0,   345,   321,     0,   341,   130,     0,     0,
     231,   231,   231,   231,     0,     0,   228,   228,    23,   273,
     273,    33,   231,   231,   424,   422,   402,   403,   404,   400,
     401,   109,     0,   108,   103,     0,   256,     0,   256,     0,
       0,   254,     0,    89,    90,   110,   143,   265,    77,   150,
     151,     0,     0,     0,     0,     0,   133,     0,     0,     0,
       0,   144,   110,   228,   228,   113,     0,     0,     0,     0,
     228,   228,   124,   130,   231,   231,   178,   168,     0,   228,
     228,   231,   231,   164,     0,   130,   349,     0,     0,    42,
      41,    87,     0,    98,    96,     0,     0,   250,     0,   234,
       0,     0,     0,   232,     0,   245,   249,   237,     0,     0,
      12,   192,   193,     0,     0,   201,     0,   250,     0,     0,
     275,   276,   368,   359,   363,     0,   367,   275,   329,   351,
     332,    79,     0,   295,   301,   296,   273,   298,   308,    74,
     328,     0,   326,     0,   350,   324,     0,     0,   110,   130,
       0,     0,     0,     0,   250,   250,   231,   231,    29,    30,
     273,     0,     0,     0,    71,   110,     0,   263,     0,     0,
       0,   257,   259,     0,     0,   162,   160,     0,   152,   153,
     156,   157,   155,   158,     0,   231,   231,   114,     0,   228,
     228,   231,   231,     0,     0,     0,   170,     0,     0,     0,
     231,   231,   172,   173,   180,     0,   167,     0,     0,     0,
      44,     0,     0,    94,   250,   210,   238,   236,     0,   241,
       0,     0,     0,   194,   251,     0,     0,     0,     0,     0,
     106,   198,     0,     0,   374,     0,   365,   273,   273,     0,
       0,   275,   356,     0,     0,   275,     0,   300,     0,   327,
     325,   323,    59,    62,     0,   250,     0,     0,   250,     0,
     214,   216,     0,     0,    32,     0,   250,     0,     0,   261,
     260,     0,   262,    91,    95,   161,   102,     0,     0,     0,
     231,   231,     0,     0,     0,     0,   116,     0,   100,     0,
       0,   177,   188,     0,     0,    48,    46,     0,    45,     0,
       0,    84,   206,    12,     0,   235,   248,   247,   246,   239,
       0,     0,   197,     0,   250,     0,    12,   275,   275,   369,
     362,   361,   360,   358,   366,   364,     0,   333,   303,     0,
     297,    58,   218,     0,     0,   224,     0,    12,    12,   250,
       0,    20,    27,     0,   104,   264,   117,     0,     0,     0,
       0,   119,   120,   125,   126,   171,   169,     0,     0,   183,
     186,     0,     0,   180,     0,    43,     0,     0,    85,    12,
       0,   240,   252,   253,   250,   212,   195,     0,   373,   372,
     334,   275,    12,   250,   250,    12,   250,     0,     0,   202,
       0,   250,     0,   115,   121,     0,     0,   176,   174,   188,
       0,     0,   188,    47,    49,   130,   130,     0,   211,   208,
      12,   199,   302,     0,   226,   222,     0,   220,   215,   217,
      12,   250,    28,   118,     0,   123,     0,   185,   179,   180,
     181,    92,    86,   207,    12,     0,   219,    12,    12,   225,
      12,     0,   204,   122,   128,   127,   178,   188,     0,   213,
       0,     0,     0,   203,    12,     0,   182,   209,   227,   223,
     221,     0,   187,   205
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -920,  -920,  -920,  -920,  -920,   -65,  1014,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  1032,   -26,  -342,  -920,  -920,   -56,
    1022,   -48,  1042,    34,   -70,   -32,  -270,  -569,   -64,   -28,
    -263,   -78,  -920,  -920,  -920,  -443,   685,  -278,  -920,  -920,
    -920,  -920,  -193,  -920,   234,  -279,  -294,  -257,  -504,  -920,
    -282,  -920,  -920,  -882,  -920,  -919,   332,  1047,   -18,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,   641,   224,   539,   769,
    -475,   198,  -143,  -920,   700,   448,   844,   170,  -920,   -85,
    -920,  -920,  -338,  -920,  -188,   307,  -920,  -920,   622,  -920,
     -13,  -920,  -920,   -39,  -167,   772,  -920,   -16,  1103,  -920,
    -920,  -920,  -920,  -920,  -920,  -920,   522,  -515,   519,  -920,
    -920,  -920,  -920,  -920,   456,   848,   -55,  -166,  -111,  -920,
     907
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     254,   581,   721,    37,    39,    40,   503,   857,   648,   471,
      54,    55,    41,    42,   103,   152,   446,   447,   105,   180,
     370,   181,   928,   966,   509,   535,   289,   308,   486,   754,
     956,   182,   183,   184,   185,   682,   473,   474,   316,   317,
     318,   645,  1006,   852,   500,   919,   920,    45,    46,    28,
     670,   876,   349,   980,  1014,   929,   994,   863,   970,   897,
     898,   942,  1000,   998,   945,   997,    77,   190,   340,   341,
     667,   527,   291,   456,   457,   300,   301,   319,    29,    50,
     197,    51,   539,   540,   106,   107,   108,   553,   554,   109,
     153,   111,   112,   154,   366,   367,   114,   155,   387,   116,
     117,   118,   119,   543,   799,   683,   684,   921,   686,   120,
     121,   122,   123,   205,   369,   157,   158,   159,   160,   260,
     261
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   178,   192,   528,   201,   655,   127,   179,   207,   351,
     178,   449,   113,    72,   214,   177,   179,   604,   541,   104,
     371,   292,   544,   724,   177,   472,   475,    78,   685,   255,
     493,   510,   511,   664,   263,   115,    11,   477,   110,   465,
      85,   962,   393,   990,   302,    85,     1,   237,   512,   614,
     615,   377,   203,     2,     8,   178,   549,   344,   384,   985,
     621,   179,   668,    87,   875,   334,   866,   195,   784,   177,
      86,   513,   514,   515,   516,   204,    10,   524,   450,    88,
      89,    90,   600,   335,    91,    92,   256,   257,  1016,   336,
     769,    33,   213,   337,   524,   550,   451,    93,   178,   287,
      94,   288,   452,   665,   179,   345,   453,  1007,   186,   371,
     177,   177,   558,   432,   434,   436,   438,   440,   441,   442,
     372,   373,   102,   785,    95,   568,   189,   732,   346,   669,
     986,   669,   758,   339,   867,    97,   362,   525,   818,   786,
     298,   394,   601,   178,    99,    34,   455,   100,   101,   179,
     770,    68,   913,    73,   400,   102,   805,   177,   113,    81,
     647,    70,   356,    79,   306,   104,   578,    56,    71,   491,
     135,   113,   113,   609,   610,   443,   309,    57,   104,   104,
     302,   115,   371,   776,   110,   777,   310,   779,    47,    48,
     611,   676,   401,   819,   115,   115,   368,   110,   110,   806,
     395,    64,   368,   357,    69,   306,    74,   557,  -269,   368,
    -269,   625,    79,   498,   492,    47,   643,    58,   178,    74,
     427,   428,   429,   430,   179,   626,   312,   212,   213,   505,
     313,   789,   178,   499,   639,    12,    85,   178,   179,   314,
     315,   307,   332,   179,    52,   187,   177,   302,    13,   694,
      79,   829,   193,   830,   266,   832,   328,   329,   781,   213,
     448,   359,    53,   506,  -269,   586,  -269,   587,   330,   588,
     627,   589,   640,   590,   695,   764,   914,    66,   767,   448,
     178,   685,   212,   309,   532,   696,   179,   552,   555,   331,
      38,   267,   796,   310,   234,   213,    61,   268,   269,   270,
     271,   272,   273,   797,    85,   178,    62,   765,   196,   275,
      65,   179,    95,   652,   213,   654,   276,   277,   203,   177,
     738,   739,   534,    97,   150,   178,    85,   734,   798,   693,
     274,   179,    85,   312,   737,   865,   297,   313,   325,   326,
     278,   542,   794,   102,   744,   873,   314,   315,    43,   802,
     327,   697,   681,   853,   334,   102,   691,   890,   368,   342,
     740,   741,   742,   743,   127,   854,   445,   448,   533,   617,
     618,   450,   335,   893,   894,    34,   896,   659,   336,   661,
      72,   619,   337,   605,   903,   102,  -344,   102,   905,   451,
     178,   178,    78,   701,   703,   452,   179,   179,   706,   453,
     295,   296,   620,   628,   915,   177,    85,  -344,   397,  -344,
    -344,   338,   561,   629,   404,   405,  -344,  -344,  -344,  -344,
      75,    47,   339,   309,   354,   855,   856,   651,   454,   653,
     744,    76,   534,   310,    82,   178,   178,   178,   178,   455,
     347,   179,   179,   179,   179,    83,   392,   828,    79,   177,
     177,   177,   177,   727,    79,   729,   178,   950,  -143,  -143,
     458,   311,   179,   885,   952,   614,   615,   888,  -143,  -143,
    -143,   614,   615,   312,   957,   958,   735,   313,   487,   178,
    -143,   858,   321,   795,    44,   179,   314,   315,   569,   707,
     131,  -143,   859,   177,   718,   719,    79,   178,   666,   614,
     615,   113,  1015,   179,   519,   322,   521,   323,   104,   523,
     324,   177,   984,   576,   614,   615,   879,   363,   364,   614,
     615,    79,   178,   616,   115,   469,   156,   110,   179,   368,
     835,    49,   178,   170,   171,   172,   173,   470,   179,   938,
     939,   132,    59,   478,   368,   368,   202,   479,   206,   368,
     208,   209,   577,   480,   608,   755,   215,   518,   699,    60,
      79,   463,   549,   178,   448,    30,   549,   766,   464,   179,
      31,    79,   531,    80,   666,   178,   545,   177,   860,   545,
      47,   179,   963,   546,   964,    63,   559,    35,   178,   177,
     178,    36,   178,   481,   179,   545,   179,   482,   179,   259,
     262,    67,   564,   972,   178,   787,   483,   484,   485,   592,
     179,   808,   126,  -316,   592,  -316,   593,  -349,   218,   487,
     595,   622,   597,   124,   128,   599,   574,   575,   125,   545,
     666,   814,   666,   129,   666,   824,   698,   462,  -349,   178,
    -349,  -349,   305,   212,   221,   179,   534,  -349,  -349,  -349,
    -349,   210,   592,   177,   545,   287,   178,   288,   178,   708,
     178,   809,   179,   130,   179,   222,   179,   223,   224,   478,
     225,   545,   189,   594,   545,   213,   924,   379,   810,   480,
     382,   811,   385,   925,   463,   382,   388,   389,   390,   391,
      84,   961,   414,   133,   342,   355,   134,   415,   666,   138,
     666,   178,   666,   139,   279,   280,   281,   179,   178,   219,
     238,   220,   881,   882,   179,   177,   282,   283,   178,   481,
     267,   198,   188,   482,   179,   199,   268,   269,   270,   271,
     272,   273,   483,   484,   485,   562,   244,   284,   562,   771,
     563,   656,   285,   566,   772,   191,   178,   178,   194,   178,
     534,   612,   179,   179,   673,   179,   613,   178,   200,   274,
     534,   178,   478,   179,   352,  1004,   834,   179,   353,  1005,
     458,   265,   480,   228,   229,   178,   504,   178,  -314,   216,
    -314,   179,   264,   179,   223,   224,   217,   225,   534,   534,
     265,   534,   228,   547,   710,   711,   712,   713,   930,   534,
     247,   504,   248,   534,   239,   249,   722,   723,   536,   537,
     538,   937,   481,    79,   242,   501,   482,   448,   250,   534,
      79,    79,   526,   602,   251,   483,   484,   485,   252,   333,
     178,   253,   947,   948,   240,   348,   179,   178,   170,   171,
     172,   173,   294,   179,  -315,   241,  -315,   178,   178,    79,
     243,   633,   245,   179,   179,   775,   246,   478,   756,   757,
      79,   836,   658,   218,   967,   762,   763,   480,   286,   584,
     259,    79,   534,   660,    79,   791,   662,   973,   290,   534,
     976,   293,   299,   402,   403,   178,   320,   991,   992,   534,
     534,   179,    79,   343,   726,   178,   178,    79,    79,   728,
     730,   179,   179,   350,   487,   995,   374,   481,   375,   177,
     177,   482,   820,   821,   487,  1001,   800,   378,   801,   680,
     483,   484,   485,   342,   800,   380,   960,   534,   386,  1008,
     396,   398,  1010,  1011,   444,  1012,   459,   399,   409,   411,
     822,   823,   460,   417,   267,   418,   461,   466,   476,  1021,
     268,   269,   270,   271,   272,   273,   478,   488,   649,   489,
     904,   490,   494,   495,   497,   496,   480,   507,   508,   837,
     838,   520,   862,   517,    87,   842,   843,   522,   560,   529,
     561,   567,   551,   274,   850,   851,   565,   591,   530,   596,
      88,    89,    90,   598,   603,    91,    92,   877,   487,   606,
     636,   650,   623,   689,   690,   624,   481,   630,    93,   631,
     482,    94,   632,   892,   637,   657,   895,   638,   704,   483,
     484,   485,   267,   644,   902,   671,   672,   674,   268,   269,
     270,   271,   272,   273,   675,    95,   677,   692,   687,   570,
     571,   572,   573,    96,   709,   714,    97,    98,   715,   720,
     582,   287,   583,   733,   736,    99,   753,   747,   100,   101,
     748,   274,   749,   750,   909,   910,   102,   759,   773,   768,
      14,    15,   935,    16,   774,   778,   780,   579,   783,   782,
     790,   812,    14,    15,   788,    16,   813,   815,   816,    17,
     817,    85,   825,   826,   833,    18,   827,   949,    19,    20,
     580,    17,    22,   846,   831,   845,   839,    18,   848,   847,
      19,    20,    21,   849,    22,   419,   420,   421,   422,   423,
     424,   425,   426,   861,   161,   864,   669,   868,   870,   634,
     635,   871,   969,   872,   792,   793,   641,   642,   162,   163,
     874,   974,   975,    23,   977,   883,   889,   891,   164,   982,
     899,   165,    24,    25,   906,    23,    26,   900,   901,   908,
     907,   911,   912,   916,    24,    25,   166,   167,    26,   917,
     918,   923,   932,   168,   926,   933,   169,   878,   934,  1002,
     927,   360,   936,   943,   170,   171,   172,   173,   174,   941,
     267,   944,   175,   946,   951,   176,   268,   269,   270,   271,
     272,   273,   267,   953,   226,   954,   235,   102,   268,   269,
     270,   271,   272,   273,   955,   959,   968,   716,   717,   965,
     162,   163,   971,   504,   504,   227,   978,   228,   229,   274,
     164,   979,   981,   165,   230,   231,   232,   233,   869,   988,
     989,   274,   983,   993,   996,   999,  1003,  1009,   166,   167,
    1013,  1017,   678,  1018,  1019,   168,   679,  1020,   169,   886,
     887,  1022,  1023,   408,   745,   746,   170,   171,   172,   173,
     174,   751,   752,  -349,   236,   412,   267,   176,   725,   406,
     760,   761,   268,   269,   270,   271,   272,   273,   358,   102,
     407,   987,   844,   663,  -349,   211,  -349,  -349,   410,   212,
     731,   922,   267,  -349,  -349,  -349,  -349,   607,   268,   269,
     270,   271,   272,   273,   140,   274,   807,   688,   880,   884,
     931,   585,     0,     0,     0,   141,   142,   381,     0,     0,
       0,   213,     0,   143,   258,   144,     0,     0,   145,     0,
       0,   274,     0,   146,   147,   148,   149,   150,     0,     0,
       0,   166,   167,     0,     0,     0,     0,     0,     0,   151,
       0,   169,     0,     0,   140,     0,   102,     0,     0,   170,
     171,   172,   173,   174,     0,   141,   142,     0,     0,     0,
     504,     0,   504,   143,     0,   144,   365,     0,   145,   140,
     840,   841,   102,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   376,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   383,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   556,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   700,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,   702,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   144,   705,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     144,     0,     0,   145,   140,     0,   102,     0,   146,   147,
     148,   149,   150,     0,     0,   141,   142,     0,     0,     0,
       0,     0,     0,   143,   151,   502,     0,     0,   145,   140,
       0,   102,     0,   146,   147,   148,   149,   150,     0,     0,
     141,   142,     0,     0,     0,     0,     0,     0,   143,   151,
     502,     0,     0,   145,     0,     0,   102,     0,   146,   147,
     148,   149,   646,     0,     0,   -50,   -50,     0,     0,   302,
     140,     0,     0,     0,   151,   -50,   -50,   -50,     0,     0,
     303,   102,   431,   304,     0,   305,     0,   -50,     0,   143,
       0,   144,     0,     0,   145,   140,     0,     0,   -50,   146,
     147,   148,   149,   150,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   213,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,   143,
     151,   144,     0,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,     0,   437,     0,     0,
       0,     0,     0,     0,   143,   151,   144,     0,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,     0,   439,     0,     0,     0,     0,   140,     0,   143,
     151,   144,     0,     0,   145,     0,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   143,     0,   144,     0,
       0,   145,     0,     0,     0,   151,   146,   147,   148,   149,
     150,     0,   102,     0,     0,     0,     0,   267,     0,     0,
       0,     0,   151,   268,   269,   270,   271,   272,   273,   102,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
     361,   268,   269,   270,   271,   272,   273,     0,   467,   468,
       0,     0,     0,     0,   267,     0,   274,     0,   469,   413,
     268,   269,   270,   271,   272,   273,   170,   171,   172,   173,
     470,     0,   267,     0,   274,     0,     0,   416,   268,   269,
     270,   271,   272,   273,     0,     0,     0,     0,   267,     0,
     548,     0,     0,   274,   268,   269,   270,   271,   272,   273,
       0,     0,     0,     0,     0,     0,     0,   267,     0,     0,
       0,   274,   803,   268,   269,   270,   271,   272,   273,     0,
       0,     0,   267,     0,     0,   804,     0,   274,   268,   269,
     270,   271,   272,   273,  -342,     0,     0,     0,     0,     0,
       0,   267,     0,   940,     0,     0,   274,   268,   269,   270,
     271,   272,   273,     0,     0,  -342,     0,  -342,  -342,     0,
     565,   274,     0,     0,  -342,  -342,  -342,  -342,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   345,    89,   509,    54,    71,    93,   197,
      80,   289,    51,    39,    99,    71,    80,   460,   356,    51,
     213,   164,   360,   592,    80,   304,   305,    45,   543,   140,
     312,   325,   326,     6,   145,    51,     2,   307,    51,   302,
       5,   923,     5,   962,    48,     5,     4,   125,   327,    54,
      55,   218,    91,    11,    76,   125,    58,     5,   225,     5,
      65,   125,     9,     3,     9,     8,     8,     5,     5,   125,
      30,   328,   329,   330,   331,    91,    76,     5,     8,    19,
      20,    21,     5,    26,    24,    25,   141,   142,  1007,    32,
       5,     5,    96,    36,     5,    97,    26,    37,   168,    62,
      40,    64,    32,    76,   168,    53,    36,   989,    74,   302,
     166,   167,   375,   279,   280,   281,   282,   283,   284,   285,
      85,    86,    95,    60,    64,   395,    64,   602,    76,    76,
      76,    76,   636,    76,    76,    75,    76,    65,     5,    76,
     168,     5,    65,   213,    84,    59,    76,    87,    88,   213,
      65,     5,    63,     5,    38,    95,    17,   213,   197,     5,
     502,    53,    17,    58,    28,   197,     5,    26,    60,    19,
      65,   210,   211,   467,   468,   286,    26,    36,   210,   211,
      48,   197,   375,   658,   197,   660,    36,   662,    18,    19,
     469,    59,    76,    60,   210,   211,   212,   210,   211,    60,
      64,    31,   218,    58,    58,    28,    58,   374,    58,   225,
      60,     5,    58,     5,    64,    45,   498,    76,   288,    58,
     275,   276,   277,   278,   288,    19,    76,    64,    96,    28,
      80,   674,   302,    25,    38,     0,     5,   307,   302,    89,
      90,    64,    50,   307,    76,    75,   302,    48,    59,     5,
      58,   726,    82,   728,     5,   730,    44,    45,    59,    96,
     288,    30,    94,    62,    58,   431,    60,   433,    56,   435,
      64,   437,    76,   439,    30,    28,   845,    12,    60,   307,
     350,   796,    64,    26,   349,    41,   350,   372,   373,    77,
      76,    42,     5,    36,   124,    96,    26,    48,    49,    50,
      51,    52,    53,    16,     5,   375,    36,    60,    84,    47,
       5,   375,    64,   506,    96,   508,    54,    55,   357,   375,
     614,   615,   350,    75,    76,   395,     5,   605,    41,    30,
      81,   395,     5,    76,   613,   778,    76,    80,    54,    55,
      78,   357,   680,    95,   622,   788,    89,    90,    76,   687,
      66,    30,   540,    64,     8,    95,   549,    30,   374,   189,
     617,   618,   619,   620,   412,    76,    76,   395,    76,    44,
      45,     8,    26,   816,   817,    59,   819,   520,    32,   522,
     406,    56,    36,   461,   827,    95,    38,    95,   831,    26,
     460,   461,   410,   560,   561,    32,   460,   461,   565,    36,
     166,   167,    77,    26,   847,   461,     5,    59,   238,    61,
      62,    65,    64,    36,   244,   245,    68,    69,    70,    71,
      48,   251,    76,    26,    23,   767,   768,   505,    65,   507,
     708,    59,   460,    36,    48,   505,   506,   507,   508,    76,
      50,   505,   506,   507,   508,    59,    50,   725,    58,   505,
     506,   507,   508,   596,    58,   598,   526,   900,    44,    45,
     290,    64,   526,   801,   907,    54,    55,   805,    54,    55,
      56,    54,    55,    76,   917,   918,    65,    80,   308,   549,
      66,    65,     6,    66,    76,   549,    89,    90,    50,   567,
      59,    77,    76,   549,   579,   580,    58,   567,   526,    54,
      55,   540,  1006,   567,   334,    29,   336,    31,   540,   339,
      34,   567,   955,    50,    54,    55,   795,   210,   211,    54,
      55,    58,   592,    63,   540,    64,    70,   540,   592,   545,
      65,    76,   602,    72,    73,    74,    75,    76,   602,   877,
     878,    59,    76,    26,   560,   561,    90,    30,    92,   565,
      94,    95,    50,    36,    50,   633,   100,   333,    50,    76,
      58,    58,    58,   633,   592,    59,    58,   645,    65,   633,
      64,    58,   348,    60,   602,   645,    58,   633,   771,    58,
     410,   645,   924,    65,   926,    76,    65,    72,   658,   645,
     660,    76,   662,    76,   658,    58,   660,    80,   662,   143,
     144,    76,    65,   941,   674,   670,    89,    90,    91,    58,
     674,   696,    76,    59,    58,    61,    65,    38,    64,   449,
     450,    65,   452,    48,    76,   455,   402,   403,    53,    58,
     658,   709,   660,    76,   662,   720,    65,    59,    59,   709,
      61,    62,    64,    64,    38,   709,   674,    68,    69,    70,
      71,    60,    58,   709,    58,    62,   726,    64,   728,    65,
     730,    65,   726,    76,   728,    59,   730,    61,    62,    26,
      64,    58,    64,    30,    58,    96,    58,   221,    65,    36,
     224,    65,   226,    65,    58,   229,   230,   231,   232,   233,
      49,    65,    58,    76,   524,    17,    76,    63,   726,    72,
     728,   771,   730,    76,    44,    45,    46,   771,   778,    59,
      48,    61,   797,   798,   778,   771,    56,    57,   788,    76,
      42,    72,    76,    80,   788,    76,    48,    49,    50,    51,
      52,    53,    89,    90,    91,    58,    48,    77,    58,    58,
      63,   517,    82,    63,    63,    76,   816,   817,    76,   819,
     778,    59,   816,   817,   530,   819,    64,   827,    76,    81,
     788,   831,    26,   827,    38,    72,    30,   831,    42,    76,
     600,    59,    36,    61,    62,   845,   320,   847,    59,    76,
      61,   845,    59,   847,    61,    62,    76,    64,   816,   817,
      59,   819,    61,    62,   570,   571,   572,   573,   863,   827,
       8,   345,    10,   831,    59,    13,   582,   583,   352,   353,
     354,   876,    76,    58,    76,    60,    80,   845,    26,   847,
      58,    58,    60,    60,    32,    89,    90,    91,    36,   188,
     900,    39,   897,   898,    59,   194,   900,   907,    72,    73,
      74,    75,    76,   907,    59,    59,    61,   917,   918,    58,
      76,    60,    48,   917,   918,   657,    59,    26,   634,   635,
      58,    30,    60,    64,   929,   641,   642,    36,    79,   413,
     414,    58,   900,    60,    58,   677,    60,   942,    64,   907,
     945,    76,    76,   242,   243,   955,    53,   965,   966,   917,
     918,   955,    58,     5,    60,   965,   966,    58,    58,    60,
      60,   965,   966,    60,   734,   970,    64,    76,    48,   965,
     966,    80,   714,   715,   744,   980,    58,    76,    60,    17,
      89,    90,    91,   753,    58,    76,    60,   955,    76,   994,
       5,    76,   997,   998,    75,  1000,    28,    76,    76,    76,
     716,   717,    60,    76,    42,    76,    19,    76,    76,  1014,
      48,    49,    50,    51,    52,    53,    26,    76,   502,    76,
      30,    76,    76,    76,    30,    76,    36,    28,    28,   745,
     746,    76,   774,   332,     3,   751,   752,    76,    64,    76,
      64,    53,    92,    81,   760,   761,    64,    63,   347,    76,
      19,    20,    21,    76,    76,    24,    25,    17,   828,    76,
      65,     5,    76,   547,   548,    76,    76,    76,    37,    76,
      80,    40,    76,   815,    76,     5,   818,    76,   562,    89,
      90,    91,    42,    76,   826,     5,     5,    60,    48,    49,
      50,    51,    52,    53,     5,    64,     5,    76,    41,   398,
     399,   400,   401,    72,    53,     5,    75,    76,     5,    15,
     409,    62,   411,    28,    76,    84,    62,    76,    87,    88,
      76,    81,    76,    76,   840,   841,    95,    65,    30,    58,
       7,     8,   874,    10,     5,    60,    28,    14,     5,    53,
      76,     5,     7,     8,    60,    10,     5,     5,    60,    26,
      60,     5,    30,     5,    76,    32,    60,   899,    35,    36,
      37,    26,    39,     5,    60,    60,    65,    32,    30,    60,
      35,    36,    37,    19,    39,   267,   268,   269,   270,   271,
     272,   273,   274,    28,    12,    53,    76,    76,    75,   488,
     489,    75,   934,     5,   678,   679,   495,   496,    26,    27,
       5,   943,   944,    80,   946,    30,    76,     5,    36,   951,
       5,    39,    89,    90,     5,    80,    93,    60,    59,    19,
      60,     5,     5,    76,    89,    90,    54,    55,    93,    60,
      60,    28,     5,    61,    60,     5,    64,    17,     5,   981,
      63,    33,     5,     5,    72,    73,    74,    75,    76,    17,
      42,     5,    80,     5,     5,    83,    48,    49,    50,    51,
      52,    53,    42,    76,    38,     5,    12,    95,    48,    49,
      50,    51,    52,    53,    60,     5,     5,   576,   577,    28,
      26,    27,     5,   767,   768,    59,     5,    61,    62,    81,
      36,     5,     5,    39,    68,    69,    70,    71,   782,    64,
      28,    81,     5,     5,     5,     5,     5,     5,    54,    55,
       5,     5,    18,     5,     5,    61,    22,     5,    64,   803,
     804,    65,     5,   249,   623,   624,    72,    73,    74,    75,
      76,   630,   631,    38,    80,   253,    42,    83,   593,   247,
     639,   640,    48,    49,    50,    51,    52,    53,    28,    95,
     248,   959,   753,   524,    59,    60,    61,    62,   251,    64,
     600,   853,    42,    68,    69,    70,    71,   463,    48,    49,
      50,    51,    52,    53,    43,    81,   694,   545,   796,   800,
     864,   414,    -1,    -1,    -1,    54,    55,   224,    -1,    -1,
      -1,    96,    -1,    62,    63,    64,    -1,    -1,    67,    -1,
      -1,    81,    -1,    72,    73,    74,    75,    76,    -1,    -1,
      -1,    54,    55,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    64,    -1,    -1,    43,    -1,    95,    -1,    -1,    72,
      73,    74,    75,    76,    -1,    54,    55,    -1,    -1,    -1,
     924,    -1,   926,    62,    -1,    64,    65,    -1,    67,    43,
     749,   750,    95,    72,    73,    74,    75,    76,    -1,    -1,
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
      -1,    -1,    55,    -1,    -1,    -1,    -1,    43,    -1,    62,
      88,    64,    -1,    -1,    67,    -1,    -1,    95,    -1,    72,
      73,    74,    75,    76,    -1,    -1,    62,    -1,    64,    -1,
      -1,    67,    -1,    -1,    -1,    88,    72,    73,    74,    75,
      76,    -1,    95,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    88,    48,    49,    50,    51,    52,    53,    95,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      65,    48,    49,    50,    51,    52,    53,    -1,    54,    55,
      -1,    -1,    -1,    -1,    42,    -1,    81,    -1,    64,    66,
      48,    49,    50,    51,    52,    53,    72,    73,    74,    75,
      76,    -1,    42,    -1,    81,    -1,    -1,    65,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    42,    -1,
      60,    -1,    -1,    81,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    81,    66,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    42,    -1,    -1,    60,    -1,    81,    48,    49,
      50,    51,    52,    53,    38,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    63,    -1,    -1,    81,    48,    49,    50,
      51,    52,    53,    -1,    -1,    59,    -1,    61,    62,    -1,
      64,    81,    -1,    -1,    68,    69,    70,    71,    -1,    -1,
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
      28,    59,    53,     5,     5,    60,    76,   103,    60,   133,
      76,   179,   222,   222,   190,    66,     5,    16,    41,   212,
      58,    60,   190,    66,    60,    17,    60,   196,   187,    65,
      65,    65,     5,     5,   129,     5,    60,    60,     5,    60,
     179,   179,   175,   175,   187,    30,     5,    60,   135,   178,
     178,    60,   178,    76,    30,    65,    30,   175,   175,    65,
     174,   174,   175,   175,   176,    60,     5,    60,    30,    19,
     175,   175,   151,    64,    76,   114,   114,   115,    65,    76,
     140,    28,   179,   165,    53,   133,     8,    76,    76,   222,
      75,    75,     5,   133,     5,     9,   159,    17,    17,   143,
     214,   187,   187,    30,   216,   190,   222,   222,   190,    76,
      30,     5,   179,   133,   133,   179,   133,   167,   168,     5,
      60,    59,   179,   133,    30,   133,     5,    60,    19,   175,
     175,     5,     5,    63,   125,   133,    76,    60,    60,   153,
     154,   215,   183,    28,    58,    65,    60,    63,   130,   163,
     103,   222,     5,     5,     5,   179,     5,   103,   190,   190,
      63,    17,   169,     5,     5,   172,     5,   103,   103,   179,
     133,     5,   133,    76,     5,    60,   138,   133,   133,     5,
      60,    65,   151,   114,   114,    28,   131,   103,     5,   179,
     166,     5,   190,   103,   179,   179,   103,   179,     5,     5,
     161,     5,   179,     5,   133,     5,    76,   154,    64,    28,
     153,   129,   129,     5,   164,   103,     5,   173,   171,     5,
     170,   103,   179,     5,    72,    76,   150,   151,   103,     5,
     103,   103,   103,     5,   162,   146,   153,     5,     5,     5,
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
     179,   179,   179,   179,   180,   180,   180,   181,   181,   182,
     182,   182,   182,   182,   182,   183,   183,   184,   185,   185,
     186,   188,   187,   189,   187,   191,   190,   192,   192,   192,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   194,   194,   194,   194,   195,
     195,   195,   196,   196,   197,   197,   198,   198,   198,   199,
     199,   199,   199,   199,   200,   200,   200,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   201,   201,   201,   202,
     202,   203,   203,   203,   203,   204,   204,   204,   204,   204,
     204,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     206,   206,   207,   208,   208,   209,   209,   211,   210,   212,
     212,   212,   213,   213,   214,   214,   215,   215,   216,   216,
     217,   218,   219,   219,   219,   220,   221,   221,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   223,   223,   223,
     223,   223,   223,   223,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   225,   225,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   227,   227,   228,   228
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
       0,     3,     5,     5,     3,     2,     0,     3,     1,     3,
       4,     4,     4,     3,     5,     3,     1,     1,     3,     1,
       3,     0,     4,     0,     2,     0,     2,     1,     3,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     5,     5,     7,     5,     1,
       3,     2,     6,     4,     1,     2,     2,     4,     5,     1,
       1,     3,     3,     1,     1,     3,     1,     4,     4,     3,
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
#line 239 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2358 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 240 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2364 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 243 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2370 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 244 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2376 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 247 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2382 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 248 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2388 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 251 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2394 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 252 "pascal.y"
     { (yyval.node) = NULL; }
#line 2400 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 255 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2406 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 258 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2412 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 259 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2418 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 262 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2424 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 263 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2430 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 264 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2436 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 265 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2442 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 266 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2448 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 267 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2454 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 268 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2460 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 271 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2466 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 274 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2472 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 275 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2478 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 278 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2484 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 279 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2490 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 280 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2496 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 281 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2502 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 282 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2508 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 283 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2514 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 286 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2520 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 287 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2526 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 288 "pascal.y"
     { (yyval.node) = NULL; }
#line 2532 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 291 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2538 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 292 "pascal.y"
     { (yyval.node) = NULL; }
#line 2544 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 295 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2550 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 296 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2556 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 297 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2562 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 298 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2568 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 301 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2574 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 302 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2580 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 305 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2586 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 306 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2592 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 309 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2598 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 310 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2604 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 311 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2610 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 312 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2616 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 315 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2622 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 316 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2628 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 319 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2634 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 320 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2640 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 323 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2646 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 324 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2652 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 325 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2658 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 326 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2664 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 327 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2670 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 330 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2676 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 331 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2682 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 334 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2688 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 335 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2694 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 336 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2700 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 337 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2706 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 338 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2712 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 339 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2718 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 342 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2724 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 343 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2730 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2736 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 347 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2742 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 350 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2748 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2754 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2760 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2766 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2772 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 361 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2778 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 362 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2784 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 365 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2790 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 368 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2796 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 369 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2802 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 370 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2808 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 373 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2814 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 374 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2820 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 377 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2826 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 378 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2832 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 379 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2838 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 380 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2844 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 381 "pascal.y"
                                                       { }
#line 2850 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 381 "pascal.y"
                                                           { }
#line 2856 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 381 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2862 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 382 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2868 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 383 "pascal.y"
                           { pascal_dir_arm(); (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2874 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 384 "pascal.y"
                                      { pascal_dir_arm(); if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2880 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 385 "pascal.y"
                                            { pascal_dir_arm(); pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2886 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 386 "pascal.y"
                                                       { pascal_dir_arm(); if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2892 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 387 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2898 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 388 "pascal.y"
                         { }
#line 2904 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 388 "pascal.y"
                                               { (yyval.node) = pt_record_or_helper((yyvsp[-4].node), (yyvsp[-1].node)); }
#line 2910 "pascal.tab.c"
    break;

  case 95: /* type: TYPESY IDENT FORSY type class_members ENDSY  */
#line 389 "pascal.y"
                                                 { if (!pt_is_helper_word((yyvsp[-4].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_type_helper((yyvsp[-2].node), (yyvsp[-1].node)); }
#line 2916 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt SETSY OFSY simple_type  */
#line 390 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2922 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY  */
#line 391 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2928 "pascal.tab.c"
    break;

  case 98: /* type: packed_opt FILESY OFSY type  */
#line 392 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2934 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY record_body ENDSY  */
#line 393 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2940 "pascal.tab.c"
    break;

  case 100: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 394 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2946 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY class_members ENDSY  */
#line 395 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2952 "pascal.tab.c"
    break;

  case 102: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 396 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2958 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 397 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2964 "pascal.tab.c"
    break;

  case 104: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 398 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2970 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT  */
#line 401 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2976 "pascal.tab.c"
    break;

  case 106: /* result_type: IDENT PERIOD IDENT  */
#line 402 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2982 "pascal.tab.c"
    break;

  case 107: /* result_type: spec_ref  */
#line 403 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2988 "pascal.tab.c"
    break;

  case 108: /* guid_opt: %empty  */
#line 406 "pascal.y"
    { (yyval.node) = NULL; }
#line 2994 "pascal.tab.c"
    break;

  case 109: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 407 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 3000 "pascal.tab.c"
    break;

  case 110: /* class_members: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3006 "pascal.tab.c"
    break;

  case 111: /* class_members: class_members class_member  */
#line 411 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3012 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT  */
#line 414 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 3018 "pascal.tab.c"
    break;

  case 113: /* class_member: IDENT SEMICOLON  */
#line 415 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 3024 "pascal.tab.c"
    break;

  case 114: /* class_member: IDENT FORSY IDENT  */
#line 416 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3030 "pascal.tab.c"
    break;

  case 115: /* class_member: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 417 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3036 "pascal.tab.c"
    break;

  case 116: /* class_member: id_list COLON type SEMICOLON  */
#line 418 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3042 "pascal.tab.c"
    break;

  case 117: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 419 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3048 "pascal.tab.c"
    break;

  case 118: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 420 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3054 "pascal.tab.c"
    break;

  case 119: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 421 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 3060 "pascal.tab.c"
    break;

  case 120: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 422 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3066 "pascal.tab.c"
    break;

  case 121: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 423 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3072 "pascal.tab.c"
    break;

  case 122: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 424 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3078 "pascal.tab.c"
    break;

  case 123: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 425 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3084 "pascal.tab.c"
    break;

  case 124: /* prop_index_opt: %empty  */
#line 428 "pascal.y"
    { (yyval.node) = NULL; }
#line 3090 "pascal.tab.c"
    break;

  case 125: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 429 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3096 "pascal.tab.c"
    break;

  case 126: /* prop_specs: %empty  */
#line 432 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3102 "pascal.tab.c"
    break;

  case 127: /* prop_specs: prop_specs IDENT IDENT  */
#line 433 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3108 "pascal.tab.c"
    break;

  case 128: /* prop_specs: prop_specs IDENT INTCONST  */
#line 434 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3114 "pascal.tab.c"
    break;

  case 129: /* packed_opt: PACKEDSY  */
#line 437 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3120 "pascal.tab.c"
    break;

  case 130: /* packed_opt: %empty  */
#line 438 "pascal.y"
     { (yyval.node) = NULL; }
#line 3126 "pascal.tab.c"
    break;

  case 131: /* simple_type: LPARENT enum_list RPARENT  */
#line 441 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3132 "pascal.tab.c"
    break;

  case 132: /* simple_type: IDENT  */
#line 442 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3138 "pascal.tab.c"
    break;

  case 133: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 443 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3144 "pascal.tab.c"
    break;

  case 134: /* simple_type: IDENT PERIOD IDENT  */
#line 444 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3150 "pascal.tab.c"
    break;

  case 135: /* simple_type: spec_ref  */
#line 445 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3156 "pascal.tab.c"
    break;

  case 136: /* simple_type: lo_bound DOTDOT b_expr  */
#line 446 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3162 "pascal.tab.c"
    break;

  case 137: /* lo_bound: lo_term  */
#line 449 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3168 "pascal.tab.c"
    break;

  case 138: /* lo_bound: PLUS lo_term  */
#line 450 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3174 "pascal.tab.c"
    break;

  case 139: /* lo_bound: MINUS lo_term  */
#line 451 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3180 "pascal.tab.c"
    break;

  case 140: /* lo_bound: lo_bound PLUS b_term  */
#line 452 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3186 "pascal.tab.c"
    break;

  case 141: /* lo_bound: lo_bound MINUS b_term  */
#line 453 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3192 "pascal.tab.c"
    break;

  case 142: /* lo_term: scalar_constant  */
#line 456 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3198 "pascal.tab.c"
    break;

  case 143: /* lo_term: IDENT PERIOD IDENT  */
#line 457 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3204 "pascal.tab.c"
    break;

  case 144: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 458 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3210 "pascal.tab.c"
    break;

  case 145: /* lo_term: lo_term MUL b_factor  */
#line 459 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3216 "pascal.tab.c"
    break;

  case 146: /* lo_term: lo_term IDIV b_factor  */
#line 460 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3222 "pascal.tab.c"
    break;

  case 147: /* lo_term: lo_term IMOD b_factor  */
#line 461 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3228 "pascal.tab.c"
    break;

  case 148: /* lo_term: lo_term SHIFTOP b_factor  */
#line 462 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3234 "pascal.tab.c"
    break;

  case 149: /* b_expr: b_term  */
#line 465 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3240 "pascal.tab.c"
    break;

  case 150: /* b_expr: PLUS b_term  */
#line 466 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3246 "pascal.tab.c"
    break;

  case 151: /* b_expr: MINUS b_term  */
#line 467 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3252 "pascal.tab.c"
    break;

  case 152: /* b_expr: b_expr PLUS b_term  */
#line 468 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3258 "pascal.tab.c"
    break;

  case 153: /* b_expr: b_expr MINUS b_term  */
#line 469 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3264 "pascal.tab.c"
    break;

  case 154: /* b_term: b_factor  */
#line 472 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3270 "pascal.tab.c"
    break;

  case 155: /* b_term: b_term MUL b_factor  */
#line 473 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3276 "pascal.tab.c"
    break;

  case 156: /* b_term: b_term IDIV b_factor  */
#line 474 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3282 "pascal.tab.c"
    break;

  case 157: /* b_term: b_term IMOD b_factor  */
#line 475 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3288 "pascal.tab.c"
    break;

  case 158: /* b_term: b_term SHIFTOP b_factor  */
#line 476 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3294 "pascal.tab.c"
    break;

  case 159: /* b_factor: scalar_constant  */
#line 479 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3300 "pascal.tab.c"
    break;

  case 160: /* b_factor: IDENT PERIOD IDENT  */
#line 480 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3306 "pascal.tab.c"
    break;

  case 161: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 481 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3312 "pascal.tab.c"
    break;

  case 162: /* b_factor: LPARENT b_expr RPARENT  */
#line 482 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3318 "pascal.tab.c"
    break;

  case 163: /* record_body: record_field_list record_case_opt  */
#line 485 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3324 "pascal.tab.c"
    break;

  case 164: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 488 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3330 "pascal.tab.c"
    break;

  case 165: /* record_field_list: record_field  */
#line 489 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3336 "pascal.tab.c"
    break;

  case 166: /* $@4: %empty  */
#line 492 "pascal.y"
                  { }
#line 3342 "pascal.tab.c"
    break;

  case 167: /* record_field: id_list COLON $@4 type  */
#line 492 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3348 "pascal.tab.c"
    break;

  case 168: /* record_field: IDENT FORSY IDENT  */
#line 493 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3354 "pascal.tab.c"
    break;

  case 169: /* record_field: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 494 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3360 "pascal.tab.c"
    break;

  case 170: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 495 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3366 "pascal.tab.c"
    break;

  case 171: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 496 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3372 "pascal.tab.c"
    break;

  case 172: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 497 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3378 "pascal.tab.c"
    break;

  case 173: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 498 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3384 "pascal.tab.c"
    break;

  case 174: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 499 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3390 "pascal.tab.c"
    break;

  case 175: /* record_field: IDENT record_field  */
#line 500 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3396 "pascal.tab.c"
    break;

  case 176: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 501 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3402 "pascal.tab.c"
    break;

  case 177: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 502 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3408 "pascal.tab.c"
    break;

  case 178: /* record_field: %empty  */
#line 503 "pascal.y"
     { (yyval.node) = NULL; }
#line 3414 "pascal.tab.c"
    break;

  case 179: /* case_arm_mark: %empty  */
#line 506 "pascal.y"
    { }
#line 3420 "pascal.tab.c"
    break;

  case 180: /* case_open: %empty  */
#line 509 "pascal.y"
    { }
#line 3426 "pascal.tab.c"
    break;

  case 181: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 512 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3432 "pascal.tab.c"
    break;

  case 182: /* record_case_opt: CASESY IDENT COLON LPARENT enum_list RPARENT OFSY case_open record_case_list  */
#line 513 "pascal.y"
                                                                                  { tree_t *_e = pt_retag((yyvsp[-4].node), TT_ENUM_TYPE); for (int _i = 0; _e && _i < _e->n; _i++) cenv_int(_e->c[_i]->v.sval, _i); (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-7].node)), _e), (yyvsp[0].node)); }
#line 3438 "pascal.tab.c"
    break;

  case 183: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 514 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3444 "pascal.tab.c"
    break;

  case 184: /* record_case_opt: %empty  */
#line 515 "pascal.y"
     { (yyval.node) = NULL; }
#line 3450 "pascal.tab.c"
    break;

  case 185: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 518 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3456 "pascal.tab.c"
    break;

  case 186: /* record_case_list: record_case_arm  */
#line 519 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3462 "pascal.tab.c"
    break;

  case 187: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 522 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3468 "pascal.tab.c"
    break;

  case 188: /* record_case_arm: %empty  */
#line 523 "pascal.y"
     { (yyval.node) = NULL; }
#line 3474 "pascal.tab.c"
    break;

  case 189: /* var_decl_list: var_decl_list var_decl  */
#line 526 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3480 "pascal.tab.c"
    break;

  case 190: /* var_decl_list: var_decl  */
#line 527 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3486 "pascal.tab.c"
    break;

  case 191: /* var_decl: id_list COLON type SEMICOLON  */
#line 530 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3492 "pascal.tab.c"
    break;

  case 192: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 531 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3498 "pascal.tab.c"
    break;

  case 193: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 532 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3504 "pascal.tab.c"
    break;

  case 194: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 535 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3510 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 536 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3516 "pascal.tab.c"
    break;

  case 196: /* $@5: %empty  */
#line 537 "pascal.y"
                                                                               { }
#line 3522 "pascal.tab.c"
    break;

  case 197: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 537 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3528 "pascal.tab.c"
    break;

  case 198: /* $@6: %empty  */
#line 538 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3534 "pascal.tab.c"
    break;

  case 199: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 538 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3540 "pascal.tab.c"
    break;

  case 200: /* $@7: %empty  */
#line 539 "pascal.y"
                                        { }
#line 3546 "pascal.tab.c"
    break;

  case 201: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 539 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3552 "pascal.tab.c"
    break;

  case 202: /* $@8: %empty  */
#line 540 "pascal.y"
                                                                                                           { }
#line 3558 "pascal.tab.c"
    break;

  case 203: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 540 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3564 "pascal.tab.c"
    break;

  case 204: /* $@9: %empty  */
#line 541 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3570 "pascal.tab.c"
    break;

  case 205: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3576 "pascal.tab.c"
    break;

  case 206: /* $@10: %empty  */
#line 542 "pascal.y"
                                                                                                 { }
#line 3582 "pascal.tab.c"
    break;

  case 207: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 542 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3588 "pascal.tab.c"
    break;

  case 208: /* $@11: %empty  */
#line 543 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3594 "pascal.tab.c"
    break;

  case 209: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 543 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3600 "pascal.tab.c"
    break;

  case 210: /* $@12: %empty  */
#line 544 "pascal.y"
                                                                                            { }
#line 3606 "pascal.tab.c"
    break;

  case 211: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 544 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3612 "pascal.tab.c"
    break;

  case 212: /* $@13: %empty  */
#line 545 "pascal.y"
                                                                                                             { }
#line 3618 "pascal.tab.c"
    break;

  case 213: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 545 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3626 "pascal.tab.c"
    break;

  case 214: /* $@14: %empty  */
#line 548 "pascal.y"
                                                                                              { }
#line 3632 "pascal.tab.c"
    break;

  case 215: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 548 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3640 "pascal.tab.c"
    break;

  case 216: /* $@15: %empty  */
#line 551 "pascal.y"
                                                                                             { }
#line 3646 "pascal.tab.c"
    break;

  case 217: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 551 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3654 "pascal.tab.c"
    break;

  case 218: /* $@16: %empty  */
#line 554 "pascal.y"
                                                                                                    { }
#line 3660 "pascal.tab.c"
    break;

  case 219: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 554 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3668 "pascal.tab.c"
    break;

  case 220: /* $@17: %empty  */
#line 557 "pascal.y"
                                                                                                                { }
#line 3674 "pascal.tab.c"
    break;

  case 221: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 557 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3683 "pascal.tab.c"
    break;

  case 222: /* $@18: %empty  */
#line 561 "pascal.y"
                                                                                                                  { }
#line 3689 "pascal.tab.c"
    break;

  case 223: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 561 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3698 "pascal.tab.c"
    break;

  case 224: /* $@19: %empty  */
#line 565 "pascal.y"
                                                                                              { }
#line 3704 "pascal.tab.c"
    break;

  case 225: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 565 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3713 "pascal.tab.c"
    break;

  case 226: /* $@20: %empty  */
#line 569 "pascal.y"
                                                                                                                     { }
#line 3719 "pascal.tab.c"
    break;

  case 227: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 569 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3727 "pascal.tab.c"
    break;

  case 228: /* pv_mark: %empty  */
#line 574 "pascal.y"
    { }
#line 3733 "pascal.tab.c"
    break;

  case 229: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 577 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3739 "pascal.tab.c"
    break;

  case 230: /* parameter_list_opt: LPARENT RPARENT  */
#line 578 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3745 "pascal.tab.c"
    break;

  case 231: /* parameter_list_opt: %empty  */
#line 579 "pascal.y"
     { (yyval.node) = NULL; }
#line 3751 "pascal.tab.c"
    break;

  case 232: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 582 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3757 "pascal.tab.c"
    break;

  case 233: /* parameter_decl_list: parameter_decl  */
#line 583 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3763 "pascal.tab.c"
    break;

  case 234: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 586 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3769 "pascal.tab.c"
    break;

  case 235: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 587 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3775 "pascal.tab.c"
    break;

  case 236: /* parameter_decl: VARSY id_list COLON param_type  */
#line 588 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3781 "pascal.tab.c"
    break;

  case 237: /* parameter_decl: id_list COLON param_type  */
#line 589 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3787 "pascal.tab.c"
    break;

  case 238: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 590 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3793 "pascal.tab.c"
    break;

  case 239: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 591 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3799 "pascal.tab.c"
    break;

  case 240: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 592 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3805 "pascal.tab.c"
    break;

  case 241: /* parameter_decl: IDENT id_list COLON param_type  */
#line 593 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3811 "pascal.tab.c"
    break;

  case 242: /* parameter_decl: VARSY id_list  */
#line 594 "pascal.y"
                   { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "var"); }
#line 3817 "pascal.tab.c"
    break;

  case 243: /* parameter_decl: CONSTSY id_list  */
#line 595 "pascal.y"
                     { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "const"); }
#line 3823 "pascal.tab.c"
    break;

  case 244: /* parameter_decl: IDENT id_list  */
#line 596 "pascal.y"
                   { int cr = (yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, "constref"); if (!(yyvsp[-1].node)->v.sval || (strcasecmp((yyvsp[-1].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3829 "pascal.tab.c"
    break;

  case 245: /* param_type: IDENT  */
#line 599 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3835 "pascal.tab.c"
    break;

  case 246: /* param_type: IDENT PERIOD IDENT  */
#line 600 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3841 "pascal.tab.c"
    break;

  case 247: /* param_type: ARRAYSY OFSY IDENT  */
#line 601 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3847 "pascal.tab.c"
    break;

  case 248: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 602 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3853 "pascal.tab.c"
    break;

  case 249: /* param_type: spec_ref  */
#line 603 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3859 "pascal.tab.c"
    break;

  case 250: /* routine_directives: %empty  */
#line 606 "pascal.y"
    { (yyval.node) = NULL; }
#line 3865 "pascal.tab.c"
    break;

  case 251: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 607 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe", "ms_abi_default", "sysv_abi_default",
            "vectorcall", "oldfpccall", "interrupt", "hardfloat", "varargs", "public" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3879 "pascal.tab.c"
    break;

  case 252: /* routine_directives: routine_directives IDENT COLON STRINGCONST SEMICOLON  */
#line 616 "pascal.y"
                                                          {
        if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "alias")) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3889 "pascal.tab.c"
    break;

  case 253: /* routine_directives: routine_directives IDENT IDENT STRINGCONST SEMICOLON  */
#line 621 "pascal.y"
                                                          {
        if (!(yyvsp[-3].node)->v.sval || !(yyvsp[-2].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "public") || strcasecmp((yyvsp[-2].node)->v.sval, "name")) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3899 "pascal.tab.c"
    break;

  case 254: /* pf_params: LPARENT pf_sections RPARENT  */
#line 628 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3905 "pascal.tab.c"
    break;

  case 255: /* pf_params: LPARENT RPARENT  */
#line 629 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3911 "pascal.tab.c"
    break;

  case 256: /* pf_params: %empty  */
#line 630 "pascal.y"
     { (yyval.node) = NULL; }
#line 3917 "pascal.tab.c"
    break;

  case 257: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 633 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3923 "pascal.tab.c"
    break;

  case 258: /* pf_sections: pf_section  */
#line 634 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3929 "pascal.tab.c"
    break;

  case 259: /* pf_section: id_list COLON param_type  */
#line 637 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3935 "pascal.tab.c"
    break;

  case 260: /* pf_section: VARSY id_list COLON param_type  */
#line 638 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3941 "pascal.tab.c"
    break;

  case 261: /* pf_section: CONSTSY id_list COLON param_type  */
#line 639 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3947 "pascal.tab.c"
    break;

  case 262: /* pf_section: IDENT id_list COLON param_type  */
#line 640 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3953 "pascal.tab.c"
    break;

  case 263: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 641 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3959 "pascal.tab.c"
    break;

  case 264: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 642 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3965 "pascal.tab.c"
    break;

  case 265: /* enum_list: enum_list COMMA enum_member  */
#line 645 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3971 "pascal.tab.c"
    break;

  case 266: /* enum_list: enum_member  */
#line 646 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3977 "pascal.tab.c"
    break;

  case 267: /* enum_member: IDENT  */
#line 649 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3983 "pascal.tab.c"
    break;

  case 268: /* id_list: id_list COMMA IDENT  */
#line 652 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3989 "pascal.tab.c"
    break;

  case 269: /* id_list: IDENT  */
#line 653 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3995 "pascal.tab.c"
    break;

  case 270: /* body: BEGINSY statement_list ENDSY  */
#line 656 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4001 "pascal.tab.c"
    break;

  case 271: /* $@21: %empty  */
#line 659 "pascal.y"
                             { pas_stmt_mark(); }
#line 4007 "pascal.tab.c"
    break;

  case 272: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 659 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 4013 "pascal.tab.c"
    break;

  case 273: /* $@22: %empty  */
#line 660 "pascal.y"
     { pas_stmt_mark(); }
#line 4019 "pascal.tab.c"
    break;

  case 274: /* statement_list: $@22 statement  */
#line 660 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 4025 "pascal.tab.c"
    break;

  case 275: /* $@23: %empty  */
#line 663 "pascal.y"
    { pas_stmt_mark(); }
#line 4031 "pascal.tab.c"
    break;

  case 276: /* body_stmt: $@23 statement  */
#line 663 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 4037 "pascal.tab.c"
    break;

  case 277: /* statement: statement_no_label  */
#line 666 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 4043 "pascal.tab.c"
    break;

  case 278: /* statement: INTCONST COLON statement_no_label  */
#line 667 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4049 "pascal.tab.c"
    break;

  case 279: /* statement: IDENT COLON statement_no_label  */
#line 668 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4055 "pascal.tab.c"
    break;

  case 280: /* statement_no_label: assignment  */
#line 671 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4061 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: call  */
#line 672 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4067 "pascal.tab.c"
    break;

  case 282: /* statement_no_label: compound_statement  */
#line 673 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 4073 "pascal.tab.c"
    break;

  case 283: /* statement_no_label: goto_statement  */
#line 674 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4079 "pascal.tab.c"
    break;

  case 284: /* statement_no_label: if_statement  */
#line 675 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 4085 "pascal.tab.c"
    break;

  case 285: /* statement_no_label: case_statement  */
#line 676 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4091 "pascal.tab.c"
    break;

  case 286: /* statement_no_label: while_statement  */
#line 677 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4097 "pascal.tab.c"
    break;

  case 287: /* statement_no_label: repeat_statement  */
#line 678 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4103 "pascal.tab.c"
    break;

  case 288: /* statement_no_label: for_statement  */
#line 679 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4109 "pascal.tab.c"
    break;

  case 289: /* statement_no_label: with_statement  */
#line 680 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4115 "pascal.tab.c"
    break;

  case 290: /* statement_no_label: try_statement  */
#line 681 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4121 "pascal.tab.c"
    break;

  case 291: /* statement_no_label: raise_statement  */
#line 682 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4127 "pascal.tab.c"
    break;

  case 292: /* statement_no_label: inherited_call  */
#line 683 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4133 "pascal.tab.c"
    break;

  case 293: /* statement_no_label: INHERITEDSY  */
#line 684 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4139 "pascal.tab.c"
    break;

  case 294: /* statement_no_label: %empty  */
#line 685 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4145 "pascal.tab.c"
    break;

  case 295: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 688 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4151 "pascal.tab.c"
    break;

  case 296: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 689 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4157 "pascal.tab.c"
    break;

  case 297: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 690 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4163 "pascal.tab.c"
    break;

  case 298: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 691 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4169 "pascal.tab.c"
    break;

  case 299: /* handler_list: handler  */
#line 694 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4175 "pascal.tab.c"
    break;

  case 300: /* handler_list: handler_list SEMICOLON handler  */
#line 695 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4181 "pascal.tab.c"
    break;

  case 301: /* handler_list: handler_list SEMICOLON  */
#line 696 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4187 "pascal.tab.c"
    break;

  case 302: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 699 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4193 "pascal.tab.c"
    break;

  case 303: /* handler: ONSY IDENT DOSY body_stmt  */
#line 700 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4199 "pascal.tab.c"
    break;

  case 304: /* raise_statement: RAISESY  */
#line 703 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4205 "pascal.tab.c"
    break;

  case 305: /* raise_statement: RAISESY expression  */
#line 704 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4211 "pascal.tab.c"
    break;

  case 306: /* inherited_call: INHERITEDSY IDENT  */
#line 707 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4217 "pascal.tab.c"
    break;

  case 307: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 708 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4223 "pascal.tab.c"
    break;

  case 308: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 709 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4229 "pascal.tab.c"
    break;

  case 309: /* call: IDENT  */
#line 712 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4235 "pascal.tab.c"
    break;

  case 310: /* call: spec_expr  */
#line 713 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4241 "pascal.tab.c"
    break;

  case 311: /* call: selector PERIOD IDENT  */
#line 714 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4247 "pascal.tab.c"
    break;

  case 312: /* call: call_with_args PERIOD IDENT  */
#line 715 "pascal.y"
                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4253 "pascal.tab.c"
    break;

  case 313: /* call: call_with_args  */
#line 716 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4259 "pascal.tab.c"
    break;

  case 314: /* recv_primary: STRINGCONST  */
#line 719 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4265 "pascal.tab.c"
    break;

  case 315: /* recv_primary: LPARENT expression RPARENT  */
#line 720 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4271 "pascal.tab.c"
    break;

  case 316: /* recv_primary: spec_expr  */
#line 721 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4277 "pascal.tab.c"
    break;

  case 317: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 724 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4283 "pascal.tab.c"
    break;

  case 318: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 725 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4289 "pascal.tab.c"
    break;

  case 319: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 726 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4295 "pascal.tab.c"
    break;

  case 320: /* call_with_args: IDENT LPARENT RPARENT  */
#line 727 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4301 "pascal.tab.c"
    break;

  case 321: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 728 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4307 "pascal.tab.c"
    break;

  case 322: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 729 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4313 "pascal.tab.c"
    break;

  case 323: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 730 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4319 "pascal.tab.c"
    break;

  case 324: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 731 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4325 "pascal.tab.c"
    break;

  case 325: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 732 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4331 "pascal.tab.c"
    break;

  case 326: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 733 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4337 "pascal.tab.c"
    break;

  case 327: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 734 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4343 "pascal.tab.c"
    break;

  case 328: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 735 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4349 "pascal.tab.c"
    break;

  case 329: /* argument_list: argument_list COMMA argument  */
#line 738 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4355 "pascal.tab.c"
    break;

  case 330: /* argument_list: argument  */
#line 739 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4361 "pascal.tab.c"
    break;

  case 331: /* argument: expression  */
#line 742 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4367 "pascal.tab.c"
    break;

  case 332: /* argument: expression COLON expression  */
#line 743 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4373 "pascal.tab.c"
    break;

  case 333: /* argument: expression COLON expression COLON expression  */
#line 744 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4379 "pascal.tab.c"
    break;

  case 334: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 745 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4385 "pascal.tab.c"
    break;

  case 335: /* assignment: selector BECOMES expression  */
#line 748 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4391 "pascal.tab.c"
    break;

  case 336: /* assignment: selector ADDBECOMES expression  */
#line 749 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4397 "pascal.tab.c"
    break;

  case 337: /* assignment: selector SUBBECOMES expression  */
#line 750 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4403 "pascal.tab.c"
    break;

  case 338: /* assignment: selector MULBECOMES expression  */
#line 751 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4409 "pascal.tab.c"
    break;

  case 339: /* assignment: selector DIVBECOMES expression  */
#line 752 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4415 "pascal.tab.c"
    break;

  case 340: /* assignment: call_with_args BECOMES expression  */
#line 753 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4421 "pascal.tab.c"
    break;

  case 341: /* selector: selector LBRACK expression_list RBRACK  */
#line 756 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4427 "pascal.tab.c"
    break;

  case 342: /* selector: selector PERIOD IDENT  */
#line 757 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4433 "pascal.tab.c"
    break;

  case 343: /* selector: selector ARROW  */
#line 758 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4439 "pascal.tab.c"
    break;

  case 344: /* selector: call_with_args PERIOD IDENT  */
#line 759 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4445 "pascal.tab.c"
    break;

  case 345: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 760 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4451 "pascal.tab.c"
    break;

  case 346: /* selector: call_with_args ARROW  */
#line 761 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4457 "pascal.tab.c"
    break;

  case 347: /* selector: recv_primary PERIOD IDENT  */
#line 762 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4463 "pascal.tab.c"
    break;

  case 348: /* selector: recv_primary ARROW  */
#line 763 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4469 "pascal.tab.c"
    break;

  case 349: /* selector: IDENT  */
#line 764 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4475 "pascal.tab.c"
    break;

  case 350: /* expression_list: expression_list COMMA expression  */
#line 767 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4481 "pascal.tab.c"
    break;

  case 351: /* expression_list: expression  */
#line 768 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4487 "pascal.tab.c"
    break;

  case 352: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 771 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4493 "pascal.tab.c"
    break;

  case 353: /* goto_statement: GOTOSY INTCONST  */
#line 774 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4499 "pascal.tab.c"
    break;

  case 354: /* goto_statement: GOTOSY IDENT  */
#line 775 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4505 "pascal.tab.c"
    break;

  case 355: /* if_statement: IFSY expression THENSY body_stmt  */
#line 778 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4511 "pascal.tab.c"
    break;

  case 356: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 779 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4517 "pascal.tab.c"
    break;

  case 357: /* $@24: %empty  */
#line 782 "pascal.y"
                           { }
#line 4523 "pascal.tab.c"
    break;

  case 358: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 782 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4529 "pascal.tab.c"
    break;

  case 359: /* case_tail: %empty  */
#line 785 "pascal.y"
    { (yyval.node) = NULL; }
#line 4535 "pascal.tab.c"
    break;

  case 360: /* case_tail: ELSESY statement_list  */
#line 786 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4541 "pascal.tab.c"
    break;

  case 361: /* case_tail: OTHERWISESY statement_list  */
#line 787 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4547 "pascal.tab.c"
    break;

  case 362: /* case_list: case_list SEMICOLON case_elem  */
#line 790 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4553 "pascal.tab.c"
    break;

  case 363: /* case_list: case_elem  */
#line 791 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4559 "pascal.tab.c"
    break;

  case 364: /* case_elem: constant_list COLON body_stmt  */
#line 794 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4565 "pascal.tab.c"
    break;

  case 365: /* case_elem: %empty  */
#line 795 "pascal.y"
     { (yyval.node) = NULL; }
#line 4571 "pascal.tab.c"
    break;

  case 366: /* constant_list: constant_list COMMA case_label  */
#line 798 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4577 "pascal.tab.c"
    break;

  case 367: /* constant_list: case_label  */
#line 799 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4583 "pascal.tab.c"
    break;

  case 368: /* case_label: b_expr  */
#line 802 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4589 "pascal.tab.c"
    break;

  case 369: /* case_label: b_expr DOTDOT b_expr  */
#line 803 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4595 "pascal.tab.c"
    break;

  case 370: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 806 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4601 "pascal.tab.c"
    break;

  case 371: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 809 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4607 "pascal.tab.c"
    break;

  case 372: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 812 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4613 "pascal.tab.c"
    break;

  case 373: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 813 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4619 "pascal.tab.c"
    break;

  case 374: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 814 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4625 "pascal.tab.c"
    break;

  case 375: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 817 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4631 "pascal.tab.c"
    break;

  case 376: /* with_open: with_open COMMA selector  */
#line 820 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4637 "pascal.tab.c"
    break;

  case 377: /* with_open: selector  */
#line 821 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4643 "pascal.tab.c"
    break;

  case 378: /* expression: simple_expression  */
#line 828 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4649 "pascal.tab.c"
    break;

  case 379: /* expression: expression INOP simple_expression  */
#line 829 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4655 "pascal.tab.c"
    break;

  case 380: /* expression: expression LTOP simple_expression  */
#line 830 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4661 "pascal.tab.c"
    break;

  case 381: /* expression: expression LEOP simple_expression  */
#line 831 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4667 "pascal.tab.c"
    break;

  case 382: /* expression: expression GTOP simple_expression  */
#line 832 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4673 "pascal.tab.c"
    break;

  case 383: /* expression: expression GEOP simple_expression  */
#line 833 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4679 "pascal.tab.c"
    break;

  case 384: /* expression: expression NEOP simple_expression  */
#line 834 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4685 "pascal.tab.c"
    break;

  case 385: /* expression: expression EQOP simple_expression  */
#line 835 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4691 "pascal.tab.c"
    break;

  case 386: /* expression: expression ISOP simple_expression  */
#line 836 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4697 "pascal.tab.c"
    break;

  case 387: /* simple_expression: term  */
#line 839 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4703 "pascal.tab.c"
    break;

  case 388: /* simple_expression: PLUS term  */
#line 840 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4709 "pascal.tab.c"
    break;

  case 389: /* simple_expression: MINUS term  */
#line 841 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4715 "pascal.tab.c"
    break;

  case 390: /* simple_expression: simple_expression PLUS term  */
#line 842 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4721 "pascal.tab.c"
    break;

  case 391: /* simple_expression: simple_expression MINUS term  */
#line 843 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4727 "pascal.tab.c"
    break;

  case 392: /* simple_expression: simple_expression OROP term  */
#line 844 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4733 "pascal.tab.c"
    break;

  case 393: /* simple_expression: simple_expression XOROP term  */
#line 845 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4739 "pascal.tab.c"
    break;

  case 394: /* term: power  */
#line 848 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4745 "pascal.tab.c"
    break;

  case 395: /* term: term MUL power  */
#line 849 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4751 "pascal.tab.c"
    break;

  case 396: /* term: term RDIV power  */
#line 850 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4757 "pascal.tab.c"
    break;

  case 397: /* term: term IDIV power  */
#line 851 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4763 "pascal.tab.c"
    break;

  case 398: /* term: term IMOD power  */
#line 852 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4769 "pascal.tab.c"
    break;

  case 399: /* term: term ANDOP power  */
#line 853 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4775 "pascal.tab.c"
    break;

  case 400: /* term: term MUL MINUS power  */
#line 854 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4781 "pascal.tab.c"
    break;

  case 401: /* term: term RDIV MINUS power  */
#line 855 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4787 "pascal.tab.c"
    break;

  case 402: /* term: term IDIV MINUS power  */
#line 856 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4793 "pascal.tab.c"
    break;

  case 403: /* term: term IMOD MINUS power  */
#line 857 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4799 "pascal.tab.c"
    break;

  case 404: /* term: term ANDOP MINUS power  */
#line 858 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4805 "pascal.tab.c"
    break;

  case 405: /* term: term SHIFTOP power  */
#line 859 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4811 "pascal.tab.c"
    break;

  case 406: /* term: term ASOP power  */
#line 860 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4817 "pascal.tab.c"
    break;

  case 407: /* power: factor  */
#line 863 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4823 "pascal.tab.c"
    break;

  case 408: /* power: power POWOP factor  */
#line 864 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4829 "pascal.tab.c"
    break;

  case 409: /* factor: selector  */
#line 867 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4835 "pascal.tab.c"
    break;

  case 410: /* factor: call_with_args  */
#line 868 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4841 "pascal.tab.c"
    break;

  case 411: /* factor: INTCONST  */
#line 869 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4847 "pascal.tab.c"
    break;

  case 412: /* factor: REALCONST  */
#line 870 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4853 "pascal.tab.c"
    break;

  case 413: /* factor: STRINGCONST  */
#line 871 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4859 "pascal.tab.c"
    break;

  case 414: /* factor: CHARCODE  */
#line 872 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4865 "pascal.tab.c"
    break;

  case 415: /* factor: LPARENT expression RPARENT  */
#line 873 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4871 "pascal.tab.c"
    break;

  case 416: /* factor: inherited_call  */
#line 874 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4877 "pascal.tab.c"
    break;

  case 417: /* factor: NOTSY factor  */
#line 875 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4883 "pascal.tab.c"
    break;

  case 418: /* factor: ATSIGN factor  */
#line 876 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4889 "pascal.tab.c"
    break;

  case 419: /* factor: LBRACK RBRACK  */
#line 877 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4895 "pascal.tab.c"
    break;

  case 420: /* factor: LBRACK set_member_list RBRACK  */
#line 878 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4901 "pascal.tab.c"
    break;

  case 421: /* set_member_list: set_member  */
#line 881 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4907 "pascal.tab.c"
    break;

  case 422: /* set_member_list: set_member_list COMMA set_member  */
#line 882 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4913 "pascal.tab.c"
    break;

  case 423: /* set_member: expression  */
#line 885 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4919 "pascal.tab.c"
    break;

  case 424: /* set_member: expression DOTDOT expression  */
#line 886 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4925 "pascal.tab.c"
    break;


#line 4929 "pascal.tab.c"

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

#line 892 "pascal.y"

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
