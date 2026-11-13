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
  YYSYMBOL_BITPACKEDSY = 28,               /* BITPACKEDSY  */
  YYSYMBOL_OFSY = 29,                      /* OFSY  */
  YYSYMBOL_FILESY = 30,                    /* FILESY  */
  YYSYMBOL_ENDSY = 31,                     /* ENDSY  */
  YYSYMBOL_SETSY = 32,                     /* SETSY  */
  YYSYMBOL_VARSY = 33,                     /* VARSY  */
  YYSYMBOL_THENSY = 34,                    /* THENSY  */
  YYSYMBOL_RECORDSY = 35,                  /* RECORDSY  */
  YYSYMBOL_EXPORTSSY = 36,                 /* EXPORTSSY  */
  YYSYMBOL_FUNCTIONSY = 37,                /* FUNCTIONSY  */
  YYSYMBOL_BEGINSY = 38,                   /* BEGINSY  */
  YYSYMBOL_BECOMES = 39,                   /* BECOMES  */
  YYSYMBOL_TYPESY = 40,                    /* TYPESY  */
  YYSYMBOL_IFSY = 41,                      /* IFSY  */
  YYSYMBOL_ELSESY = 42,                    /* ELSESY  */
  YYSYMBOL_INOP = 43,                      /* INOP  */
  YYSYMBOL_NOTSY = 44,                     /* NOTSY  */
  YYSYMBOL_IDIV = 45,                      /* IDIV  */
  YYSYMBOL_IMOD = 46,                      /* IMOD  */
  YYSYMBOL_ANDOP = 47,                     /* ANDOP  */
  YYSYMBOL_OROP = 48,                      /* OROP  */
  YYSYMBOL_LTOP = 49,                      /* LTOP  */
  YYSYMBOL_LEOP = 50,                      /* LEOP  */
  YYSYMBOL_GTOP = 51,                      /* GTOP  */
  YYSYMBOL_GEOP = 52,                      /* GEOP  */
  YYSYMBOL_NEOP = 53,                      /* NEOP  */
  YYSYMBOL_EQOP = 54,                      /* EQOP  */
  YYSYMBOL_PLUS = 55,                      /* PLUS  */
  YYSYMBOL_MINUS = 56,                     /* MINUS  */
  YYSYMBOL_MUL = 57,                       /* MUL  */
  YYSYMBOL_RDIV = 58,                      /* RDIV  */
  YYSYMBOL_COMMA = 59,                     /* COMMA  */
  YYSYMBOL_PERIOD = 60,                    /* PERIOD  */
  YYSYMBOL_COLON = 61,                     /* COLON  */
  YYSYMBOL_ARROW = 62,                     /* ARROW  */
  YYSYMBOL_LBRACK = 63,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 64,                    /* RBRACK  */
  YYSYMBOL_LPARENT = 65,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 66,                   /* RPARENT  */
  YYSYMBOL_DOTDOT = 67,                    /* DOTDOT  */
  YYSYMBOL_ATSIGN = 68,                    /* ATSIGN  */
  YYSYMBOL_ADDBECOMES = 69,                /* ADDBECOMES  */
  YYSYMBOL_SUBBECOMES = 70,                /* SUBBECOMES  */
  YYSYMBOL_MULBECOMES = 71,                /* MULBECOMES  */
  YYSYMBOL_DIVBECOMES = 72,                /* DIVBECOMES  */
  YYSYMBOL_INTCONST = 73,                  /* INTCONST  */
  YYSYMBOL_CHARCODE = 74,                  /* CHARCODE  */
  YYSYMBOL_REALCONST = 75,                 /* REALCONST  */
  YYSYMBOL_STRINGCONST = 76,               /* STRINGCONST  */
  YYSYMBOL_IDENT = 77,                     /* IDENT  */
  YYSYMBOL_SHIFTOP = 78,                   /* SHIFTOP  */
  YYSYMBOL_XOROP = 79,                     /* XOROP  */
  YYSYMBOL_POWOP = 80,                     /* POWOP  */
  YYSYMBOL_CLASSSY = 81,                   /* CLASSSY  */
  YYSYMBOL_ISOP = 82,                      /* ISOP  */
  YYSYMBOL_ASOP = 83,                      /* ASOP  */
  YYSYMBOL_OBJECTSY = 84,                  /* OBJECTSY  */
  YYSYMBOL_TRYSY = 85,                     /* TRYSY  */
  YYSYMBOL_EXCEPTSY = 86,                  /* EXCEPTSY  */
  YYSYMBOL_FINALLYSY = 87,                 /* FINALLYSY  */
  YYSYMBOL_RAISESY = 88,                   /* RAISESY  */
  YYSYMBOL_INHERITEDSY = 89,               /* INHERITEDSY  */
  YYSYMBOL_CONSTRUCTORSY = 90,             /* CONSTRUCTORSY  */
  YYSYMBOL_DESTRUCTORSY = 91,              /* DESTRUCTORSY  */
  YYSYMBOL_PROPERTYSY = 92,                /* PROPERTYSY  */
  YYSYMBOL_ONSY = 93,                      /* ONSY  */
  YYSYMBOL_GENERICSY = 94,                 /* GENERICSY  */
  YYSYMBOL_GENERICTSY = 95,                /* GENERICTSY  */
  YYSYMBOL_SPECIALIZESY = 96,              /* SPECIALIZESY  */
  YYSYMBOL_GENLT = 97,                     /* GENLT  */
  YYSYMBOL_GENGT = 98,                     /* GENGT  */
  YYSYMBOL_YYACCEPT = 99,                  /* $accept  */
  YYSYMBOL_compilation_unit = 100,         /* compilation_unit  */
  YYSYMBOL_program = 101,                  /* program  */
  YYSYMBOL_program_name = 102,             /* program_name  */
  YYSYMBOL_file_id_list_opt = 103,         /* file_id_list_opt  */
  YYSYMBOL_block = 104,                    /* block  */
  YYSYMBOL_decl_part_list = 105,           /* decl_part_list  */
  YYSYMBOL_decl_part = 106,                /* decl_part  */
  YYSYMBOL_unit = 107,                     /* unit  */
  YYSYMBOL_intf_part_list = 108,           /* intf_part_list  */
  YYSYMBOL_intf_part = 109,                /* intf_part  */
  YYSYMBOL_init_opt = 110,                 /* init_opt  */
  YYSYMBOL_fini_opt = 111,                 /* fini_opt  */
  YYSYMBOL_label_list = 112,               /* label_list  */
  YYSYMBOL_const_decl_list = 113,          /* const_decl_list  */
  YYSYMBOL_const_decl = 114,               /* const_decl  */
  YYSYMBOL_typed_value = 115,              /* typed_value  */
  YYSYMBOL_typed_value_list = 116,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 117,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 118,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 119,           /* type_decl_list  */
  YYSYMBOL_type_decl = 120,                /* type_decl  */
  YYSYMBOL_uses_list = 121,                /* uses_list  */
  YYSYMBOL_uses_name = 122,                /* uses_name  */
  YYSYMBOL_spec_gen = 123,                 /* spec_gen  */
  YYSYMBOL_spec_expr = 124,                /* spec_expr  */
  YYSYMBOL_class_parents = 125,            /* class_parents  */
  YYSYMBOL_parent_item = 126,              /* parent_item  */
  YYSYMBOL_spec_kw = 127,                  /* spec_kw  */
  YYSYMBOL_spec_ref = 128,                 /* spec_ref  */
  YYSYMBOL_type_arg_list = 129,            /* type_arg_list  */
  YYSYMBOL_type = 130,                     /* type  */
  YYSYMBOL_131_1 = 131,                    /* $@1  */
  YYSYMBOL_132_2 = 132,                    /* $@2  */
  YYSYMBOL_133_3 = 133,                    /* $@3  */
  YYSYMBOL_result_type = 134,              /* result_type  */
  YYSYMBOL_guid_opt = 135,                 /* guid_opt  */
  YYSYMBOL_class_members = 136,            /* class_members  */
  YYSYMBOL_class_member = 137,             /* class_member  */
  YYSYMBOL_prop_index_opt = 138,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 139,               /* prop_specs  */
  YYSYMBOL_packed_opt = 140,               /* packed_opt  */
  YYSYMBOL_simple_type = 141,              /* simple_type  */
  YYSYMBOL_lo_bound = 142,                 /* lo_bound  */
  YYSYMBOL_lo_term = 143,                  /* lo_term  */
  YYSYMBOL_b_expr = 144,                   /* b_expr  */
  YYSYMBOL_b_term = 145,                   /* b_term  */
  YYSYMBOL_b_factor = 146,                 /* b_factor  */
  YYSYMBOL_record_body = 147,              /* record_body  */
  YYSYMBOL_record_field_list = 148,        /* record_field_list  */
  YYSYMBOL_record_field = 149,             /* record_field  */
  YYSYMBOL_150_4 = 150,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 151,            /* case_arm_mark  */
  YYSYMBOL_case_open = 152,                /* case_open  */
  YYSYMBOL_record_case_opt = 153,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 154,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 155,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 156,            /* var_decl_list  */
  YYSYMBOL_var_decl = 157,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 158,           /* procedure_decl  */
  YYSYMBOL_159_5 = 159,                    /* $@5  */
  YYSYMBOL_160_6 = 160,                    /* $@6  */
  YYSYMBOL_161_7 = 161,                    /* $@7  */
  YYSYMBOL_162_8 = 162,                    /* $@8  */
  YYSYMBOL_163_9 = 163,                    /* $@9  */
  YYSYMBOL_164_10 = 164,                   /* $@10  */
  YYSYMBOL_165_11 = 165,                   /* $@11  */
  YYSYMBOL_166_12 = 166,                   /* $@12  */
  YYSYMBOL_167_13 = 167,                   /* $@13  */
  YYSYMBOL_168_14 = 168,                   /* $@14  */
  YYSYMBOL_169_15 = 169,                   /* $@15  */
  YYSYMBOL_170_16 = 170,                   /* $@16  */
  YYSYMBOL_171_17 = 171,                   /* $@17  */
  YYSYMBOL_172_18 = 172,                   /* $@18  */
  YYSYMBOL_173_19 = 173,                   /* $@19  */
  YYSYMBOL_174_20 = 174,                   /* $@20  */
  YYSYMBOL_pv_mark = 175,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 176,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 177,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 178,           /* parameter_decl  */
  YYSYMBOL_param_type = 179,               /* param_type  */
  YYSYMBOL_routine_directives = 180,       /* routine_directives  */
  YYSYMBOL_pf_params = 181,                /* pf_params  */
  YYSYMBOL_pf_sections = 182,              /* pf_sections  */
  YYSYMBOL_pf_section = 183,               /* pf_section  */
  YYSYMBOL_enum_list = 184,                /* enum_list  */
  YYSYMBOL_enum_member = 185,              /* enum_member  */
  YYSYMBOL_id_list = 186,                  /* id_list  */
  YYSYMBOL_body = 187,                     /* body  */
  YYSYMBOL_statement_list = 188,           /* statement_list  */
  YYSYMBOL_189_21 = 189,                   /* $@21  */
  YYSYMBOL_190_22 = 190,                   /* $@22  */
  YYSYMBOL_body_stmt = 191,                /* body_stmt  */
  YYSYMBOL_192_23 = 192,                   /* $@23  */
  YYSYMBOL_statement = 193,                /* statement  */
  YYSYMBOL_statement_no_label = 194,       /* statement_no_label  */
  YYSYMBOL_try_statement = 195,            /* try_statement  */
  YYSYMBOL_handler_list = 196,             /* handler_list  */
  YYSYMBOL_handler = 197,                  /* handler  */
  YYSYMBOL_raise_statement = 198,          /* raise_statement  */
  YYSYMBOL_inherited_call = 199,           /* inherited_call  */
  YYSYMBOL_call = 200,                     /* call  */
  YYSYMBOL_recv_primary = 201,             /* recv_primary  */
  YYSYMBOL_call_with_args = 202,           /* call_with_args  */
  YYSYMBOL_argument_list = 203,            /* argument_list  */
  YYSYMBOL_argument = 204,                 /* argument  */
  YYSYMBOL_assignment = 205,               /* assignment  */
  YYSYMBOL_selector = 206,                 /* selector  */
  YYSYMBOL_expression_list = 207,          /* expression_list  */
  YYSYMBOL_compound_statement = 208,       /* compound_statement  */
  YYSYMBOL_goto_statement = 209,           /* goto_statement  */
  YYSYMBOL_if_statement = 210,             /* if_statement  */
  YYSYMBOL_case_statement = 211,           /* case_statement  */
  YYSYMBOL_212_24 = 212,                   /* $@24  */
  YYSYMBOL_case_tail = 213,                /* case_tail  */
  YYSYMBOL_case_list = 214,                /* case_list  */
  YYSYMBOL_case_elem = 215,                /* case_elem  */
  YYSYMBOL_constant_list = 216,            /* constant_list  */
  YYSYMBOL_case_label = 217,               /* case_label  */
  YYSYMBOL_while_statement = 218,          /* while_statement  */
  YYSYMBOL_repeat_statement = 219,         /* repeat_statement  */
  YYSYMBOL_for_statement = 220,            /* for_statement  */
  YYSYMBOL_with_statement = 221,           /* with_statement  */
  YYSYMBOL_with_open = 222,                /* with_open  */
  YYSYMBOL_expression = 223,               /* expression  */
  YYSYMBOL_simple_expression = 224,        /* simple_expression  */
  YYSYMBOL_term = 225,                     /* term  */
  YYSYMBOL_power = 226,                    /* power  */
  YYSYMBOL_factor = 227,                   /* factor  */
  YYSYMBOL_set_member_list = 228,          /* set_member_list  */
  YYSYMBOL_set_member = 229                /* set_member  */
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
#define YYLAST   1943

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  131
/* YYNRULES -- Number of rules.  */
#define YYNRULES  425
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1025

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   353


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
      95,    96,    97,    98
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
     438,   439,   442,   443,   444,   445,   446,   447,   450,   451,
     452,   453,   454,   457,   458,   459,   460,   461,   462,   463,
     466,   467,   468,   469,   470,   473,   474,   475,   476,   477,
     480,   481,   482,   483,   486,   489,   490,   493,   493,   494,
     495,   496,   497,   498,   499,   500,   501,   502,   503,   504,
     507,   510,   513,   514,   515,   516,   519,   520,   523,   524,
     527,   528,   531,   532,   533,   536,   537,   538,   538,   539,
     539,   540,   540,   541,   541,   542,   542,   543,   543,   544,
     544,   545,   545,   546,   546,   549,   549,   552,   552,   555,
     555,   558,   558,   562,   562,   566,   566,   570,   570,   575,
     578,   579,   580,   583,   584,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   600,   601,   602,   603,
     604,   607,   608,   617,   622,   629,   630,   631,   634,   635,
     638,   639,   640,   641,   642,   643,   646,   647,   650,   653,
     654,   657,   660,   660,   661,   661,   664,   664,   667,   668,
     669,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   689,   690,   691,   692,
     695,   696,   697,   700,   701,   704,   705,   708,   709,   710,
     713,   714,   715,   716,   717,   720,   721,   722,   725,   726,
     727,   728,   729,   730,   731,   732,   733,   734,   735,   736,
     739,   740,   743,   744,   745,   746,   749,   750,   751,   752,
     753,   754,   757,   758,   759,   760,   761,   762,   763,   764,
     765,   768,   769,   772,   775,   776,   779,   780,   783,   783,
     786,   787,   788,   791,   792,   795,   796,   799,   800,   803,
     804,   807,   810,   813,   814,   815,   818,   821,   822,   829,
     830,   831,   832,   833,   834,   835,   836,   837,   840,   841,
     842,   843,   844,   845,   846,   849,   850,   851,   852,   853,
     854,   855,   856,   857,   858,   859,   860,   861,   864,   865,
     868,   869,   870,   871,   872,   873,   874,   875,   876,   877,
     878,   879,   882,   883,   886,   887
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
  "PACKEDSY", "BITPACKEDSY", "OFSY", "FILESY", "ENDSY", "SETSY", "VARSY",
  "THENSY", "RECORDSY", "EXPORTSSY", "FUNCTIONSY", "BEGINSY", "BECOMES",
  "TYPESY", "IFSY", "ELSESY", "INOP", "NOTSY", "IDIV", "IMOD", "ANDOP",
  "OROP", "LTOP", "LEOP", "GTOP", "GEOP", "NEOP", "EQOP", "PLUS", "MINUS",
  "MUL", "RDIV", "COMMA", "PERIOD", "COLON", "ARROW", "LBRACK", "RBRACK",
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

#define YYPACT_NINF (-874)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-351)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     160,   -20,   -10,   154,  -874,   109,  1105,  -874,  -874,   462,
    -874,    41,  -874,  -874,     7,   140,   -10,   143,   199,   199,
     250,  -874,   263,   206,   256,   279,   398,  -874,  -874,  -874,
     288,   199,   366,   375,   328,  -874,  -874,   148,   216,   140,
    -874,   187,   361,   380,  -874,   199,  -874,   541,   219,   400,
      55,   902,   572,   379,   263,  -874,   388,   420,   448,   381,
     477,   495,   509,  -874,   447,  -874,  -874,  -874,  -874,   626,
    1511,  1135,  -874,  -874,   -10,   199,   532,   501,  -874,   535,
    1135,  -874,   199,   545,    97,  -874,  -874,   667,   562,  -874,
    1511,   411,  1511,  -874,  1511,  1511,   581,  -874,   886,  -874,
    1511,   583,   618,  -874,   536,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,   608,   590,  -874,  1182,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,   199,  1222,   684,  -874,   683,   692,
     700,   690,   708,   754,   767,  -874,   750,   891,  -874,  -874,
    1719,  1719,  1719,  1301,  1511,  1719,  -874,  -874,  -874,   660,
      87,   583,   776,  -874,   676,   764,    44,   406,   837,   766,
    -874,   696,   797,  -874,  -874,   797,   795,   637,   637,   242,
     799,  -874,  -874,  -874,  -874,  1596,   128,   232,  -874,  -874,
    -874,  -874,   836,   489,  -874,   403,   427,   361,    12,  -874,
     133,   875,  -874,    37,   352,  -874,  -874,   830,   902,  -874,
    -874,   761,   311,   740,   676,   764,   204,  1297,   290,  1767,
     565,  1021,  1021,  1326,   999,    32,  1861,   828,   851,  1361,
     832,  -874,  1511,   841,  -874,  1511,  1386,  1511,   842,  -874,
    1511,  1511,  1511,  1511,  1511,   396,    83,   245,   924,   199,
     855,   858,   212,  -874,  -874,   199,   199,  -874,   140,   -10,
    -874,   860,   199,   861,   263,  -874,  -874,   837,   837,  -874,
     720,   619,  -874,  1779,  -874,   862,   867,  -874,  1511,  1511,
    1511,  1511,  1511,  1511,  1511,  1511,  1719,  1719,  1719,  1719,
    1602,  1627,  1652,  1677,  1702,  1719,  1719,  1719,   831,   273,
    -874,   179,   916,   892,   933,   624,   427,   427,   106,  -874,
    -874,   518,  -874,   999,   877,  1215,  1215,   882,   273,   137,
     883,   884,   885,   176,   888,   895,   897,   938,    57,  -874,
     718,  1536,   251,   952,   953,  -874,   599,   599,  1215,   599,
     599,   599,   599,  -874,   501,   199,   907,   199,   911,  -874,
     199,    49,  -874,   727,  -874,  -874,  1536,   912,  -874,   501,
    -874,   306,  -874,  1511,  1511,  1511,  -874,  -874,   411,  -874,
    -874,  -874,  -874,   934,  -874,  -874,  -874,   522,  -874,   780,
    1801,   193,  -874,   899,  -874,  1411,   999,  -874,   564,   928,
    1861,  1111,   664,  1861,  -874,   577,  1861,  1742,   665,  1861,
    1861,  1861,  1861,   941,  -874,  -874,   273,  -874,   424,  -874,
    -874,  -874,  -874,   501,   501,   481,   506,   140,   244,  1011,
    -874,   199,  -874,   263,  1511,  1511,  -874,   749,   935,   936,
     406,   406,   406,   406,   406,   406,   406,   406,   837,   837,
     837,   837,  1719,   766,  1719,   766,  1719,   766,  1719,   766,
    1719,   766,   766,   766,  -874,   943,   106,   578,  -874,  -874,
     318,   199,   937,   199,   939,  -874,   199,    61,  -874,   758,
     945,   306,  1135,   949,   799,  -874,   516,   436,   599,   599,
    1215,   686,  -874,   507,   432,  -874,   413,  -874,   579,   950,
    -874,   951,   203,   508,   956,   958,   961,  -874,   762,  -874,
    -874,   944,   962,   966,  -874,   234,  -874,  -874,  -874,    52,
     973,  -874,  -874,  1561,  1006,  1861,  1135,   999,  1135,   999,
      52,   432,   432,   773,  -874,  -874,  -874,  -874,   501,  1007,
     786,   797,   791,   797,   792,   222,  -874,    29,    30,  1008,
    1010,   501,   959,  1024,   182,  -874,  1025,   364,  1187,  1861,
    -874,   902,  -874,   764,  1215,  1014,  1511,  -874,  1511,  1511,
     999,  -874,   975,   310,   362,  -874,   365,  -874,   582,   528,
    -874,  1436,  1461,  1511,  -874,  -874,  1486,  -874,  1135,   592,
    1003,   501,   501,   501,   501,  1048,  1053,  -874,  -874,  -874,
    -874,  -874,  1045,   501,   501,  1861,  -874,   766,   766,   766,
     766,   766,  -874,   273,   998,  -874,   802,   797,   809,   797,
     810,   228,  -874,    29,  -874,  1034,  -874,  -874,  -874,  -874,
     432,   432,   445,   990,  1215,   599,   599,  -874,   599,   599,
     599,   599,  -874,  -874,  -874,  -874,  -874,  1000,  1001,  1005,
    1012,  -874,  -874,  1017,  1135,   501,   501,    52,  -874,  1019,
    -874,  -874,   501,   501,  -874,   264,  1135,   239,  1028,    65,
    1779,  -874,  -874,   716,  -874,  -874,  1052,  1083,  -874,    29,
    -874,    29,  1029,    29,  -874,  1062,   269,  -874,  1039,  1091,
      47,  -874,  -874,  -874,  1038,   306,  -874,  1023,  -874,  1511,
    1511,  -874,  -874,   461,    48,  -874,   818,  -874,  -874,  -874,
    1814,  1826,  -874,   223,  -874,   899,  -874,  -874,  -874,  -874,
    -874,  -874,   603,  -874,   622,  1861,  -874,   638,  1098,  1099,
    1135,  1102,  1047,  1050,    95,  -874,  -874,   501,   501,  1109,
    1109,  -874,  1085,  1113,  1058,  -874,  -874,    29,  -874,    29,
    1059,    29,  -874,  -874,  1044,   342,  -874,  -874,   493,   432,
     432,  -874,  -874,  -874,  -874,   543,   501,   501,  -874,  1060,
    -874,  -874,   501,   501,   222,  1063,  1122,  -874,  1067,  1101,
    1110,   501,   501,  -874,  -874,  -874,   335,  -874,  1536,  1536,
     476,  -874,   999,  1106,  -874,  -874,  1057,  1076,  -874,   306,
    -874,    33,  1069,  1511,  -874,  -874,  1068,  1072,  1131,   306,
    1144,  -874,    68,  1262,  1285,  -874,  1215,  1215,  -874,  -874,
    1121,  1215,  -874,  -874,  1511,  1511,  -874,  1077,  -874,   370,
    -874,  -874,  -874,  -874,  -874,  1150,  -874,   306,   306,  -874,
     306,  1057,  1057,  1151,  1096,  1109,  1100,  -874,   306,   640,
    -874,  -874,   306,  -874,  -874,  -874,  -874,  -874,  1159,  1107,
    1148,   501,   501,  1164,  1165,    45,   273,  -874,   306,  -874,
    1108,  1116,  1117,  1215,   799,  1155,  -874,  -874,   639,  -874,
    1118,  1123,  -874,  1057,  -874,  1511,  -874,  -874,  -874,  -874,
    1861,  1183,  1184,  -874,  1188,  -874,  1189,  -874,  -874,  -874,
     773,  -874,  1109,  1109,  -874,  -874,  -874,  1839,  1861,  -874,
    1175,  -874,  -874,  1057,  1193,  1196,  1057,  1197,  -874,  -874,
    -874,   306,  -874,  1057,  1198,  -874,  -874,  -874,   306,  1128,
    1202,  1153,  -874,  -874,  -874,  -874,  -874,  -874,   306,   306,
    1210,  -874,   826,   641,  -874,  1536,  -874,  1536,  1191,  -874,
    -874,  1212,  1861,  -874,  -874,  -874,  1057,  -874,  1213,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  1217,  1220,
    1057,  1221,  -874,  1227,  -874,  -874,   306,    43,  -874,  -874,
    1215,  1162,  1204,  1215,  -874,  -874,  1135,  1135,  1230,  -874,
    1057,  -874,  -874,  -874,  1250,  1057,  1057,  1251,  1057,  -874,
    -874,  -874,  -874,  1057,  -874,  1252,  -874,   728,  -874,  -874,
    -874,  1210,  -874,  -874,  -874,  -874,  1253,  -874,  -874,  -874,
    -874,  -874,  1255,  1057,  -874,  -874,  -874,    52,  1215,  1258,
    -874,  1261,  1263,  1267,  -874,  -874,  1201,  1210,  -874,  -874,
    -874,  -874,  1268,  -874,  -874
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
      65,     0,     1,     5,     0,     0,     0,     0,     0,     0,
       0,   274,     0,     0,     0,     0,     0,    11,    17,    10,
       0,     0,     0,     0,     0,    36,    37,     0,     0,    14,
      39,     0,    63,   229,   270,    16,   191,     0,     0,   229,
       0,   295,     0,     0,    15,    56,     0,     0,     0,     0,
       0,     0,     0,     7,     0,    12,    22,    66,    13,     0,
       0,   131,    38,    18,     0,     0,     0,   232,   190,     0,
     131,    19,     0,     0,   232,   272,   271,     0,     0,   274,
       0,     0,     0,   274,     0,     0,     0,   315,   310,   274,
     305,   294,     0,    69,   311,    68,   275,   278,   291,   292,
     293,   282,     0,   314,   281,     0,   283,   284,   285,   286,
     287,   288,   289,   290,     0,   131,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     8,     0,     0,    34,    35,
       0,     0,     0,     0,     0,     0,   412,   415,   413,   414,
     350,     0,   317,   417,   411,   410,     0,   379,   388,   395,
     408,   108,   257,   129,   130,   257,     0,     0,     0,     0,
       0,    51,    54,    52,    53,   133,   110,   179,   143,    76,
      75,   136,     0,     0,    80,     0,   138,    64,     0,   229,
       0,     0,   269,     0,     0,   229,   201,     0,   295,   354,
     355,     0,     0,     0,     0,   378,     0,     0,     0,     0,
       0,   295,   295,     0,     0,     0,   306,   307,     0,     0,
       0,   349,     0,     0,   347,     0,     0,     0,     0,   344,
       0,     0,     0,     0,     0,     0,   108,   110,     0,     0,
       0,     0,     0,   229,   229,     0,     0,     4,     0,     0,
      12,     0,     0,     0,     0,    21,   418,   389,   390,   420,
     424,     0,   422,     0,   419,     0,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,     0,    88,     0,     0,    50,   139,   140,    81,    83,
     268,     0,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,     0,     0,     0,     0,   185,   166,
       0,     0,     0,    97,     0,    93,     0,     0,     0,     0,
       0,     0,     0,   229,   232,     0,     0,     0,     0,   231,
     270,     0,   234,     0,   251,   192,     0,     0,   229,   232,
      12,     0,   273,     0,     0,     0,   276,   276,     0,   358,
     353,   276,   316,   310,   279,   280,   321,     0,   331,   410,
     332,     0,    78,   274,   274,     0,     0,   320,     0,   348,
     341,   313,     0,   352,   323,     0,   336,   312,     0,   337,
     338,   339,   340,     0,    61,    60,     0,    57,     0,   229,
     229,   229,   229,   232,   232,     0,     0,    24,     0,    31,
     229,    26,   229,    25,     0,     0,   421,   416,   345,   343,
     380,   381,   382,   383,   384,   385,   386,   387,   393,   391,
     392,   394,     0,   398,     0,   399,     0,   400,     0,   396,
       0,   397,   406,   407,   409,     0,    72,     0,    70,    73,
       0,     0,     0,     0,     0,   256,   270,     0,   259,     0,
       0,     0,   131,     0,     0,   132,     0,   135,     0,     0,
       0,    50,   160,     0,   150,   155,     0,    82,     0,     0,
     101,     0,   112,     0,     0,     0,     0,   111,     0,   229,
     229,     0,     0,     0,   176,     0,   229,   229,    99,   179,
       0,   164,   167,     0,     0,    42,   131,     0,   131,     0,
     179,   141,   142,   137,   147,   148,   146,   149,   232,     0,
     244,   257,   243,   257,   245,     0,   230,     0,   197,     0,
       0,   232,     0,     0,   105,   107,     0,     0,     0,   372,
     371,   295,   376,   377,   366,   356,     0,   318,     0,     0,
       0,    67,     0,     0,     0,   300,     0,   308,     0,     0,
     319,     0,     0,     0,   346,   322,     0,   342,   131,     0,
       0,   232,   232,   232,   232,     0,     0,   229,   229,    23,
     274,   274,    33,   232,   232,   425,   423,   403,   404,   405,
     401,   402,   109,     0,   108,   103,     0,   257,     0,   257,
       0,     0,   255,     0,    89,    90,   110,   144,   266,    77,
     151,   152,     0,     0,     0,     0,     0,   134,     0,     0,
       0,     0,   145,   110,   229,   229,   113,     0,     0,     0,
       0,   229,   229,   124,   131,   232,   232,   179,   169,     0,
     229,   229,   232,   232,   165,     0,   131,   350,     0,     0,
      42,    41,    87,     0,    98,    96,     0,     0,   251,     0,
     235,     0,     0,     0,   233,     0,   246,   250,   238,     0,
       0,    12,   193,   194,     0,     0,   202,     0,   251,     0,
       0,   276,   277,   369,   360,   364,     0,   368,   276,   330,
     352,   333,    79,     0,   296,   302,   297,   274,   299,   309,
      74,   329,     0,   327,     0,   351,   325,     0,     0,   110,
     131,     0,     0,     0,     0,   251,   251,   232,   232,    29,
      30,   274,     0,     0,     0,    71,   110,     0,   264,     0,
       0,     0,   258,   260,     0,     0,   163,   161,     0,   153,
     154,   157,   158,   156,   159,     0,   232,   232,   114,     0,
     229,   229,   232,   232,     0,     0,     0,   171,     0,     0,
       0,   232,   232,   173,   174,   181,     0,   168,     0,     0,
       0,    44,     0,     0,    94,   251,   211,   239,   237,     0,
     242,     0,     0,     0,   195,   252,     0,     0,     0,     0,
       0,   106,   199,     0,     0,   375,     0,   366,   274,   274,
       0,     0,   276,   357,     0,     0,   276,     0,   301,     0,
     328,   326,   324,    59,    62,     0,   251,     0,     0,   251,
       0,   215,   217,     0,     0,    32,     0,   251,     0,     0,
     262,   261,     0,   263,    91,    95,   162,   102,     0,     0,
       0,   232,   232,     0,     0,     0,     0,   116,     0,   100,
       0,     0,   178,   189,     0,     0,    48,    46,     0,    45,
       0,     0,    84,   207,    12,     0,   236,   249,   248,   247,
     240,     0,     0,   198,     0,   251,     0,    12,   276,   276,
     370,   363,   362,   361,   359,   367,   365,     0,   334,   304,
       0,   298,    58,   219,     0,     0,   225,     0,    12,    12,
     251,     0,    20,    27,     0,   104,   265,   117,     0,     0,
       0,     0,   119,   120,   125,   126,   172,   170,     0,     0,
     184,   187,     0,     0,   181,     0,    43,     0,     0,    85,
      12,     0,   241,   253,   254,   251,   213,   196,     0,   374,
     373,   335,   276,    12,   251,   251,    12,   251,     0,     0,
     203,     0,   251,     0,   115,   121,     0,     0,   177,   175,
     189,     0,     0,   189,    47,    49,   131,   131,     0,   212,
     209,    12,   200,   303,     0,   227,   223,     0,   221,   216,
     218,    12,   251,    28,   118,     0,   123,     0,   186,   180,
     181,   182,    92,    86,   208,    12,     0,   220,    12,    12,
     226,    12,     0,   205,   122,   128,   127,   179,   189,     0,
     214,     0,     0,     0,   204,    12,     0,   183,   210,   228,
     224,   222,     0,   188,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -874,  -874,  -874,  -874,  -874,   -65,  1026,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  1027,   -33,  -342,  -874,  -874,   -56,
    1020,   -41,  1032,    54,   -70,   -26,  -275,  -575,   -64,   305,
    -263,   -78,  -874,  -874,  -874,   -83,   688,  -278,  -874,  -874,
    -874,  -874,  -211,  -874,   512,  -297,  -268,  -257,  -505,  -874,
    -293,  -874,  -874,  -873,  -874,  -748,   323,  1033,   -34,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,   669,   224,   550,   782,
    -580,  -593,  -133,  -874,   685,   454,   853,    85,  -874,   -72,
    -874,  -874,  -335,  -874,  -184,   645,  -874,  -874,   625,  -874,
     -21,  -874,  -874,   -23,  -181,   775,  -874,   -15,  1094,  -874,
    -874,  -874,  -874,  -874,  -874,  -874,   525,  -525,   523,  -874,
    -874,  -874,  -874,  -874,   460,  1138,   -98,  -146,  -111,  -874,
     908
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    32,     5,     6,    27,     7,   137,
     255,   582,   722,    37,    39,    40,   504,   858,   649,   472,
      54,    55,    41,    42,   103,   152,   447,   448,   105,   181,
     371,   182,   929,   967,   510,   536,   290,   309,   487,   755,
     957,   183,   184,   185,   186,   683,   474,   475,   317,   318,
     319,   646,  1007,   853,   501,   920,   921,    45,    46,    28,
     671,   877,   350,   981,  1015,   930,   995,   864,   971,   898,
     899,   943,  1001,   999,   946,   998,    77,   191,   341,   342,
     668,   528,   292,   457,   458,   301,   302,   320,    29,    50,
     198,    51,   540,   541,   106,   107,   108,   554,   555,   109,
     153,   111,   112,   154,   367,   368,   114,   155,   388,   116,
     117,   118,   119,   544,   800,   684,   685,   922,   687,   120,
     121,   122,   123,   206,   370,   157,   158,   159,   160,   261,
     262
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     136,   179,   193,   372,   529,   656,    72,   180,   473,   476,
     179,    78,   450,   127,   352,   178,   180,   202,   725,   686,
     494,   208,   542,   733,   178,   104,   545,   215,   113,   256,
     110,   513,   293,   478,   264,   665,   115,    85,   378,   669,
     466,   867,   345,   257,   258,   385,    33,   238,   986,   267,
     525,   963,   785,   797,   525,   179,    11,     8,   511,   512,
      85,   180,   499,   333,   798,   776,   601,    10,   204,   178,
     770,    79,   514,   515,   516,   517,   205,   876,   310,   777,
      35,   778,   500,   780,    36,   792,    86,   268,   394,   311,
     799,   346,   372,   269,   270,   271,   272,   273,   274,   179,
     819,    34,   196,    47,    48,   180,   666,   670,   786,   914,
     868,   178,   178,   559,   347,   526,    64,  1008,   373,   374,
     987,   569,   821,   822,   787,   102,   275,   602,   187,   313,
      47,   771,   759,   314,   433,   435,   437,   439,   441,   442,
     443,   335,   315,   316,   179,   670,   288,   830,   289,   831,
     180,   833,   213,    68,    12,   303,   820,   307,   178,   336,
     188,   648,   190,   479,     1,   372,   337,   194,   480,    13,
     338,     2,   104,   612,   481,   113,   444,   110,   428,   429,
     430,   431,   863,   115,   214,   104,   104,   451,   113,   113,
     110,   110,    73,   308,   558,   492,   115,   115,   369,   339,
     610,   611,   310,   214,   369,   452,   644,    69,   626,   235,
     340,   369,   453,   311,   482,   991,   454,    38,   483,   179,
      43,   357,   627,   893,    81,   180,   896,   484,   485,   486,
     335,   303,    56,   179,   903,  -270,   451,  -270,   179,   180,
     806,   493,   677,    57,   180,   455,    74,   178,   336,   579,
     395,   401,   550,   313,   452,   337,   456,   314,   310,   338,
    1017,   453,  -270,   358,  -270,   454,   315,   316,   628,   311,
      70,   915,   686,   640,   307,   343,    44,    71,    79,   214,
     506,   179,   936,    58,   807,   533,   587,   180,   588,   402,
     589,   551,   590,   765,   591,    85,   653,   312,   655,   340,
     768,   553,   556,    74,   213,   456,   179,   950,   197,   313,
     396,   641,   180,   314,   507,    85,    85,   738,   303,   298,
     178,   360,   315,   316,   398,   766,   179,    49,   735,   782,
     405,   406,   180,    59,   355,   204,   214,    47,   102,   692,
      52,   694,   970,   543,   479,   745,   795,   739,   740,   595,
     446,   975,   976,   803,   978,   481,    60,   682,    53,   983,
     369,   741,   742,   743,   744,    63,   214,   695,   479,   102,
      85,    65,   127,   835,    72,    85,   459,    78,   605,   481,
     702,   704,   679,   534,   606,   707,   680,    66,   660,  1003,
     662,   179,   179,   696,   488,   482,   698,   180,   180,   483,
     854,   891,   102,   348,   697,    67,   178,   268,   484,   485,
     486,    79,   855,   269,   270,   271,   272,   273,   274,   482,
     520,    34,   522,   483,    61,   524,   856,   857,   652,    75,
     654,   745,   484,   485,   486,    62,   179,   179,   179,   179,
      76,   131,   180,   180,   180,   180,   275,   393,   829,    82,
     178,   178,   178,   178,   276,    79,   126,   179,   326,   327,
      83,   277,   278,   180,   728,   128,   730,   886,   615,   616,
     328,   889,   329,   330,   299,   570,    95,   618,   619,   622,
     179,  -144,  -144,    79,   331,   279,   180,    97,   150,   620,
     708,  -144,  -144,  -144,   178,   322,    47,   129,   179,   880,
     615,   616,  1016,  -144,   180,   332,    79,   102,   719,   720,
     621,   736,   178,   135,  -144,   104,   615,   616,   113,   323,
     110,   324,    30,   179,   325,   130,   115,    31,   796,   180,
     156,   369,   577,   179,   629,   488,   596,   132,   598,   180,
      79,   600,   859,   939,   940,   630,   369,   369,   615,   616,
     203,   369,   207,   860,   209,   210,   756,   578,   519,   836,
     216,   861,   615,   616,   179,    79,   190,   609,   767,   479,
     180,   617,   133,   532,   837,   550,   179,   464,   178,   700,
     481,   546,   180,   964,   465,   965,   134,   550,   547,   179,
     178,   179,   790,   179,   449,   180,  -317,   180,  -317,   180,
      79,   219,    80,   260,   263,   179,   788,   973,   268,   189,
     343,   180,   192,   449,   269,   270,   271,   272,   273,   274,
     482,   124,   195,   546,   483,   809,   125,   575,   576,   222,
     560,   362,   815,   484,   485,   486,   546,   593,   593,   201,
     179,   546,   211,   565,   594,   623,   180,   275,   699,   825,
     223,   593,   224,   225,   178,   226,   535,   179,   709,   179,
     217,   179,   546,   180,   470,   180,   479,   180,   220,   810,
     221,   905,   171,   172,   173,   174,   471,   481,   415,   296,
     297,   546,   380,   416,   463,   383,   459,   386,   811,   306,
     383,   389,   390,   391,   392,   218,   866,   546,   925,   138,
     464,   449,   179,   139,   812,   926,   874,   962,   180,   179,
     171,   172,   173,   174,   295,   180,   178,   482,    84,   179,
    -315,   483,  -315,   563,   563,   180,   882,   883,   564,   567,
     484,   485,   486,   239,   894,   895,   265,   897,   224,   225,
     199,   226,   657,   240,   200,   904,   613,   179,   179,   906,
     179,   614,   241,   180,   180,   674,   180,   356,   179,   288,
     242,   289,   179,   268,   180,   916,   535,   243,   180,   269,
     270,   271,   272,   273,   274,   772,   179,    79,   179,   502,
     773,   505,   180,   268,   180,   244,    79,   414,   527,   269,
     270,   271,   272,   273,   274,   711,   712,   713,   714,   931,
     353,  1005,   275,   245,   354,  1006,   505,   723,   724,  -316,
     247,  -316,   938,   537,   538,   539,   246,    79,   951,   603,
     488,    79,   275,   634,   266,   953,   229,   230,   615,   616,
     488,   179,   667,   948,   949,   958,   959,   180,   179,   343,
     266,   219,   229,   548,   180,    79,   287,   659,   179,   179,
      79,    79,   661,   663,   180,   180,   364,   365,   334,   757,
     758,    79,   291,   727,   349,   968,   763,   764,    79,    79,
     729,   731,   294,   985,   585,   260,   300,   801,   974,   802,
     344,   977,   280,   281,   282,   801,   179,   961,   992,   993,
     321,   351,   180,   375,   283,   284,   179,   179,   449,   248,
     376,   249,   180,   180,   250,    87,   996,   445,   667,   379,
     178,   178,   403,   404,   488,   285,  1002,   251,   381,   387,
     286,    88,    89,    90,   252,  -350,    91,    92,   253,   397,
    1009,   254,   399,  1011,  1012,   400,  1013,   410,   412,   418,
      93,   823,   824,    94,   419,   460,  -350,   212,  -350,  -350,
    1022,   213,   462,   461,   467,  -350,  -350,  -350,  -350,   477,
     489,   490,   491,   650,   667,   495,   667,    95,   667,   498,
     838,   839,   496,  -350,   497,    96,   843,   844,    97,    98,
     535,   508,   509,   214,   521,   851,   852,    99,   523,   530,
     100,   101,   552,   561,  -350,   568,  -350,  -350,   102,   213,
     562,   566,   518,  -350,  -350,  -350,  -350,   592,   690,   691,
     637,   651,   658,   672,   597,   673,   599,   531,    14,    15,
     675,    16,   604,   705,    87,   580,   607,   624,   625,   676,
     678,   214,   667,   631,   667,   632,   667,    17,   633,   638,
      88,    89,    90,   639,    18,    91,    92,    19,    20,   581,
     645,    22,   693,   715,   167,   168,   688,   710,   716,    93,
     721,   288,    94,   734,   170,   910,   911,   737,   571,   572,
     573,   574,   171,   172,   173,   174,   175,   748,   749,   583,
     754,   584,   750,   774,   535,   760,    95,   769,   775,   751,
     779,   781,    23,   783,   535,   102,   784,    97,   363,   789,
     791,    24,    25,   813,   814,    26,    99,   816,   817,   100,
     101,   818,    14,    15,    85,    16,   826,   102,   827,   828,
     832,   834,   535,   535,   846,   535,   840,   847,   848,   850,
     865,    17,   849,   535,   670,   862,   873,   535,    18,   793,
     794,    19,    20,    21,   871,    22,   869,   161,   872,   875,
    -345,   449,   884,   535,   890,   892,   900,   901,   635,   636,
     902,   162,   163,   164,   907,   642,   643,   909,   908,   912,
     913,  -345,   165,  -345,  -345,   166,   562,   918,   919,   927,
    -345,  -345,  -345,  -345,   924,   917,    23,   928,   933,   934,
     167,   168,   942,   935,   937,    24,    25,   169,   944,    26,
     170,   945,   947,   952,   681,   954,   535,   955,   171,   172,
     173,   174,   175,   535,   956,   960,   176,   969,   972,   177,
     966,   227,   979,   535,   535,   980,   982,   989,   505,   505,
     268,   102,   984,   990,   236,   994,   269,   270,   271,   272,
     273,   274,   228,   870,   229,   230,   717,   718,   162,   163,
     164,   231,   232,   233,   234,   997,  1000,  1004,  1010,   165,
    1014,   535,   166,  1018,   887,   888,  1019,  1023,  1020,   275,
     468,   469,  1021,  1024,   413,   407,   409,   167,   168,   878,
     470,   408,   726,   988,   169,   411,   732,   170,   171,   172,
     173,   174,   471,   746,   747,   171,   172,   173,   174,   175,
     752,   753,   879,   237,   845,   268,   177,   664,   923,   761,
     762,   269,   270,   271,   272,   273,   274,   608,   102,   382,
     808,   689,   881,   586,   885,   932,   359,     0,   268,     0,
       0,     0,     0,     0,   269,   270,   271,   272,   273,   274,
     268,     0,     0,     0,   275,   140,   269,   270,   271,   272,
     273,   274,     0,     0,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   259,   144,   275,     0,   145,
     140,     0,     0,     0,   146,   147,   148,   149,   150,   275,
       0,   141,   142,     0,     0,   505,     0,   505,     0,   143,
     151,   144,   366,     0,   145,     0,     0,   102,     0,   146,
     147,   148,   149,   150,     0,   140,   420,   421,   422,   423,
     424,   425,   426,   427,     0,   151,   141,   142,     0,   841,
     842,     0,   102,     0,   143,     0,   144,   377,     0,   145,
     140,     0,     0,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   384,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   557,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   701,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,   703,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   144,   706,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   144,     0,     0,   145,
     140,     0,   102,     0,   146,   147,   148,   149,   150,     0,
       0,   141,   142,     0,     0,     0,     0,     0,     0,   143,
     151,   503,     0,     0,   145,   140,     0,   102,     0,   146,
     147,   148,   149,   150,     0,     0,   141,   142,     0,     0,
       0,     0,     0,     0,   143,   151,   503,     0,     0,   145,
       0,     0,   102,     0,   146,   147,   148,   149,   647,     0,
       0,   -50,   -50,     0,     0,   303,   140,     0,     0,     0,
     151,   -50,   -50,   -50,     0,     0,   304,   102,   432,   305,
       0,   306,     0,   -50,     0,   143,     0,   144,     0,     0,
     145,   140,     0,     0,   -50,   146,   147,   148,   149,   150,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
     143,   151,   144,   214,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,   143,   151,   144,     0,     0,
     145,   140,     0,   102,     0,   146,   147,   148,   149,   150,
       0,     0,     0,   438,     0,     0,     0,     0,     0,     0,
     143,   151,   144,     0,     0,   145,   140,     0,   102,     0,
     146,   147,   148,   149,   150,     0,     0,     0,   440,     0,
       0,     0,     0,   140,     0,   143,   151,   144,     0,     0,
     145,     0,     0,   102,     0,   146,   147,   148,   149,   150,
       0,  -343,   143,     0,   144,     0,     0,   145,     0,     0,
       0,   151,   146,   147,   148,   149,   150,     0,   102,     0,
       0,   361,  -343,     0,  -343,  -343,     0,   566,   151,     0,
     268,  -343,  -343,  -343,  -343,   102,   269,   270,   271,   272,
     273,   274,   268,     0,     0,     0,     0,     0,   269,   270,
     271,   272,   273,   274,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   268,   417,     0,     0,     0,   275,
     269,   270,   271,   272,   273,   274,     0,   268,     0,     0,
       0,   275,   549,   269,   270,   271,   272,   273,   274,   268,
       0,     0,     0,     0,     0,   269,   270,   271,   272,   273,
     274,   804,   268,   275,     0,     0,     0,   805,   269,   270,
     271,   272,   273,   274,     0,     0,   275,     0,     0,     0,
       0,     0,     0,   941,   268,     0,     0,     0,   275,     0,
     269,   270,   271,   272,   273,   274,     0,     0,     0,     0,
       0,   275,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   275
};

static const yytype_int16 yycheck[] =
{
      65,    71,    80,   214,   346,   510,    39,    71,   305,   306,
      80,    45,   290,    54,   198,    71,    80,    89,   593,   544,
     313,    93,   357,   603,    80,    51,   361,    99,    51,   140,
      51,   328,   165,   308,   145,     6,    51,     5,   219,     9,
     303,     8,     5,   141,   142,   226,     5,   125,     5,     5,
       5,   924,     5,     5,     5,   125,     2,    77,   326,   327,
       5,   125,     5,    51,    16,   658,     5,    77,    91,   125,
       5,    59,   329,   330,   331,   332,    91,     9,    26,   659,
      73,   661,    25,   663,    77,   678,    31,    43,     5,    37,
      42,    54,   303,    49,    50,    51,    52,    53,    54,   169,
       5,    60,     5,    18,    19,   169,    77,    77,    61,    64,
      77,   167,   168,   376,    77,    66,    31,   990,    86,    87,
      77,   396,   715,   716,    77,    96,    82,    66,    74,    77,
      45,    66,   637,    81,   280,   281,   282,   283,   284,   285,
     286,     8,    90,    91,   214,    77,    63,   727,    65,   729,
     214,   731,    65,     5,     0,    49,    61,    29,   214,    26,
      75,   503,    65,    26,     4,   376,    33,    82,    31,    60,
      37,    11,   198,   470,    37,   198,   287,   198,   276,   277,
     278,   279,   775,   198,    97,   211,   212,     8,   211,   212,
     211,   212,     5,    65,   375,    19,   211,   212,   213,    66,
     468,   469,    26,    97,   219,    26,   499,    59,     5,   124,
      77,   226,    33,    37,    77,   963,    37,    77,    81,   289,
      77,    17,    19,   816,     5,   289,   819,    90,    91,    92,
       8,    49,    26,   303,   827,    59,     8,    61,   308,   303,
      17,    65,    60,    37,   308,    66,    59,   303,    26,     5,
       5,    39,    59,    77,    26,    33,    77,    81,    26,    37,
    1008,    33,    59,    59,    61,    37,    90,    91,    65,    37,
      54,   846,   797,    39,    29,   190,    77,    61,    59,    97,
      29,   351,   875,    77,    61,   350,   432,   351,   434,    77,
     436,    98,   438,    29,   440,     5,   507,    65,   509,    77,
      61,   373,   374,    59,    65,    77,   376,   900,    84,    77,
      65,    77,   376,    81,    63,     5,     5,   614,    49,    77,
     376,    31,    90,    91,   239,    61,   396,    77,   606,    60,
     245,   246,   396,    77,    23,   358,    97,   252,    96,   550,
      77,    31,   935,   358,    26,   623,   681,   615,   616,    31,
      77,   944,   945,   688,   947,    37,    77,   541,    95,   952,
     375,   618,   619,   620,   621,    77,    97,     5,    26,    96,
       5,     5,   413,    31,   407,     5,   291,   411,   461,    37,
     561,   562,    18,    77,   462,   566,    22,    12,   521,   982,
     523,   461,   462,    31,   309,    77,    31,   461,   462,    81,
      65,    31,    96,    51,    42,    77,   462,    43,    90,    91,
      92,    59,    77,    49,    50,    51,    52,    53,    54,    77,
     335,    60,   337,    81,    26,   340,   768,   769,   506,    49,
     508,   709,    90,    91,    92,    37,   506,   507,   508,   509,
      60,    60,   506,   507,   508,   509,    82,    51,   726,    49,
     506,   507,   508,   509,    48,    59,    77,   527,    55,    56,
      60,    55,    56,   527,   597,    77,   599,   802,    55,    56,
      67,   806,    45,    46,   169,    51,    65,    45,    46,    66,
     550,    45,    46,    59,    57,    79,   550,    76,    77,    57,
     568,    55,    56,    57,   550,     6,   411,    77,   568,   796,
      55,    56,  1007,    67,   568,    78,    59,    96,   580,   581,
      78,    66,   568,    66,    78,   541,    55,    56,   541,    30,
     541,    32,    60,   593,    35,    77,   541,    65,    67,   593,
      70,   546,    51,   603,    26,   450,   451,    60,   453,   603,
      59,   456,    66,   878,   879,    37,   561,   562,    55,    56,
      90,   566,    92,    77,    94,    95,   634,    51,   334,    66,
     100,   772,    55,    56,   634,    59,    65,    51,   646,    26,
     634,    64,    77,   349,    31,    59,   646,    59,   634,    51,
      37,    59,   646,   925,    66,   927,    77,    59,    66,   659,
     646,   661,   675,   663,   289,   659,    60,   661,    62,   663,
      59,    65,    61,   143,   144,   675,   671,   942,    43,    77,
     525,   675,    77,   308,    49,    50,    51,    52,    53,    54,
      77,    49,    77,    59,    81,   697,    54,   403,   404,    39,
      66,    66,   710,    90,    91,    92,    59,    59,    59,    77,
     710,    59,    61,    66,    66,    66,   710,    82,    66,   721,
      60,    59,    62,    63,   710,    65,   351,   727,    66,   729,
      77,   731,    59,   727,    65,   729,    26,   731,    60,    66,
      62,    31,    73,    74,    75,    76,    77,    37,    59,   167,
     168,    59,   222,    64,    60,   225,   601,   227,    66,    65,
     230,   231,   232,   233,   234,    77,   779,    59,    59,    73,
      59,   396,   772,    77,    66,    66,   789,    66,   772,   779,
      73,    74,    75,    76,    77,   779,   772,    77,    49,   789,
      60,    81,    62,    59,    59,   789,   798,   799,    64,    64,
      90,    91,    92,    49,   817,   818,    60,   820,    62,    63,
      73,    65,   518,    60,    77,   828,    60,   817,   818,   832,
     820,    65,    60,   817,   818,   531,   820,    17,   828,    63,
      60,    65,   832,    43,   828,   848,   461,    77,   832,    49,
      50,    51,    52,    53,    54,    59,   846,    59,   848,    61,
      64,   321,   846,    43,   848,    77,    59,    67,    61,    49,
      50,    51,    52,    53,    54,   571,   572,   573,   574,   864,
      39,    73,    82,    49,    43,    77,   346,   583,   584,    60,
      60,    62,   877,   353,   354,   355,    49,    59,   901,    61,
     735,    59,    82,    61,    60,   908,    62,    63,    55,    56,
     745,   901,   527,   898,   899,   918,   919,   901,   908,   754,
      60,    65,    62,    63,   908,    59,    80,    61,   918,   919,
      59,    59,    61,    61,   918,   919,   211,   212,   189,   635,
     636,    59,    65,    61,   195,   930,   642,   643,    59,    59,
      61,    61,    77,   956,   414,   415,    77,    59,   943,    61,
       5,   946,    45,    46,    47,    59,   956,    61,   966,   967,
      54,    61,   956,    65,    57,    58,   966,   967,   593,     8,
      49,    10,   966,   967,    13,     3,   971,    76,   603,    77,
     966,   967,   243,   244,   829,    78,   981,    26,    77,    77,
      83,    19,    20,    21,    33,    39,    24,    25,    37,     5,
     995,    40,    77,   998,   999,    77,  1001,    77,    77,    77,
      38,   717,   718,    41,    77,    29,    60,    61,    62,    63,
    1015,    65,    19,    61,    77,    69,    70,    71,    72,    77,
      77,    77,    77,   503,   659,    77,   661,    65,   663,    31,
     746,   747,    77,    39,    77,    73,   752,   753,    76,    77,
     675,    29,    29,    97,    77,   761,   762,    85,    77,    77,
      88,    89,    93,    65,    60,    54,    62,    63,    96,    65,
      65,    65,   333,    69,    70,    71,    72,    64,   548,   549,
      66,     5,     5,     5,    77,     5,    77,   348,     7,     8,
      61,    10,    77,   563,     3,    14,    77,    77,    77,     5,
       5,    97,   727,    77,   729,    77,   731,    26,    77,    77,
      19,    20,    21,    77,    33,    24,    25,    36,    37,    38,
      77,    40,    77,     5,    55,    56,    42,    54,     5,    38,
      15,    63,    41,    29,    65,   841,   842,    77,   399,   400,
     401,   402,    73,    74,    75,    76,    77,    77,    77,   410,
      63,   412,    77,    31,   779,    66,    65,    59,     5,    77,
      61,    29,    81,    54,   789,    96,     5,    76,    77,    61,
      77,    90,    91,     5,     5,    94,    85,     5,    61,    88,
      89,    61,     7,     8,     5,    10,    31,    96,     5,    61,
      61,    77,   817,   818,    61,   820,    66,     5,    61,    19,
      54,    26,    31,   828,    77,    29,     5,   832,    33,   679,
     680,    36,    37,    38,    76,    40,    77,    12,    76,     5,
      39,   846,    31,   848,    77,     5,     5,    61,   489,   490,
      60,    26,    27,    28,     5,   496,   497,    19,    61,     5,
       5,    60,    37,    62,    63,    40,    65,    61,    61,    61,
      69,    70,    71,    72,    29,    77,    81,    64,     5,     5,
      55,    56,    17,     5,     5,    90,    91,    62,     5,    94,
      65,     5,     5,     5,    17,    77,   901,     5,    73,    74,
      75,    76,    77,   908,    61,     5,    81,     5,     5,    84,
      29,    39,     5,   918,   919,     5,     5,    65,   768,   769,
      43,    96,     5,    29,    12,     5,    49,    50,    51,    52,
      53,    54,    60,   783,    62,    63,   577,   578,    26,    27,
      28,    69,    70,    71,    72,     5,     5,     5,     5,    37,
       5,   956,    40,     5,   804,   805,     5,    66,     5,    82,
      55,    56,     5,     5,   254,   248,   250,    55,    56,    17,
      65,   249,   594,   960,    62,   252,   601,    65,    73,    74,
      75,    76,    77,   624,   625,    73,    74,    75,    76,    77,
     631,   632,    17,    81,   754,    43,    84,   525,   854,   640,
     641,    49,    50,    51,    52,    53,    54,   464,    96,   225,
     695,   546,   797,   415,   801,   865,    29,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,    54,
      43,    -1,    -1,    -1,    82,    44,    49,    50,    51,    52,
      53,    54,    -1,    -1,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    82,    -1,    68,
      44,    -1,    -1,    -1,    73,    74,    75,    76,    77,    82,
      -1,    55,    56,    -1,    -1,   925,    -1,   927,    -1,    63,
      89,    65,    66,    -1,    68,    -1,    -1,    96,    -1,    73,
      74,    75,    76,    77,    -1,    44,   268,   269,   270,   271,
     272,   273,   274,   275,    -1,    89,    55,    56,    -1,   750,
     751,    -1,    96,    -1,    63,    -1,    65,    66,    -1,    68,
      44,    -1,    -1,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      89,    65,    66,    -1,    68,    44,    -1,    96,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    89,    65,    66,    -1,    68,
      44,    -1,    96,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      89,    65,    66,    -1,    68,    44,    -1,    96,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    89,    65,    66,    -1,    68,
      44,    -1,    96,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      89,    65,    66,    -1,    68,    44,    -1,    96,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    89,    65,    -1,    -1,    68,
      44,    -1,    96,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      89,    65,    -1,    -1,    68,    44,    -1,    96,    -1,    73,
      74,    75,    76,    77,    -1,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    89,    65,    -1,    -1,    68,
      -1,    -1,    96,    -1,    73,    74,    75,    76,    77,    -1,
      -1,    45,    46,    -1,    -1,    49,    44,    -1,    -1,    -1,
      89,    55,    56,    57,    -1,    -1,    60,    96,    56,    63,
      -1,    65,    -1,    67,    -1,    63,    -1,    65,    -1,    -1,
      68,    44,    -1,    -1,    78,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    89,    65,    97,    -1,    68,    44,    -1,    96,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    89,    65,    -1,    -1,
      68,    44,    -1,    96,    -1,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    89,    65,    -1,    -1,    68,    44,    -1,    96,    -1,
      73,    74,    75,    76,    77,    -1,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    44,    -1,    63,    89,    65,    -1,    -1,
      68,    -1,    -1,    96,    -1,    73,    74,    75,    76,    77,
      -1,    39,    63,    -1,    65,    -1,    -1,    68,    -1,    -1,
      -1,    89,    73,    74,    75,    76,    77,    -1,    96,    -1,
      -1,    34,    60,    -1,    62,    63,    -1,    65,    89,    -1,
      43,    69,    70,    71,    72,    96,    49,    50,    51,    52,
      53,    54,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    66,    -1,    -1,    -1,    82,
      49,    50,    51,    52,    53,    54,    -1,    43,    -1,    -1,
      -1,    82,    61,    49,    50,    51,    52,    53,    54,    43,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    67,    43,    82,    -1,    -1,    -1,    61,    49,    50,
      51,    52,    53,    54,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    43,    -1,    -1,    -1,    82,    -1,
      49,    50,    51,    52,    53,    54,    -1,    -1,    -1,    -1,
      -1,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    82
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,   100,   101,   104,   105,   107,    77,   102,
      77,   122,     0,    60,     7,     8,    10,    26,    33,    36,
      37,    38,    40,    81,    90,    91,    94,   106,   158,   187,
      60,    65,   103,     5,    60,    73,    77,   112,    77,   113,
     114,   121,   122,    77,    77,   156,   157,   186,   186,    77,
     188,   190,    77,    95,   119,   120,    26,    37,    77,    77,
      77,    26,    37,    77,   186,     5,    12,    77,     5,    59,
      54,    61,   114,     5,    59,    49,    60,   175,   157,    59,
      61,     5,    49,    60,   175,     5,    31,     3,    19,    20,
      21,    24,    25,    38,    41,    65,    73,    76,    77,    85,
      88,    89,    96,   123,   124,   127,   193,   194,   195,   198,
     199,   200,   201,   202,   205,   206,   208,   209,   210,   211,
     218,   219,   220,   221,    49,    54,    77,   120,    77,    77,
      77,    60,    60,    77,    77,    66,   104,   108,    73,    77,
      44,    55,    56,    63,    65,    68,    73,    74,    75,    76,
      77,    89,   124,   199,   202,   206,   223,   224,   225,   226,
     227,    12,    26,    27,    28,    37,    40,    55,    56,    62,
      65,    73,    74,    75,    76,    77,    81,    84,   118,   123,
     127,   128,   130,   140,   141,   142,   143,   122,   186,    77,
      65,   176,    77,   130,   186,    77,     5,   176,   189,    73,
      77,    77,   188,   223,   202,   206,   222,   223,   188,   223,
     223,    61,    61,    65,    97,   188,   223,    77,    77,    65,
      60,    62,    39,    60,    62,    63,    65,    39,    60,    62,
      63,    69,    70,    71,    72,   186,    12,    81,   130,    49,
      60,    60,    60,    77,    77,    49,    49,    60,     8,    10,
      13,    26,    33,    37,    40,   109,   227,   225,   225,    64,
     223,   228,   229,   223,   227,    60,    60,     5,    43,    49,
      50,    51,    52,    53,    54,    82,    48,    55,    56,    79,
      45,    46,    47,    57,    58,    78,    83,    80,    63,    65,
     135,    65,   181,   181,    77,    77,   143,   143,    77,   128,
      77,   184,   185,    49,    60,    63,    65,    29,    65,   136,
      26,    37,    65,    77,    81,    90,    91,   147,   148,   149,
     186,    54,     6,    30,    32,    35,    55,    56,    67,    45,
      46,    57,    78,    51,   175,     8,    26,    33,    37,    66,
      77,   177,   178,   186,     5,     5,    54,    77,    51,   175,
     161,    61,   193,    39,    43,    23,    17,    17,    59,    29,
      31,    34,    66,    77,   194,   194,    66,   203,   204,   206,
     223,   129,   141,    86,    87,    65,    49,    66,   203,    77,
     223,    77,   207,   223,    66,   203,   223,    77,   207,   223,
     223,   223,   223,    51,     5,     5,    65,     5,   186,    77,
      77,    39,    77,   175,   175,   186,   186,   113,   121,   105,
      77,   156,    77,   119,    67,    59,    64,    66,    77,    77,
     224,   224,   224,   224,   224,   224,   224,   224,   225,   225,
     225,   225,    56,   226,    56,   226,    56,   226,    56,   226,
      56,   226,   226,   226,   227,    76,    77,   125,   126,   128,
     136,     8,    26,    33,    37,    66,    77,   182,   183,   186,
      29,    61,    19,    60,    59,    66,   129,    77,    55,    56,
      65,    77,   118,   144,   145,   146,   144,    77,   125,    26,
      31,    37,    77,    81,    90,    91,    92,   137,   186,    77,
      77,    77,    19,    65,   149,    77,    77,    77,    31,     5,
      25,   153,    61,    65,   115,   223,    29,    63,    29,    29,
     133,   145,   145,   144,   146,   146,   146,   146,   175,   176,
     186,    77,   186,    77,   186,     5,    66,    61,   180,   115,
      77,   175,   176,   104,    77,   128,   134,   223,   223,   223,
     191,   192,   191,   206,   212,   191,    59,    66,    63,    61,
      59,    98,    93,   188,   196,   197,   188,    66,   203,   129,
      66,    65,    65,    59,    64,    66,    65,    64,    54,   125,
      51,   175,   175,   175,   175,   176,   176,    51,    51,     5,
      14,    38,   110,   175,   175,   223,   229,   226,   226,   226,
     226,   226,    64,    59,    66,    31,   186,    77,   186,    77,
     186,     5,    66,    61,    77,   134,   130,    77,   185,    51,
     145,   145,   144,    60,    65,    55,    56,    64,    45,    46,
      57,    78,    66,    66,    77,    77,     5,    19,    65,    26,
      37,    77,    77,    77,    61,   175,   175,    66,    77,    77,
      39,    77,   175,   175,   149,    77,   150,    77,   115,   117,
     223,     5,   130,   141,   130,   141,   147,   176,     5,    61,
     181,    61,   181,    61,   178,     6,    77,   128,   179,     9,
      77,   159,     5,     5,   176,    61,     5,    60,     5,    18,
      22,    17,   193,   144,   214,   215,   216,   217,    42,   204,
     223,   223,   141,    77,    31,     5,    31,    42,    31,    66,
      51,    66,   203,    66,   203,   223,    66,   203,   130,    66,
      54,   176,   176,   176,   176,     5,     5,   175,   175,   188,
     188,    15,   111,   176,   176,   126,   135,    61,   181,    61,
     181,    61,   183,   179,    29,   136,    66,    77,   144,   145,
     145,   146,   146,   146,   146,   136,   175,   175,    77,    77,
      77,    77,   175,   175,    63,   138,   130,   176,   176,   147,
      66,   175,   175,   176,   176,    29,    61,   130,    61,    59,
       5,    66,    59,    64,    31,     5,   180,   179,   179,    61,
     179,    29,    60,    54,     5,     5,    61,    77,   104,    61,
     134,    77,   180,   223,   223,   191,    67,     5,    16,    42,
     213,    59,    61,   191,    67,    61,    17,    61,   197,   188,
      66,    66,    66,     5,     5,   130,     5,    61,    61,     5,
      61,   180,   180,   176,   176,   188,    31,     5,    61,   136,
     179,   179,    61,   179,    77,    31,    66,    31,   176,   176,
      66,   175,   175,   176,   176,   177,    61,     5,    61,    31,
      19,   176,   176,   152,    65,    77,   115,   115,   116,    66,
      77,   141,    29,   180,   166,    54,   134,     8,    77,    77,
     223,    76,    76,     5,   134,     5,     9,   160,    17,    17,
     144,   215,   188,   188,    31,   217,   191,   223,   223,   191,
      77,    31,     5,   180,   134,   134,   180,   134,   168,   169,
       5,    61,    60,   180,   134,    31,   134,     5,    61,    19,
     176,   176,     5,     5,    64,   126,   134,    77,    61,    61,
     154,   155,   216,   184,    29,    59,    66,    61,    64,   131,
     164,   104,   223,     5,     5,     5,   180,     5,   104,   191,
     191,    64,    17,   170,     5,     5,   173,     5,   104,   104,
     180,   134,     5,   134,    77,     5,    61,   139,   134,   134,
       5,    61,    66,   152,   115,   115,    29,   132,   104,     5,
     180,   167,     5,   191,   104,   180,   180,   104,   180,     5,
       5,   162,     5,   180,     5,   134,     5,    77,   155,    65,
      29,   154,   130,   130,     5,   165,   104,     5,   174,   172,
       5,   171,   104,   180,     5,    73,    77,   151,   152,   104,
       5,   104,   104,   104,     5,   163,   147,   154,     5,     5,
       5,     5,   104,    66,     5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    99,   100,   100,   101,   101,   102,   102,   103,   103,
     104,   105,   105,   106,   106,   106,   106,   106,   106,   106,
     107,   108,   108,   109,   109,   109,   109,   109,   109,   110,
     110,   110,   111,   111,   112,   112,   112,   112,   113,   113,
     114,   114,   115,   115,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   119,   119,   120,   120,   120,
     120,   120,   120,   121,   121,   122,   122,   123,   124,   124,
     125,   125,   126,   126,   127,   128,   128,   128,   129,   129,
     130,   130,   130,   130,   131,   132,   130,   130,   130,   130,
     130,   130,   130,   133,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   134,   134,   134,   135,   135,
     136,   136,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   139,   139,   139,   140,
     140,   140,   141,   141,   141,   141,   141,   141,   142,   142,
     142,   142,   142,   143,   143,   143,   143,   143,   143,   143,
     144,   144,   144,   144,   144,   145,   145,   145,   145,   145,
     146,   146,   146,   146,   147,   148,   148,   150,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     151,   152,   153,   153,   153,   153,   154,   154,   155,   155,
     156,   156,   157,   157,   157,   158,   158,   159,   158,   160,
     158,   161,   158,   162,   158,   163,   158,   164,   158,   165,
     158,   166,   158,   167,   158,   168,   158,   169,   158,   170,
     158,   171,   158,   172,   158,   173,   158,   174,   158,   175,
     176,   176,   176,   177,   177,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   179,   179,   179,   179,
     179,   180,   180,   180,   180,   181,   181,   181,   182,   182,
     183,   183,   183,   183,   183,   183,   184,   184,   185,   186,
     186,   187,   189,   188,   190,   188,   192,   191,   193,   193,
     193,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   195,   195,   195,   195,
     196,   196,   196,   197,   197,   198,   198,   199,   199,   199,
     200,   200,   200,   200,   200,   201,   201,   201,   202,   202,
     202,   202,   202,   202,   202,   202,   202,   202,   202,   202,
     203,   203,   204,   204,   204,   204,   205,   205,   205,   205,
     205,   205,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   207,   207,   208,   209,   209,   210,   210,   212,   211,
     213,   213,   213,   214,   214,   215,   215,   216,   216,   217,
     217,   218,   219,   220,   220,   220,   221,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   226,   226,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   228,   228,   229,   229
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
       1,     0,     3,     1,     4,     3,     1,     3,     1,     2,
       2,     3,     3,     1,     3,     4,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     4,     3,     2,     3,     1,     0,     4,     3,
       6,     4,     6,     4,     4,     7,     2,     7,     5,     0,
       0,     0,     7,     9,     5,     0,     3,     1,     6,     0,
       2,     1,     4,     6,     6,     8,    10,     0,     9,     0,
      11,     0,     7,     0,    13,     0,    15,     0,    12,     0,
      14,     0,    11,     0,    13,     0,    11,     0,    11,     0,
      12,     0,    14,     0,    14,     0,    12,     0,    14,     0,
       3,     2,     0,     3,     1,     3,     5,     4,     3,     4,
       5,     6,     4,     2,     2,     2,     1,     3,     3,     3,
       1,     0,     3,     5,     5,     3,     2,     0,     3,     1,
       3,     4,     4,     4,     3,     5,     3,     1,     1,     3,
       1,     3,     0,     4,     0,     2,     0,     2,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     5,     5,     7,     5,
       1,     3,     2,     6,     4,     1,     2,     2,     4,     5,
       1,     1,     3,     3,     1,     1,     3,     1,     4,     4,
       3,     3,     4,     3,     6,     5,     6,     5,     6,     5,
       3,     1,     1,     3,     5,     6,     3,     3,     3,     3,
       3,     3,     4,     3,     2,     3,     4,     2,     3,     2,
       1,     3,     1,     3,     2,     2,     4,     6,     0,     7,
       0,     2,     2,     3,     1,     3,     0,     3,     1,     1,
       3,     4,     4,     8,     8,     6,     4,     3,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     2,
       2,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       3,     4,     4,     4,     4,     4,     3,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     1,     2,     2,
       2,     3,     1,     3,     1,     3
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
#line 2357 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 240 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2363 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 243 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2369 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 244 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2375 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 247 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2381 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 248 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2387 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 251 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2393 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 252 "pascal.y"
     { (yyval.node) = NULL; }
#line 2399 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 255 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2405 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 258 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2411 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 259 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2417 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 262 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2423 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 263 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2429 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 264 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2435 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 265 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2441 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 266 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2447 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY uses_list SEMICOLON  */
#line 267 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2453 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 268 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2459 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY uses_name SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 271 "pascal.y"
                                                                                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2465 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 274 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2471 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 275 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2477 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY uses_list SEMICOLON  */
#line 278 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2483 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 279 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2489 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 280 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2495 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 281 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2501 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives  */
#line 282 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2507 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives  */
#line 283 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2513 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 286 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2519 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 287 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2525 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 288 "pascal.y"
     { (yyval.node) = NULL; }
#line 2531 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 291 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2537 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 292 "pascal.y"
     { (yyval.node) = NULL; }
#line 2543 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 295 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2549 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 296 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2555 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 297 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2561 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 298 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2567 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 301 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2573 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 302 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2579 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 305 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2585 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 306 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2591 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 309 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2597 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 310 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2603 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 311 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2609 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 312 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2615 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 315 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2621 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 316 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2627 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 319 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2633 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 320 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2639 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 323 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2645 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 324 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2651 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 325 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2657 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 326 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2663 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 327 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2669 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 330 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2675 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 331 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2681 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 334 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2687 "pascal.tab.c"
    break;

  case 58: /* type_decl: GENERICTSY IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 335 "pascal.y"
                                                            { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2693 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT LTOP id_list GTOP EQOP type SEMICOLON  */
#line 336 "pascal.y"
                                                 { pascal_note_type_decl((yyvsp[-6].node)); (yyval.node) = pt_add(pt_decl("gtype", (yyvsp[-6].node), (yyvsp[-1].node)), (yyvsp[-4].node)); }
#line 2699 "pascal.tab.c"
    break;

  case 60: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 337 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2705 "pascal.tab.c"
    break;

  case 61: /* type_decl: IDENT EQOP INTERFACESY SEMICOLON  */
#line 338 "pascal.y"
                                      { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), "forward")); }
#line 2711 "pascal.tab.c"
    break;

  case 62: /* type_decl: IDENT EQOP CLASSSY LPARENT class_parents RPARENT SEMICOLON  */
#line 339 "pascal.y"
                                                                { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2717 "pascal.tab.c"
    break;

  case 63: /* uses_list: uses_name  */
#line 342 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2723 "pascal.tab.c"
    break;

  case 64: /* uses_list: uses_list COMMA uses_name  */
#line 343 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2729 "pascal.tab.c"
    break;

  case 65: /* uses_name: IDENT  */
#line 346 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2735 "pascal.tab.c"
    break;

  case 66: /* uses_name: uses_name PERIOD IDENT  */
#line 347 "pascal.y"
                            { (yyval.node) = pt_dot((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2741 "pascal.tab.c"
    break;

  case 67: /* spec_gen: IDENT GENLT type_arg_list GENGT  */
#line 350 "pascal.y"
                                    { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2747 "pascal.tab.c"
    break;

  case 68: /* spec_expr: spec_kw  */
#line 353 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2753 "pascal.tab.c"
    break;

  case 69: /* spec_expr: spec_gen  */
#line 354 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2759 "pascal.tab.c"
    break;

  case 70: /* class_parents: parent_item  */
#line 357 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2765 "pascal.tab.c"
    break;

  case 71: /* class_parents: class_parents COMMA parent_item  */
#line 358 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2771 "pascal.tab.c"
    break;

  case 72: /* parent_item: IDENT  */
#line 361 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2777 "pascal.tab.c"
    break;

  case 73: /* parent_item: spec_ref  */
#line 362 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2783 "pascal.tab.c"
    break;

  case 74: /* spec_kw: SPECIALIZESY IDENT LTOP type_arg_list GTOP  */
#line 365 "pascal.y"
                                               { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2789 "pascal.tab.c"
    break;

  case 75: /* spec_ref: spec_kw  */
#line 368 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2795 "pascal.tab.c"
    break;

  case 76: /* spec_ref: spec_gen  */
#line 369 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2801 "pascal.tab.c"
    break;

  case 77: /* spec_ref: IDENT LTOP type_arg_list GTOP  */
#line 370 "pascal.y"
                                   { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_spec((yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2807 "pascal.tab.c"
    break;

  case 78: /* type_arg_list: simple_type  */
#line 373 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2813 "pascal.tab.c"
    break;

  case 79: /* type_arg_list: type_arg_list COMMA simple_type  */
#line 374 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2819 "pascal.tab.c"
    break;

  case 80: /* type: simple_type  */
#line 377 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2825 "pascal.tab.c"
    break;

  case 81: /* type: ARROW IDENT  */
#line 378 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2831 "pascal.tab.c"
    break;

  case 82: /* type: CLASSSY OFSY IDENT  */
#line 379 "pascal.y"
                        { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "classref"), (yyvsp[0].node)); }
#line 2837 "pascal.tab.c"
    break;

  case 83: /* type: ARROW spec_ref  */
#line 380 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2843 "pascal.tab.c"
    break;

  case 84: /* $@1: %empty  */
#line 381 "pascal.y"
                                                       { }
#line 2849 "pascal.tab.c"
    break;

  case 85: /* $@2: %empty  */
#line 381 "pascal.y"
                                                           { }
#line 2855 "pascal.tab.c"
    break;

  case 86: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 381 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2861 "pascal.tab.c"
    break;

  case 87: /* type: packed_opt ARRAYSY OFSY type  */
#line 382 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2867 "pascal.tab.c"
    break;

  case 88: /* type: PROCEDURESY pf_params  */
#line 383 "pascal.y"
                           { pascal_dir_arm(); (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2873 "pascal.tab.c"
    break;

  case 89: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 384 "pascal.y"
                                      { pascal_dir_arm(); if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2879 "pascal.tab.c"
    break;

  case 90: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 385 "pascal.y"
                                            { pascal_dir_arm(); pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2885 "pascal.tab.c"
    break;

  case 91: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 386 "pascal.y"
                                                       { pascal_dir_arm(); if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2891 "pascal.tab.c"
    break;

  case 92: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 387 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2897 "pascal.tab.c"
    break;

  case 93: /* $@3: %empty  */
#line 388 "pascal.y"
                         { }
#line 2903 "pascal.tab.c"
    break;

  case 94: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 388 "pascal.y"
                                               { (yyval.node) = pt_record_or_helper((yyvsp[-4].node), (yyvsp[-1].node)); }
#line 2909 "pascal.tab.c"
    break;

  case 95: /* type: TYPESY IDENT FORSY type class_members ENDSY  */
#line 389 "pascal.y"
                                                 { if (!pt_is_helper_word((yyvsp[-4].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_type_helper((yyvsp[-2].node), (yyvsp[-1].node)); }
#line 2915 "pascal.tab.c"
    break;

  case 96: /* type: packed_opt SETSY OFSY simple_type  */
#line 390 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2921 "pascal.tab.c"
    break;

  case 97: /* type: packed_opt FILESY  */
#line 391 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2927 "pascal.tab.c"
    break;

  case 98: /* type: packed_opt FILESY OFSY type  */
#line 392 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2933 "pascal.tab.c"
    break;

  case 99: /* type: OBJECTSY record_body ENDSY  */
#line 393 "pascal.y"
                                { (yyval.node) = pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)); }
#line 2939 "pascal.tab.c"
    break;

  case 100: /* type: OBJECTSY LPARENT IDENT RPARENT record_body ENDSY  */
#line 394 "pascal.y"
                                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-1].node)), pt_add(pt_new(TT_ARGS), (yyvsp[-3].node))); }
#line 2945 "pascal.tab.c"
    break;

  case 101: /* type: CLASSSY class_members ENDSY  */
#line 395 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2951 "pascal.tab.c"
    break;

  case 102: /* type: CLASSSY LPARENT class_parents RPARENT class_members ENDSY  */
#line 396 "pascal.y"
                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2957 "pascal.tab.c"
    break;

  case 103: /* type: INTERFACESY guid_opt class_members ENDSY  */
#line 397 "pascal.y"
                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2963 "pascal.tab.c"
    break;

  case 104: /* type: INTERFACESY LPARENT class_parents RPARENT guid_opt class_members ENDSY  */
#line 398 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), pt_new(TT_KEYWORD)), pt_retag((yyvsp[-4].node), TT_ARGS)), (yyvsp[-2].node)), (yyvsp[-1].node)), "interface"); }
#line 2969 "pascal.tab.c"
    break;

  case 105: /* result_type: IDENT  */
#line 401 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2975 "pascal.tab.c"
    break;

  case 106: /* result_type: IDENT PERIOD IDENT  */
#line 402 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2981 "pascal.tab.c"
    break;

  case 107: /* result_type: spec_ref  */
#line 403 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2987 "pascal.tab.c"
    break;

  case 108: /* guid_opt: %empty  */
#line 406 "pascal.y"
    { (yyval.node) = NULL; }
#line 2993 "pascal.tab.c"
    break;

  case 109: /* guid_opt: LBRACK STRINGCONST RBRACK  */
#line 407 "pascal.y"
                               { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "guid"), (yyvsp[-1].node)); }
#line 2999 "pascal.tab.c"
    break;

  case 110: /* class_members: %empty  */
#line 410 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3005 "pascal.tab.c"
    break;

  case 111: /* class_members: class_members class_member  */
#line 411 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3011 "pascal.tab.c"
    break;

  case 112: /* class_member: IDENT  */
#line 414 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 3017 "pascal.tab.c"
    break;

  case 113: /* class_member: IDENT SEMICOLON  */
#line 415 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 3023 "pascal.tab.c"
    break;

  case 114: /* class_member: IDENT FORSY IDENT  */
#line 416 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3029 "pascal.tab.c"
    break;

  case 115: /* class_member: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 417 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3035 "pascal.tab.c"
    break;

  case 116: /* class_member: id_list COLON type SEMICOLON  */
#line 418 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3041 "pascal.tab.c"
    break;

  case 117: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 419 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3047 "pascal.tab.c"
    break;

  case 118: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 420 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3053 "pascal.tab.c"
    break;

  case 119: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 421 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 3059 "pascal.tab.c"
    break;

  case 120: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 422 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 3065 "pascal.tab.c"
    break;

  case 121: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 423 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3071 "pascal.tab.c"
    break;

  case 122: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 424 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 3077 "pascal.tab.c"
    break;

  case 123: /* class_member: PROPERTYSY IDENT prop_index_opt COLON parent_item prop_specs SEMICOLON  */
#line 425 "pascal.y"
                                                                            { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 3083 "pascal.tab.c"
    break;

  case 124: /* prop_index_opt: %empty  */
#line 428 "pascal.y"
    { (yyval.node) = NULL; }
#line 3089 "pascal.tab.c"
    break;

  case 125: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 429 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 3095 "pascal.tab.c"
    break;

  case 126: /* prop_specs: %empty  */
#line 432 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 3101 "pascal.tab.c"
    break;

  case 127: /* prop_specs: prop_specs IDENT IDENT  */
#line 433 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3107 "pascal.tab.c"
    break;

  case 128: /* prop_specs: prop_specs IDENT INTCONST  */
#line 434 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 3113 "pascal.tab.c"
    break;

  case 129: /* packed_opt: PACKEDSY  */
#line 437 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 3119 "pascal.tab.c"
    break;

  case 130: /* packed_opt: BITPACKEDSY  */
#line 438 "pascal.y"
                 { (yyval.node) = pt_kw("bitpacked"); }
#line 3125 "pascal.tab.c"
    break;

  case 131: /* packed_opt: %empty  */
#line 439 "pascal.y"
     { (yyval.node) = NULL; }
#line 3131 "pascal.tab.c"
    break;

  case 132: /* simple_type: LPARENT enum_list RPARENT  */
#line 442 "pascal.y"
                              { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 3137 "pascal.tab.c"
    break;

  case 133: /* simple_type: IDENT  */
#line 443 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3143 "pascal.tab.c"
    break;

  case 134: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 444 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3149 "pascal.tab.c"
    break;

  case 135: /* simple_type: IDENT PERIOD IDENT  */
#line 445 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3155 "pascal.tab.c"
    break;

  case 136: /* simple_type: spec_ref  */
#line 446 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3161 "pascal.tab.c"
    break;

  case 137: /* simple_type: lo_bound DOTDOT b_expr  */
#line 447 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3167 "pascal.tab.c"
    break;

  case 138: /* lo_bound: lo_term  */
#line 450 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 3173 "pascal.tab.c"
    break;

  case 139: /* lo_bound: PLUS lo_term  */
#line 451 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3179 "pascal.tab.c"
    break;

  case 140: /* lo_bound: MINUS lo_term  */
#line 452 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3185 "pascal.tab.c"
    break;

  case 141: /* lo_bound: lo_bound PLUS b_term  */
#line 453 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3191 "pascal.tab.c"
    break;

  case 142: /* lo_bound: lo_bound MINUS b_term  */
#line 454 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3197 "pascal.tab.c"
    break;

  case 143: /* lo_term: scalar_constant  */
#line 457 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3203 "pascal.tab.c"
    break;

  case 144: /* lo_term: IDENT PERIOD IDENT  */
#line 458 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3209 "pascal.tab.c"
    break;

  case 145: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 459 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3215 "pascal.tab.c"
    break;

  case 146: /* lo_term: lo_term MUL b_factor  */
#line 460 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3221 "pascal.tab.c"
    break;

  case 147: /* lo_term: lo_term IDIV b_factor  */
#line 461 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3227 "pascal.tab.c"
    break;

  case 148: /* lo_term: lo_term IMOD b_factor  */
#line 462 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3233 "pascal.tab.c"
    break;

  case 149: /* lo_term: lo_term SHIFTOP b_factor  */
#line 463 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3239 "pascal.tab.c"
    break;

  case 150: /* b_expr: b_term  */
#line 466 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3245 "pascal.tab.c"
    break;

  case 151: /* b_expr: PLUS b_term  */
#line 467 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3251 "pascal.tab.c"
    break;

  case 152: /* b_expr: MINUS b_term  */
#line 468 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3257 "pascal.tab.c"
    break;

  case 153: /* b_expr: b_expr PLUS b_term  */
#line 469 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3263 "pascal.tab.c"
    break;

  case 154: /* b_expr: b_expr MINUS b_term  */
#line 470 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3269 "pascal.tab.c"
    break;

  case 155: /* b_term: b_factor  */
#line 473 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3275 "pascal.tab.c"
    break;

  case 156: /* b_term: b_term MUL b_factor  */
#line 474 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3281 "pascal.tab.c"
    break;

  case 157: /* b_term: b_term IDIV b_factor  */
#line 475 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3287 "pascal.tab.c"
    break;

  case 158: /* b_term: b_term IMOD b_factor  */
#line 476 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3293 "pascal.tab.c"
    break;

  case 159: /* b_term: b_term SHIFTOP b_factor  */
#line 477 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3299 "pascal.tab.c"
    break;

  case 160: /* b_factor: scalar_constant  */
#line 480 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3305 "pascal.tab.c"
    break;

  case 161: /* b_factor: IDENT PERIOD IDENT  */
#line 481 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3311 "pascal.tab.c"
    break;

  case 162: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 482 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3317 "pascal.tab.c"
    break;

  case 163: /* b_factor: LPARENT b_expr RPARENT  */
#line 483 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 3323 "pascal.tab.c"
    break;

  case 164: /* record_body: record_field_list record_case_opt  */
#line 486 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 3329 "pascal.tab.c"
    break;

  case 165: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 489 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3335 "pascal.tab.c"
    break;

  case 166: /* record_field_list: record_field  */
#line 490 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3341 "pascal.tab.c"
    break;

  case 167: /* $@4: %empty  */
#line 493 "pascal.y"
                  { }
#line 3347 "pascal.tab.c"
    break;

  case 168: /* record_field: id_list COLON $@4 type  */
#line 493 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 3353 "pascal.tab.c"
    break;

  case 169: /* record_field: IDENT FORSY IDENT  */
#line 494 "pascal.y"
                       { if (!pt_is_helper_word((yyvsp[-2].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), NULL); }
#line 3359 "pascal.tab.c"
    break;

  case 170: /* record_field: IDENT LPARENT IDENT RPARENT FORSY IDENT  */
#line 495 "pascal.y"
                                             { if (!pt_is_helper_word((yyvsp[-5].node))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_helper_mark((yyvsp[0].node), (yyvsp[-3].node)); }
#line 3365 "pascal.tab.c"
    break;

  case 171: /* record_field: PROCEDURESY IDENT pv_mark parameter_list_opt  */
#line 496 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3371 "pascal.tab.c"
    break;

  case 172: /* record_field: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type  */
#line 497 "pascal.y"
                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3377 "pascal.tab.c"
    break;

  case 173: /* record_field: CONSTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 498 "pascal.y"
                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "constructor"); }
#line 3383 "pascal.tab.c"
    break;

  case 174: /* record_field: DESTRUCTORSY IDENT pv_mark parameter_list_opt  */
#line 499 "pascal.y"
                                                   { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "destructor"); }
#line 3389 "pascal.tab.c"
    break;

  case 175: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt COLON result_type  */
#line 500 "pascal.y"
                                                                      { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3395 "pascal.tab.c"
    break;

  case 176: /* record_field: IDENT record_field  */
#line 501 "pascal.y"
                        { if (strcasecmp((yyvsp[-1].node)->v.sval, "public") && strcasecmp((yyvsp[-1].node)->v.sval, "private") && strcasecmp((yyvsp[-1].node)->v.sval, "protected") && strcasecmp((yyvsp[-1].node)->v.sval, "published") && strcasecmp((yyvsp[-1].node)->v.sval, "strict")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = (yyvsp[0].node); }
#line 3401 "pascal.tab.c"
    break;

  case 177: /* record_field: CLASSSY IDENT BECOMES pv_mark parameter_list_opt COLON result_type  */
#line 502 "pascal.y"
                                                                        { if (strcasecmp((yyvsp[-5].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_sval(pt_new(TT_VAR), "implicit")), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3407 "pascal.tab.c"
    break;

  case 178: /* record_field: CLASSSY IDENT IDENT pv_mark parameter_list_opt  */
#line 503 "pascal.y"
                                                    { if (strcasecmp((yyvsp[-3].node)->v.sval, "operator") || (strcasecmp((yyvsp[-2].node)->v.sval, "initialize") && strcasecmp((yyvsp[-2].node)->v.sval, "finalize") && strcasecmp((yyvsp[-2].node)->v.sval, "copy") && strcasecmp((yyvsp[-2].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-2].node)), (yyvsp[0].node)), "operator"); }
#line 3413 "pascal.tab.c"
    break;

  case 179: /* record_field: %empty  */
#line 504 "pascal.y"
     { (yyval.node) = NULL; }
#line 3419 "pascal.tab.c"
    break;

  case 180: /* case_arm_mark: %empty  */
#line 507 "pascal.y"
    { }
#line 3425 "pascal.tab.c"
    break;

  case 181: /* case_open: %empty  */
#line 510 "pascal.y"
    { }
#line 3431 "pascal.tab.c"
    break;

  case 182: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 513 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3437 "pascal.tab.c"
    break;

  case 183: /* record_case_opt: CASESY IDENT COLON LPARENT enum_list RPARENT OFSY case_open record_case_list  */
#line 514 "pascal.y"
                                                                                  { tree_t *_e = pt_retag((yyvsp[-4].node), TT_ENUM_TYPE); for (int _i = 0; _e && _i < _e->n; _i++) cenv_int(_e->c[_i]->v.sval, _i); (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-7].node)), _e), (yyvsp[0].node)); }
#line 3443 "pascal.tab.c"
    break;

  case 184: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 515 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 3449 "pascal.tab.c"
    break;

  case 185: /* record_case_opt: %empty  */
#line 516 "pascal.y"
     { (yyval.node) = NULL; }
#line 3455 "pascal.tab.c"
    break;

  case 186: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 519 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3461 "pascal.tab.c"
    break;

  case 187: /* record_case_list: record_case_arm  */
#line 520 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3467 "pascal.tab.c"
    break;

  case 188: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 523 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3473 "pascal.tab.c"
    break;

  case 189: /* record_case_arm: %empty  */
#line 524 "pascal.y"
     { (yyval.node) = NULL; }
#line 3479 "pascal.tab.c"
    break;

  case 190: /* var_decl_list: var_decl_list var_decl  */
#line 527 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 3485 "pascal.tab.c"
    break;

  case 191: /* var_decl_list: var_decl  */
#line 528 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 3491 "pascal.tab.c"
    break;

  case 192: /* var_decl: id_list COLON type SEMICOLON  */
#line 531 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 3497 "pascal.tab.c"
    break;

  case 193: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 532 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3503 "pascal.tab.c"
    break;

  case 194: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 533 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 3509 "pascal.tab.c"
    break;

  case 195: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 536 "pascal.y"
                                                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3515 "pascal.tab.c"
    break;

  case 196: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives FORWARDSY SEMICOLON  */
#line 537 "pascal.y"
                                                                                                                    { pascal_note_type_ident((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-8].node)), (yyvsp[-6].node)), (yyvsp[-4].node)), pt_kw("forward")); }
#line 3521 "pascal.tab.c"
    break;

  case 197: /* $@5: %empty  */
#line 538 "pascal.y"
                                                                               { }
#line 3527 "pascal.tab.c"
    break;

  case 198: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 538 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3533 "pascal.tab.c"
    break;

  case 199: /* $@6: %empty  */
#line 539 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3539 "pascal.tab.c"
    break;

  case 200: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 539 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3545 "pascal.tab.c"
    break;

  case 201: /* $@7: %empty  */
#line 540 "pascal.y"
                                        { }
#line 3551 "pascal.tab.c"
    break;

  case 202: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 540 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3557 "pascal.tab.c"
    break;

  case 203: /* $@8: %empty  */
#line 541 "pascal.y"
                                                                                                           { }
#line 3563 "pascal.tab.c"
    break;

  case 204: /* procedure_decl: GENERICSY PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 541 "pascal.y"
                                                                                                                               { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3569 "pascal.tab.c"
    break;

  case 205: /* $@9: %empty  */
#line 542 "pascal.y"
                                                                                                                            { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3575 "pascal.tab.c"
    break;

  case 206: /* procedure_decl: GENERICSY FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 542 "pascal.y"
                                                                                                                                                                             { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3581 "pascal.tab.c"
    break;

  case 207: /* $@10: %empty  */
#line 543 "pascal.y"
                                                                                                 { }
#line 3587 "pascal.tab.c"
    break;

  case 208: /* procedure_decl: PROCEDURESY IDENT LTOP id_list GTOP pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 543 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-8].node)), pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-10].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3593 "pascal.tab.c"
    break;

  case 209: /* $@11: %empty  */
#line 544 "pascal.y"
                                                                                                                  { pascal_note_type_ident((yyvsp[-2].node)); }
#line 3599 "pascal.tab.c"
    break;

  case 210: /* procedure_decl: FUNCTIONSY IDENT LTOP id_list GTOP pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 544 "pascal.y"
                                                                                                                                                                  { (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "generic"), (yyvsp[-10].node)), pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-12].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node))); }
#line 3605 "pascal.tab.c"
    break;

  case 211: /* $@12: %empty  */
#line 545 "pascal.y"
                                                                                            { }
#line 3611 "pascal.tab.c"
    break;

  case 212: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 545 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 3617 "pascal.tab.c"
    break;

  case 213: /* $@13: %empty  */
#line 546 "pascal.y"
                                                                                                             { }
#line 3623 "pascal.tab.c"
    break;

  case 214: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 546 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 3631 "pascal.tab.c"
    break;

  case 215: /* $@14: %empty  */
#line 549 "pascal.y"
                                                                                              { }
#line 3637 "pascal.tab.c"
    break;

  case 216: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@14 block SEMICOLON  */
#line 549 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 3645 "pascal.tab.c"
    break;

  case 217: /* $@15: %empty  */
#line 552 "pascal.y"
                                                                                             { }
#line 3651 "pascal.tab.c"
    break;

  case 218: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@15 block SEMICOLON  */
#line 552 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3659 "pascal.tab.c"
    break;

  case 219: /* $@16: %empty  */
#line 555 "pascal.y"
                                                                                                    { }
#line 3665 "pascal.tab.c"
    break;

  case 220: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@16 block SEMICOLON  */
#line 555 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3673 "pascal.tab.c"
    break;

  case 221: /* $@17: %empty  */
#line 558 "pascal.y"
                                                                                                                { }
#line 3679 "pascal.tab.c"
    break;

  case 222: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@17 block SEMICOLON  */
#line 558 "pascal.y"
                                                                                                                                    {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3688 "pascal.tab.c"
    break;

  case 223: /* $@18: %empty  */
#line 562 "pascal.y"
                                                                                                                  { }
#line 3694 "pascal.tab.c"
    break;

  case 224: /* procedure_decl: CLASSSY IDENT IDENT PERIOD BECOMES pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@18 block SEMICOLON  */
#line 562 "pascal.y"
                                                                                                                                      {
        if (strcasecmp((yyvsp[-12].node)->v.sval, "operator")) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), pt_sval(pt_new(TT_VAR), "implicit"))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3703 "pascal.tab.c"
    break;

  case 225: /* $@19: %empty  */
#line 566 "pascal.y"
                                                                                              { }
#line 3709 "pascal.tab.c"
    break;

  case 226: /* procedure_decl: CLASSSY IDENT IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@19 block SEMICOLON  */
#line 566 "pascal.y"
                                                                                                                  {
        if (strcasecmp((yyvsp[-10].node)->v.sval, "operator") || (strcasecmp((yyvsp[-7].node)->v.sval, "initialize") && strcasecmp((yyvsp[-7].node)->v.sval, "finalize") && strcasecmp((yyvsp[-7].node)->v.sval, "copy") && strcasecmp((yyvsp[-7].node)->v.sval, "addref"))) { pascal_yyerror("syntax error"); YYERROR; }
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "operator");
    }
#line 3718 "pascal.tab.c"
    break;

  case 227: /* $@20: %empty  */
#line 570 "pascal.y"
                                                                                                                     { }
#line 3724 "pascal.tab.c"
    break;

  case 228: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@20 block SEMICOLON  */
#line 570 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3732 "pascal.tab.c"
    break;

  case 229: /* pv_mark: %empty  */
#line 575 "pascal.y"
    { }
#line 3738 "pascal.tab.c"
    break;

  case 230: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 578 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3744 "pascal.tab.c"
    break;

  case 231: /* parameter_list_opt: LPARENT RPARENT  */
#line 579 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3750 "pascal.tab.c"
    break;

  case 232: /* parameter_list_opt: %empty  */
#line 580 "pascal.y"
     { (yyval.node) = NULL; }
#line 3756 "pascal.tab.c"
    break;

  case 233: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 583 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3762 "pascal.tab.c"
    break;

  case 234: /* parameter_decl_list: parameter_decl  */
#line 584 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3768 "pascal.tab.c"
    break;

  case 235: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 587 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3774 "pascal.tab.c"
    break;

  case 236: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 588 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3780 "pascal.tab.c"
    break;

  case 237: /* parameter_decl: VARSY id_list COLON param_type  */
#line 589 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3786 "pascal.tab.c"
    break;

  case 238: /* parameter_decl: id_list COLON param_type  */
#line 590 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3792 "pascal.tab.c"
    break;

  case 239: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 591 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3798 "pascal.tab.c"
    break;

  case 240: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 592 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3804 "pascal.tab.c"
    break;

  case 241: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 593 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3810 "pascal.tab.c"
    break;

  case 242: /* parameter_decl: IDENT id_list COLON param_type  */
#line 594 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3816 "pascal.tab.c"
    break;

  case 243: /* parameter_decl: VARSY id_list  */
#line 595 "pascal.y"
                   { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "var"); }
#line 3822 "pascal.tab.c"
    break;

  case 244: /* parameter_decl: CONSTSY id_list  */
#line 596 "pascal.y"
                     { pascal_flip_dialect(PAS_DIALECT_FPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), "const"); }
#line 3828 "pascal.tab.c"
    break;

  case 245: /* parameter_decl: IDENT id_list  */
#line 597 "pascal.y"
                   { int cr = (yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, "constref"); if (!(yyvsp[-1].node)->v.sval || (strcasecmp((yyvsp[-1].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[0].node)), pt_sval(pt_new(TT_VAR), "$untyped")); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3834 "pascal.tab.c"
    break;

  case 246: /* param_type: IDENT  */
#line 600 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3840 "pascal.tab.c"
    break;

  case 247: /* param_type: IDENT PERIOD IDENT  */
#line 601 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3846 "pascal.tab.c"
    break;

  case 248: /* param_type: ARRAYSY OFSY IDENT  */
#line 602 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3852 "pascal.tab.c"
    break;

  case 249: /* param_type: ARRAYSY OFSY CONSTSY  */
#line 603 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), pt_sval(pt_new(TT_VAR), "const")); }
#line 3858 "pascal.tab.c"
    break;

  case 250: /* param_type: spec_ref  */
#line 604 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3864 "pascal.tab.c"
    break;

  case 251: /* routine_directives: %empty  */
#line 607 "pascal.y"
    { (yyval.node) = NULL; }
#line 3870 "pascal.tab.c"
    break;

  case 252: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 608 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe", "ms_abi_default", "sysv_abi_default",
            "vectorcall", "oldfpccall", "interrupt", "hardfloat", "varargs", "public" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3884 "pascal.tab.c"
    break;

  case 253: /* routine_directives: routine_directives IDENT COLON STRINGCONST SEMICOLON  */
#line 617 "pascal.y"
                                                          {
        if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "alias")) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3894 "pascal.tab.c"
    break;

  case 254: /* routine_directives: routine_directives IDENT IDENT STRINGCONST SEMICOLON  */
#line 622 "pascal.y"
                                                          {
        if (!(yyvsp[-3].node)->v.sval || !(yyvsp[-2].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "public") || strcasecmp((yyvsp[-2].node)->v.sval, "name")) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3904 "pascal.tab.c"
    break;

  case 255: /* pf_params: LPARENT pf_sections RPARENT  */
#line 629 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3910 "pascal.tab.c"
    break;

  case 256: /* pf_params: LPARENT RPARENT  */
#line 630 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3916 "pascal.tab.c"
    break;

  case 257: /* pf_params: %empty  */
#line 631 "pascal.y"
     { (yyval.node) = NULL; }
#line 3922 "pascal.tab.c"
    break;

  case 258: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 634 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3928 "pascal.tab.c"
    break;

  case 259: /* pf_sections: pf_section  */
#line 635 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3934 "pascal.tab.c"
    break;

  case 260: /* pf_section: id_list COLON param_type  */
#line 638 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3940 "pascal.tab.c"
    break;

  case 261: /* pf_section: VARSY id_list COLON param_type  */
#line 639 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3946 "pascal.tab.c"
    break;

  case 262: /* pf_section: CONSTSY id_list COLON param_type  */
#line 640 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3952 "pascal.tab.c"
    break;

  case 263: /* pf_section: IDENT id_list COLON param_type  */
#line 641 "pascal.y"
                                    { int cr = (yyvsp[-3].node)->v.sval && !strcasecmp((yyvsp[-3].node)->v.sval, "constref"); if (!(yyvsp[-3].node)->v.sval || (strcasecmp((yyvsp[-3].node)->v.sval, "out") && !cr)) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), cr ? "const" : "var"); }
#line 3958 "pascal.tab.c"
    break;

  case 264: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 642 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3964 "pascal.tab.c"
    break;

  case 265: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 643 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3970 "pascal.tab.c"
    break;

  case 266: /* enum_list: enum_list COMMA enum_member  */
#line 646 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3976 "pascal.tab.c"
    break;

  case 267: /* enum_list: enum_member  */
#line 647 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3982 "pascal.tab.c"
    break;

  case 268: /* enum_member: IDENT  */
#line 650 "pascal.y"
          { (yyval.node) = pas_enum_member((yyvsp[0].node)); }
#line 3988 "pascal.tab.c"
    break;

  case 269: /* id_list: id_list COMMA IDENT  */
#line 653 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3994 "pascal.tab.c"
    break;

  case 270: /* id_list: IDENT  */
#line 654 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4000 "pascal.tab.c"
    break;

  case 271: /* body: BEGINSY statement_list ENDSY  */
#line 657 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4006 "pascal.tab.c"
    break;

  case 272: /* $@21: %empty  */
#line 660 "pascal.y"
                             { pas_stmt_mark(); }
#line 4012 "pascal.tab.c"
    break;

  case 273: /* statement_list: statement_list SEMICOLON $@21 statement  */
#line 660 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 4018 "pascal.tab.c"
    break;

  case 274: /* $@22: %empty  */
#line 661 "pascal.y"
     { pas_stmt_mark(); }
#line 4024 "pascal.tab.c"
    break;

  case 275: /* statement_list: $@22 statement  */
#line 661 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 4030 "pascal.tab.c"
    break;

  case 276: /* $@23: %empty  */
#line 664 "pascal.y"
    { pas_stmt_mark(); }
#line 4036 "pascal.tab.c"
    break;

  case 277: /* body_stmt: $@23 statement  */
#line 664 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 4042 "pascal.tab.c"
    break;

  case 278: /* statement: statement_no_label  */
#line 667 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 4048 "pascal.tab.c"
    break;

  case 279: /* statement: INTCONST COLON statement_no_label  */
#line 668 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4054 "pascal.tab.c"
    break;

  case 280: /* statement: IDENT COLON statement_no_label  */
#line 669 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4060 "pascal.tab.c"
    break;

  case 281: /* statement_no_label: assignment  */
#line 672 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4066 "pascal.tab.c"
    break;

  case 282: /* statement_no_label: call  */
#line 673 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4072 "pascal.tab.c"
    break;

  case 283: /* statement_no_label: compound_statement  */
#line 674 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 4078 "pascal.tab.c"
    break;

  case 284: /* statement_no_label: goto_statement  */
#line 675 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4084 "pascal.tab.c"
    break;

  case 285: /* statement_no_label: if_statement  */
#line 676 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 4090 "pascal.tab.c"
    break;

  case 286: /* statement_no_label: case_statement  */
#line 677 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4096 "pascal.tab.c"
    break;

  case 287: /* statement_no_label: while_statement  */
#line 678 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4102 "pascal.tab.c"
    break;

  case 288: /* statement_no_label: repeat_statement  */
#line 679 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4108 "pascal.tab.c"
    break;

  case 289: /* statement_no_label: for_statement  */
#line 680 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4114 "pascal.tab.c"
    break;

  case 290: /* statement_no_label: with_statement  */
#line 681 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4120 "pascal.tab.c"
    break;

  case 291: /* statement_no_label: try_statement  */
#line 682 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 4126 "pascal.tab.c"
    break;

  case 292: /* statement_no_label: raise_statement  */
#line 683 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 4132 "pascal.tab.c"
    break;

  case 293: /* statement_no_label: inherited_call  */
#line 684 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4138 "pascal.tab.c"
    break;

  case 294: /* statement_no_label: INHERITEDSY  */
#line 685 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 4144 "pascal.tab.c"
    break;

  case 295: /* statement_no_label: %empty  */
#line 686 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 4150 "pascal.tab.c"
    break;

  case 296: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 689 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4156 "pascal.tab.c"
    break;

  case 297: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 690 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 4162 "pascal.tab.c"
    break;

  case 298: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 691 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 4168 "pascal.tab.c"
    break;

  case 299: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 692 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 4174 "pascal.tab.c"
    break;

  case 300: /* handler_list: handler  */
#line 695 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4180 "pascal.tab.c"
    break;

  case 301: /* handler_list: handler_list SEMICOLON handler  */
#line 696 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4186 "pascal.tab.c"
    break;

  case 302: /* handler_list: handler_list SEMICOLON  */
#line 697 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 4192 "pascal.tab.c"
    break;

  case 303: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 700 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4198 "pascal.tab.c"
    break;

  case 304: /* handler: ONSY IDENT DOSY body_stmt  */
#line 701 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4204 "pascal.tab.c"
    break;

  case 305: /* raise_statement: RAISESY  */
#line 704 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 4210 "pascal.tab.c"
    break;

  case 306: /* raise_statement: RAISESY expression  */
#line 705 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 4216 "pascal.tab.c"
    break;

  case 307: /* inherited_call: INHERITEDSY IDENT  */
#line 708 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 4222 "pascal.tab.c"
    break;

  case 308: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 709 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 4228 "pascal.tab.c"
    break;

  case 309: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 710 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4234 "pascal.tab.c"
    break;

  case 310: /* call: IDENT  */
#line 713 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4240 "pascal.tab.c"
    break;

  case 311: /* call: spec_expr  */
#line 714 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 4246 "pascal.tab.c"
    break;

  case 312: /* call: selector PERIOD IDENT  */
#line 715 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4252 "pascal.tab.c"
    break;

  case 313: /* call: call_with_args PERIOD IDENT  */
#line 716 "pascal.y"
                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 4258 "pascal.tab.c"
    break;

  case 314: /* call: call_with_args  */
#line 717 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4264 "pascal.tab.c"
    break;

  case 315: /* recv_primary: STRINGCONST  */
#line 720 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 4270 "pascal.tab.c"
    break;

  case 316: /* recv_primary: LPARENT expression RPARENT  */
#line 721 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4276 "pascal.tab.c"
    break;

  case 317: /* recv_primary: spec_expr  */
#line 722 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4282 "pascal.tab.c"
    break;

  case 318: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 725 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4288 "pascal.tab.c"
    break;

  case 319: /* call_with_args: spec_expr LPARENT argument_list RPARENT  */
#line 726 "pascal.y"
                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4294 "pascal.tab.c"
    break;

  case 320: /* call_with_args: spec_expr LPARENT RPARENT  */
#line 727 "pascal.y"
                               { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4300 "pascal.tab.c"
    break;

  case 321: /* call_with_args: IDENT LPARENT RPARENT  */
#line 728 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4306 "pascal.tab.c"
    break;

  case 322: /* call_with_args: call_with_args LPARENT argument_list RPARENT  */
#line 729 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4312 "pascal.tab.c"
    break;

  case 323: /* call_with_args: call_with_args LPARENT RPARENT  */
#line 730 "pascal.y"
                                    { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 4318 "pascal.tab.c"
    break;

  case 324: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 731 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4324 "pascal.tab.c"
    break;

  case 325: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 732 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4330 "pascal.tab.c"
    break;

  case 326: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 733 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4336 "pascal.tab.c"
    break;

  case 327: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 734 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4342 "pascal.tab.c"
    break;

  case 328: /* call_with_args: recv_primary PERIOD IDENT LPARENT argument_list RPARENT  */
#line 735 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 4348 "pascal.tab.c"
    break;

  case 329: /* call_with_args: recv_primary PERIOD IDENT LPARENT RPARENT  */
#line 736 "pascal.y"
                                               { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 4354 "pascal.tab.c"
    break;

  case 330: /* argument_list: argument_list COMMA argument  */
#line 739 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4360 "pascal.tab.c"
    break;

  case 331: /* argument_list: argument  */
#line 740 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4366 "pascal.tab.c"
    break;

  case 332: /* argument: expression  */
#line 743 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4372 "pascal.tab.c"
    break;

  case 333: /* argument: expression COLON expression  */
#line 744 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4378 "pascal.tab.c"
    break;

  case 334: /* argument: expression COLON expression COLON expression  */
#line 745 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4384 "pascal.tab.c"
    break;

  case 335: /* argument: selector LBRACK expression DOTDOT expression RBRACK  */
#line 746 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-5].node)), pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 4390 "pascal.tab.c"
    break;

  case 336: /* assignment: selector BECOMES expression  */
#line 749 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4396 "pascal.tab.c"
    break;

  case 337: /* assignment: selector ADDBECOMES expression  */
#line 750 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 4402 "pascal.tab.c"
    break;

  case 338: /* assignment: selector SUBBECOMES expression  */
#line 751 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 4408 "pascal.tab.c"
    break;

  case 339: /* assignment: selector MULBECOMES expression  */
#line 752 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 4414 "pascal.tab.c"
    break;

  case 340: /* assignment: selector DIVBECOMES expression  */
#line 753 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 4420 "pascal.tab.c"
    break;

  case 341: /* assignment: call_with_args BECOMES expression  */
#line 754 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4426 "pascal.tab.c"
    break;

  case 342: /* selector: selector LBRACK expression_list RBRACK  */
#line 757 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4432 "pascal.tab.c"
    break;

  case 343: /* selector: selector PERIOD IDENT  */
#line 758 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4438 "pascal.tab.c"
    break;

  case 344: /* selector: selector ARROW  */
#line 759 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4444 "pascal.tab.c"
    break;

  case 345: /* selector: call_with_args PERIOD IDENT  */
#line 760 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4450 "pascal.tab.c"
    break;

  case 346: /* selector: call_with_args LBRACK expression_list RBRACK  */
#line 761 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4456 "pascal.tab.c"
    break;

  case 347: /* selector: call_with_args ARROW  */
#line 762 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4462 "pascal.tab.c"
    break;

  case 348: /* selector: recv_primary PERIOD IDENT  */
#line 763 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4468 "pascal.tab.c"
    break;

  case 349: /* selector: recv_primary ARROW  */
#line 764 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 4474 "pascal.tab.c"
    break;

  case 350: /* selector: IDENT  */
#line 765 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4480 "pascal.tab.c"
    break;

  case 351: /* expression_list: expression_list COMMA expression  */
#line 768 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4486 "pascal.tab.c"
    break;

  case 352: /* expression_list: expression  */
#line 769 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4492 "pascal.tab.c"
    break;

  case 353: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 772 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 4498 "pascal.tab.c"
    break;

  case 354: /* goto_statement: GOTOSY INTCONST  */
#line 775 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4504 "pascal.tab.c"
    break;

  case 355: /* goto_statement: GOTOSY IDENT  */
#line 776 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 4510 "pascal.tab.c"
    break;

  case 356: /* if_statement: IFSY expression THENSY body_stmt  */
#line 779 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4516 "pascal.tab.c"
    break;

  case 357: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 780 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4522 "pascal.tab.c"
    break;

  case 358: /* $@24: %empty  */
#line 783 "pascal.y"
                           { }
#line 4528 "pascal.tab.c"
    break;

  case 359: /* case_statement: CASESY expression OFSY $@24 case_list case_tail ENDSY  */
#line 783 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 4534 "pascal.tab.c"
    break;

  case 360: /* case_tail: %empty  */
#line 786 "pascal.y"
    { (yyval.node) = NULL; }
#line 4540 "pascal.tab.c"
    break;

  case 361: /* case_tail: ELSESY statement_list  */
#line 787 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 4546 "pascal.tab.c"
    break;

  case 362: /* case_tail: OTHERWISESY statement_list  */
#line 788 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 4552 "pascal.tab.c"
    break;

  case 363: /* case_list: case_list SEMICOLON case_elem  */
#line 791 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4558 "pascal.tab.c"
    break;

  case 364: /* case_list: case_elem  */
#line 792 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4564 "pascal.tab.c"
    break;

  case 365: /* case_elem: constant_list COLON body_stmt  */
#line 795 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4570 "pascal.tab.c"
    break;

  case 366: /* case_elem: %empty  */
#line 796 "pascal.y"
     { (yyval.node) = NULL; }
#line 4576 "pascal.tab.c"
    break;

  case 367: /* constant_list: constant_list COMMA case_label  */
#line 799 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4582 "pascal.tab.c"
    break;

  case 368: /* constant_list: case_label  */
#line 800 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4588 "pascal.tab.c"
    break;

  case 369: /* case_label: b_expr  */
#line 803 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4594 "pascal.tab.c"
    break;

  case 370: /* case_label: b_expr DOTDOT b_expr  */
#line 804 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4600 "pascal.tab.c"
    break;

  case 371: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 807 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4606 "pascal.tab.c"
    break;

  case 372: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 810 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4612 "pascal.tab.c"
    break;

  case 373: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 813 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 4618 "pascal.tab.c"
    break;

  case 374: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 814 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 4624 "pascal.tab.c"
    break;

  case 375: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 815 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 4630 "pascal.tab.c"
    break;

  case 376: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 818 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4636 "pascal.tab.c"
    break;

  case 377: /* with_open: with_open COMMA selector  */
#line 821 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4642 "pascal.tab.c"
    break;

  case 378: /* with_open: selector  */
#line 822 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4648 "pascal.tab.c"
    break;

  case 379: /* expression: simple_expression  */
#line 829 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 4654 "pascal.tab.c"
    break;

  case 380: /* expression: expression INOP simple_expression  */
#line 830 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4660 "pascal.tab.c"
    break;

  case 381: /* expression: expression LTOP simple_expression  */
#line 831 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4666 "pascal.tab.c"
    break;

  case 382: /* expression: expression LEOP simple_expression  */
#line 832 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4672 "pascal.tab.c"
    break;

  case 383: /* expression: expression GTOP simple_expression  */
#line 833 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4678 "pascal.tab.c"
    break;

  case 384: /* expression: expression GEOP simple_expression  */
#line 834 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4684 "pascal.tab.c"
    break;

  case 385: /* expression: expression NEOP simple_expression  */
#line 835 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4690 "pascal.tab.c"
    break;

  case 386: /* expression: expression EQOP simple_expression  */
#line 836 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4696 "pascal.tab.c"
    break;

  case 387: /* expression: expression ISOP simple_expression  */
#line 837 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4702 "pascal.tab.c"
    break;

  case 388: /* simple_expression: term  */
#line 840 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 4708 "pascal.tab.c"
    break;

  case 389: /* simple_expression: PLUS term  */
#line 841 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 4714 "pascal.tab.c"
    break;

  case 390: /* simple_expression: MINUS term  */
#line 842 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 4720 "pascal.tab.c"
    break;

  case 391: /* simple_expression: simple_expression PLUS term  */
#line 843 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4726 "pascal.tab.c"
    break;

  case 392: /* simple_expression: simple_expression MINUS term  */
#line 844 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4732 "pascal.tab.c"
    break;

  case 393: /* simple_expression: simple_expression OROP term  */
#line 845 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4738 "pascal.tab.c"
    break;

  case 394: /* simple_expression: simple_expression XOROP term  */
#line 846 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4744 "pascal.tab.c"
    break;

  case 395: /* term: power  */
#line 849 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 4750 "pascal.tab.c"
    break;

  case 396: /* term: term MUL power  */
#line 850 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4756 "pascal.tab.c"
    break;

  case 397: /* term: term RDIV power  */
#line 851 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4762 "pascal.tab.c"
    break;

  case 398: /* term: term IDIV power  */
#line 852 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4768 "pascal.tab.c"
    break;

  case 399: /* term: term IMOD power  */
#line 853 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4774 "pascal.tab.c"
    break;

  case 400: /* term: term ANDOP power  */
#line 854 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4780 "pascal.tab.c"
    break;

  case 401: /* term: term MUL MINUS power  */
#line 855 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4786 "pascal.tab.c"
    break;

  case 402: /* term: term RDIV MINUS power  */
#line 856 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4792 "pascal.tab.c"
    break;

  case 403: /* term: term IDIV MINUS power  */
#line 857 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4798 "pascal.tab.c"
    break;

  case 404: /* term: term IMOD MINUS power  */
#line 858 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4804 "pascal.tab.c"
    break;

  case 405: /* term: term ANDOP MINUS power  */
#line 859 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 4810 "pascal.tab.c"
    break;

  case 406: /* term: term SHIFTOP power  */
#line 860 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4816 "pascal.tab.c"
    break;

  case 407: /* term: term ASOP power  */
#line 861 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4822 "pascal.tab.c"
    break;

  case 408: /* power: factor  */
#line 864 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 4828 "pascal.tab.c"
    break;

  case 409: /* power: power POWOP factor  */
#line 865 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4834 "pascal.tab.c"
    break;

  case 410: /* factor: selector  */
#line 868 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 4840 "pascal.tab.c"
    break;

  case 411: /* factor: call_with_args  */
#line 869 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4846 "pascal.tab.c"
    break;

  case 412: /* factor: INTCONST  */
#line 870 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4852 "pascal.tab.c"
    break;

  case 413: /* factor: REALCONST  */
#line 871 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4858 "pascal.tab.c"
    break;

  case 414: /* factor: STRINGCONST  */
#line 872 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 4864 "pascal.tab.c"
    break;

  case 415: /* factor: CHARCODE  */
#line 873 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4870 "pascal.tab.c"
    break;

  case 416: /* factor: LPARENT expression RPARENT  */
#line 874 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); if ((yyval.node) && (yyval.node)->slen == 0 && (yyval.node)->t != TT_ILIT && (yyval.node)->t != TT_FLIT && (yyval.node)->t != TT_CHRLIT && (yyval.node)->t != TT_QLIT) (yyval.node)->slen = -7; }
#line 4876 "pascal.tab.c"
    break;

  case 417: /* factor: inherited_call  */
#line 875 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4882 "pascal.tab.c"
    break;

  case 418: /* factor: NOTSY factor  */
#line 876 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4888 "pascal.tab.c"
    break;

  case 419: /* factor: ATSIGN factor  */
#line 877 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4894 "pascal.tab.c"
    break;

  case 420: /* factor: LBRACK RBRACK  */
#line 878 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4900 "pascal.tab.c"
    break;

  case 421: /* factor: LBRACK set_member_list RBRACK  */
#line 879 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4906 "pascal.tab.c"
    break;

  case 422: /* set_member_list: set_member  */
#line 882 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4912 "pascal.tab.c"
    break;

  case 423: /* set_member_list: set_member_list COMMA set_member  */
#line 883 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4918 "pascal.tab.c"
    break;

  case 424: /* set_member: expression  */
#line 886 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4924 "pascal.tab.c"
    break;

  case 425: /* set_member: expression DOTDOT expression  */
#line 887 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4930 "pascal.tab.c"
    break;


#line 4934 "pascal.tab.c"

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

#line 893 "pascal.y"

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
