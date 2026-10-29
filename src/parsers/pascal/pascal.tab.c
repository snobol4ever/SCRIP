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

#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
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
void pascal_last_ident_set(const char *p, int n) { g_pascal_cenv.ident_p = p; g_pascal_cenv.ident_n = n; }
tree_t *pascal_last_ident_qlit(void) {
    tree_t *q = ast_node_new(TT_QLIT);
    char *s = ct_alloc((size_t)g_pascal_cenv.ident_n + 1);
    if (g_pascal_cenv.ident_p) memcpy(s, g_pascal_cenv.ident_p, (size_t)g_pascal_cenv.ident_n);
    s[g_pascal_cenv.ident_p ? g_pascal_cenv.ident_n : 0] = '\0';
    q->v.sval = s;
    return q;
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

#line 224 "pascal.tab.c"

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
  YYSYMBOL_TRYSY = 83,                     /* TRYSY  */
  YYSYMBOL_EXCEPTSY = 84,                  /* EXCEPTSY  */
  YYSYMBOL_FINALLYSY = 85,                 /* FINALLYSY  */
  YYSYMBOL_RAISESY = 86,                   /* RAISESY  */
  YYSYMBOL_INHERITEDSY = 87,               /* INHERITEDSY  */
  YYSYMBOL_CONSTRUCTORSY = 88,             /* CONSTRUCTORSY  */
  YYSYMBOL_DESTRUCTORSY = 89,              /* DESTRUCTORSY  */
  YYSYMBOL_PROPERTYSY = 90,                /* PROPERTYSY  */
  YYSYMBOL_ONSY = 91,                      /* ONSY  */
  YYSYMBOL_YYACCEPT = 92,                  /* $accept  */
  YYSYMBOL_compilation_unit = 93,          /* compilation_unit  */
  YYSYMBOL_program = 94,                   /* program  */
  YYSYMBOL_program_name = 95,              /* program_name  */
  YYSYMBOL_file_id_list_opt = 96,          /* file_id_list_opt  */
  YYSYMBOL_block = 97,                     /* block  */
  YYSYMBOL_decl_part_list = 98,            /* decl_part_list  */
  YYSYMBOL_decl_part = 99,                 /* decl_part  */
  YYSYMBOL_unit = 100,                     /* unit  */
  YYSYMBOL_intf_part_list = 101,           /* intf_part_list  */
  YYSYMBOL_intf_part = 102,                /* intf_part  */
  YYSYMBOL_init_opt = 103,                 /* init_opt  */
  YYSYMBOL_fini_opt = 104,                 /* fini_opt  */
  YYSYMBOL_label_list = 105,               /* label_list  */
  YYSYMBOL_const_decl_list = 106,          /* const_decl_list  */
  YYSYMBOL_const_decl = 107,               /* const_decl  */
  YYSYMBOL_typed_value = 108,              /* typed_value  */
  YYSYMBOL_typed_value_list = 109,         /* typed_value_list  */
  YYSYMBOL_field_value_list = 110,         /* field_value_list  */
  YYSYMBOL_scalar_constant = 111,          /* scalar_constant  */
  YYSYMBOL_type_decl_list = 112,           /* type_decl_list  */
  YYSYMBOL_type_decl = 113,                /* type_decl  */
  YYSYMBOL_type = 114,                     /* type  */
  YYSYMBOL_115_1 = 115,                    /* $@1  */
  YYSYMBOL_116_2 = 116,                    /* $@2  */
  YYSYMBOL_117_3 = 117,                    /* $@3  */
  YYSYMBOL_result_type = 118,              /* result_type  */
  YYSYMBOL_class_members = 119,            /* class_members  */
  YYSYMBOL_class_member = 120,             /* class_member  */
  YYSYMBOL_prop_index_opt = 121,           /* prop_index_opt  */
  YYSYMBOL_prop_specs = 122,               /* prop_specs  */
  YYSYMBOL_packed_opt = 123,               /* packed_opt  */
  YYSYMBOL_simple_type = 124,              /* simple_type  */
  YYSYMBOL_lo_bound = 125,                 /* lo_bound  */
  YYSYMBOL_lo_term = 126,                  /* lo_term  */
  YYSYMBOL_b_expr = 127,                   /* b_expr  */
  YYSYMBOL_b_term = 128,                   /* b_term  */
  YYSYMBOL_b_factor = 129,                 /* b_factor  */
  YYSYMBOL_record_body = 130,              /* record_body  */
  YYSYMBOL_record_field_list = 131,        /* record_field_list  */
  YYSYMBOL_record_field = 132,             /* record_field  */
  YYSYMBOL_133_4 = 133,                    /* $@4  */
  YYSYMBOL_case_arm_mark = 134,            /* case_arm_mark  */
  YYSYMBOL_case_open = 135,                /* case_open  */
  YYSYMBOL_record_case_opt = 136,          /* record_case_opt  */
  YYSYMBOL_record_case_list = 137,         /* record_case_list  */
  YYSYMBOL_record_case_arm = 138,          /* record_case_arm  */
  YYSYMBOL_var_decl_list = 139,            /* var_decl_list  */
  YYSYMBOL_var_decl = 140,                 /* var_decl  */
  YYSYMBOL_procedure_decl = 141,           /* procedure_decl  */
  YYSYMBOL_142_5 = 142,                    /* $@5  */
  YYSYMBOL_143_6 = 143,                    /* $@6  */
  YYSYMBOL_144_7 = 144,                    /* $@7  */
  YYSYMBOL_145_8 = 145,                    /* $@8  */
  YYSYMBOL_146_9 = 146,                    /* $@9  */
  YYSYMBOL_147_10 = 147,                   /* $@10  */
  YYSYMBOL_148_11 = 148,                   /* $@11  */
  YYSYMBOL_149_12 = 149,                   /* $@12  */
  YYSYMBOL_150_13 = 150,                   /* $@13  */
  YYSYMBOL_pv_mark = 151,                  /* pv_mark  */
  YYSYMBOL_parameter_list_opt = 152,       /* parameter_list_opt  */
  YYSYMBOL_parameter_decl_list = 153,      /* parameter_decl_list  */
  YYSYMBOL_parameter_decl = 154,           /* parameter_decl  */
  YYSYMBOL_param_type = 155,               /* param_type  */
  YYSYMBOL_routine_directives = 156,       /* routine_directives  */
  YYSYMBOL_pf_params = 157,                /* pf_params  */
  YYSYMBOL_pf_sections = 158,              /* pf_sections  */
  YYSYMBOL_pf_section = 159,               /* pf_section  */
  YYSYMBOL_id_list = 160,                  /* id_list  */
  YYSYMBOL_body = 161,                     /* body  */
  YYSYMBOL_statement_list = 162,           /* statement_list  */
  YYSYMBOL_163_14 = 163,                   /* $@14  */
  YYSYMBOL_164_15 = 164,                   /* $@15  */
  YYSYMBOL_body_stmt = 165,                /* body_stmt  */
  YYSYMBOL_166_16 = 166,                   /* $@16  */
  YYSYMBOL_statement = 167,                /* statement  */
  YYSYMBOL_statement_no_label = 168,       /* statement_no_label  */
  YYSYMBOL_try_statement = 169,            /* try_statement  */
  YYSYMBOL_handler_list = 170,             /* handler_list  */
  YYSYMBOL_handler = 171,                  /* handler  */
  YYSYMBOL_raise_statement = 172,          /* raise_statement  */
  YYSYMBOL_inherited_call = 173,           /* inherited_call  */
  YYSYMBOL_call = 174,                     /* call  */
  YYSYMBOL_call_with_args = 175,           /* call_with_args  */
  YYSYMBOL_argument_list = 176,            /* argument_list  */
  YYSYMBOL_argument = 177,                 /* argument  */
  YYSYMBOL_assignment = 178,               /* assignment  */
  YYSYMBOL_selector = 179,                 /* selector  */
  YYSYMBOL_expression_list = 180,          /* expression_list  */
  YYSYMBOL_compound_statement = 181,       /* compound_statement  */
  YYSYMBOL_goto_statement = 182,           /* goto_statement  */
  YYSYMBOL_if_statement = 183,             /* if_statement  */
  YYSYMBOL_case_statement = 184,           /* case_statement  */
  YYSYMBOL_185_17 = 185,                   /* $@17  */
  YYSYMBOL_case_tail = 186,                /* case_tail  */
  YYSYMBOL_case_list = 187,                /* case_list  */
  YYSYMBOL_case_elem = 188,                /* case_elem  */
  YYSYMBOL_constant_list = 189,            /* constant_list  */
  YYSYMBOL_case_label = 190,               /* case_label  */
  YYSYMBOL_while_statement = 191,          /* while_statement  */
  YYSYMBOL_repeat_statement = 192,         /* repeat_statement  */
  YYSYMBOL_for_statement = 193,            /* for_statement  */
  YYSYMBOL_with_statement = 194,           /* with_statement  */
  YYSYMBOL_with_open = 195,                /* with_open  */
  YYSYMBOL_expression = 196,               /* expression  */
  YYSYMBOL_simple_expression = 197,        /* simple_expression  */
  YYSYMBOL_term = 198,                     /* term  */
  YYSYMBOL_power = 199,                    /* power  */
  YYSYMBOL_factor = 200,                   /* factor  */
  YYSYMBOL_set_member_list = 201,          /* set_member_list  */
  YYSYMBOL_set_member = 202                /* set_member  */
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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1194

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  111
/* YYNRULES -- Number of rules.  */
#define YYNRULES  338
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  763

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   346


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
      85,    86,    87,    88,    89,    90,    91
};

#if PASCAL_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   204,   204,   205,   208,   209,   212,   213,   216,   217,
     220,   223,   224,   227,   228,   229,   230,   231,   232,   233,
     236,   239,   240,   243,   244,   245,   246,   247,   248,   251,
     252,   253,   256,   257,   260,   261,   262,   263,   266,   267,
     270,   271,   274,   275,   276,   277,   280,   281,   284,   285,
     288,   289,   290,   291,   292,   295,   296,   299,   300,   301,
     304,   305,   306,   306,   306,   307,   308,   309,   310,   311,
     312,   313,   313,   314,   315,   316,   317,   318,   321,   322,
     325,   326,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   341,   342,   345,   346,   347,   350,   351,   354,
     355,   356,   357,   358,   361,   362,   363,   364,   365,   368,
     369,   370,   371,   372,   373,   376,   377,   378,   379,   380,
     383,   384,   385,   386,   387,   390,   391,   392,   395,   398,
     399,   402,   402,   403,   406,   409,   412,   413,   414,   417,
     418,   421,   422,   425,   426,   429,   430,   431,   434,   435,
     436,   436,   437,   437,   438,   438,   439,   439,   440,   440,
     443,   443,   446,   446,   449,   449,   452,   452,   457,   460,
     461,   462,   465,   466,   469,   470,   471,   472,   473,   474,
     475,   476,   479,   480,   481,   484,   485,   495,   496,   497,
     500,   501,   504,   505,   506,   507,   508,   511,   512,   515,
     518,   518,   519,   519,   522,   522,   525,   526,   527,   530,
     531,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   547,   548,   549,   550,   553,   554,
     555,   558,   559,   562,   563,   566,   567,   568,   571,   572,
     573,   576,   577,   578,   579,   580,   581,   584,   585,   588,
     589,   590,   593,   594,   595,   596,   597,   598,   601,   602,
     603,   604,   605,   606,   609,   610,   613,   616,   617,   620,
     621,   624,   624,   627,   628,   629,   632,   633,   636,   637,
     640,   641,   644,   645,   648,   651,   654,   655,   656,   659,
     662,   663,   670,   671,   672,   673,   674,   675,   676,   677,
     678,   681,   682,   683,   684,   685,   686,   687,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   705,   706,   709,   710,   711,   712,   713,   714,   715,
     716,   717,   718,   719,   720,   723,   724,   727,   728
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
  "ASOP", "TRYSY", "EXCEPTSY", "FINALLYSY", "RAISESY", "INHERITEDSY",
  "CONSTRUCTORSY", "DESTRUCTORSY", "PROPERTYSY", "ONSY", "$accept",
  "compilation_unit", "program", "program_name", "file_id_list_opt",
  "block", "decl_part_list", "decl_part", "unit", "intf_part_list",
  "intf_part", "init_opt", "fini_opt", "label_list", "const_decl_list",
  "const_decl", "typed_value", "typed_value_list", "field_value_list",
  "scalar_constant", "type_decl_list", "type_decl", "type", "$@1", "$@2",
  "$@3", "result_type", "class_members", "class_member", "prop_index_opt",
  "prop_specs", "packed_opt", "simple_type", "lo_bound", "lo_term",
  "b_expr", "b_term", "b_factor", "record_body", "record_field_list",
  "record_field", "$@4", "case_arm_mark", "case_open", "record_case_opt",
  "record_case_list", "record_case_arm", "var_decl_list", "var_decl",
  "procedure_decl", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "pv_mark", "parameter_list_opt", "parameter_decl_list",
  "parameter_decl", "param_type", "routine_directives", "pf_params",
  "pf_sections", "pf_section", "id_list", "body", "statement_list", "$@14",
  "$@15", "body_stmt", "$@16", "statement", "statement_no_label",
  "try_statement", "handler_list", "handler", "raise_statement",
  "inherited_call", "call", "call_with_args", "argument_list", "argument",
  "assignment", "selector", "expression_list", "compound_statement",
  "goto_statement", "if_statement", "case_statement", "$@17", "case_tail",
  "case_list", "case_elem", "constant_list", "case_label",
  "while_statement", "repeat_statement", "for_statement", "with_statement",
  "with_open", "expression", "simple_expression", "term", "power",
  "factor", "set_member_list", "set_member", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-469)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-264)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     167,     2,    16,   125,  -469,    90,   543,  -469,  -469,    13,
     158,  -469,  -469,   223,    93,   111,   114,   111,   111,   142,
    -469,   174,   104,   217,   232,  -469,  -469,  -469,   235,   111,
     233,   305,  -469,  -469,    53,   290,    93,  -469,  -469,    55,
     288,   111,  -469,   166,    56,   318,   203,   281,   306,   174,
    -469,   307,   313,   351,   373,  -469,   287,  -469,  -469,  -469,
     286,   803,   634,  -469,  -469,   411,   457,   476,  -469,   634,
    -469,   469,    51,  -469,  -469,   412,   472,  -469,   803,   478,
     803,  -469,   803,   499,  1062,  -469,   803,   491,  -469,  -469,
    -469,  -469,  -469,  -469,    84,  -469,   266,  -469,  -469,  -469,
    -469,  -469,  -469,  -469,  -469,   657,  -469,   503,   517,   496,
     505,  -469,   530,   229,  -469,  -469,   959,   959,   959,   703,
     803,   959,  -469,  -469,  -469,  -469,   531,   491,  -469,   393,
     205,   162,   268,   381,   520,  -469,   537,  -469,   537,   601,
     601,   533,   111,  -469,  -469,  -469,  -469,  1090,   542,  -469,
     555,   532,  -469,   243,   132,  -469,  -469,    97,   605,    43,
    -469,  -469,   551,   281,  -469,  -469,   323,    71,   456,   393,
     205,    74,   674,   214,   701,   396,   396,   719,    17,  1059,
     549,   803,   538,  -469,   803,   541,  -469,   803,   803,   803,
     803,   803,    54,   613,   544,   545,  -469,  -469,  -469,    93,
     111,  -469,   546,   111,   548,   174,  -469,  -469,   381,   381,
    -469,   999,   189,  -469,  1011,  -469,   550,  -469,   803,   803,
     803,   803,   803,   803,   803,   803,   959,   959,   959,   959,
     867,   883,   905,   921,   943,   959,   959,   959,   160,   602,
     565,   577,   132,   132,  -469,   298,   566,  1111,  1111,   111,
     377,   829,    37,   615,   623,  -469,   439,   439,  1111,   439,
     439,   439,   439,   476,   111,   579,   111,   581,  -469,   111,
      41,  -469,   416,   653,  -469,   829,   589,   476,  -469,   591,
    -469,   803,   803,   803,  -469,  -469,   478,  -469,  -469,  -469,
     311,  -469,  -469,  -469,   329,  -469,  1033,   578,  -469,   745,
    1059,   604,  1059,  1100,   249,  1059,  1059,  1059,  1059,  1059,
    -469,   111,  -469,  -469,  -469,   476,   476,    93,    63,   454,
    -469,   111,  -469,   174,   803,   803,  -469,  -469,   607,   268,
     268,   268,   268,   268,   268,   268,   268,   381,   381,   381,
     381,   959,   520,   959,   520,   959,   520,   959,   520,   959,
     520,   520,   520,  -469,   111,   603,   111,   606,  -469,    42,
    -469,   417,   609,   591,  -469,  -469,   439,   439,  1111,   616,
    -469,   368,   176,  -469,   354,   330,   610,  -469,   611,    52,
     180,   614,   618,   620,  -469,   434,   845,   676,  1059,   634,
    1118,   634,  1118,   111,   176,   176,   475,  -469,  -469,  -469,
    -469,   687,   442,   537,   464,   537,   465,   118,  -469,    24,
     692,   624,   694,   696,   643,   699,   646,   708,   597,   468,
    1059,  -469,   281,  -469,   205,  1111,   678,   803,  -469,   803,
     639,   273,   159,  -469,   289,  -469,   335,   761,   787,   803,
    -469,   353,   476,   476,   712,   715,  -469,  -469,  -469,   713,
     476,   476,  1059,  -469,   520,   520,   520,   520,   520,   481,
     537,   486,   537,   284,  -469,    24,  -469,   707,   176,   176,
     380,  1111,   439,   439,  -469,   439,   439,   439,   439,  -469,
    -469,  -469,  -469,  -469,   660,   662,  -469,  -469,   677,   634,
     435,   682,    44,  1011,  -469,  -469,   397,  -469,  -469,   711,
      58,  -469,   500,  -469,    24,  -469,    24,   684,    24,  -469,
     714,   688,   695,  -469,   751,  -469,  -469,  -469,   591,  -469,
     683,   752,   803,   803,  -469,  -469,   300,   186,  -469,   510,
    -469,  -469,  -469,  1046,    81,  -469,   578,  -469,  -469,  -469,
    -469,  -469,   386,  -469,   391,  1059,   755,   758,   704,  -469,
    -469,   763,   763,  -469,   739,   766,   720,    24,  -469,    24,
     725,  -469,  -469,   721,  -469,   385,   176,   176,  -469,  -469,
    -469,  -469,   564,   476,   476,  -469,  -469,   476,   476,   118,
     727,   767,   829,   829,    -1,  -469,  1118,   768,  -469,   111,
     722,  -469,  -469,   624,   736,  -469,   591,  -469,   726,   729,
     803,  -469,   796,   798,  -469,   806,   624,   535,   585,  -469,
    1111,  1111,  -469,  -469,   778,  1111,  -469,  -469,   803,  -469,
     737,  -469,   339,  -469,  -469,  -469,  -469,   591,   624,   624,
     763,   765,  -469,   591,  -469,  -469,   591,  -469,  -469,  -469,
     809,   762,   476,   476,   822,   824,    45,   764,  -469,  -469,
    -469,   413,  -469,   771,   775,  -469,  -469,   133,   634,  -469,
     803,  -469,  -469,  -469,  1059,  -469,  -469,  -469,  -469,  -469,
    -469,   475,  -469,   763,   763,  -469,  -469,  -469,  1059,  -469,
     826,  -469,   624,   834,  -469,  -469,  -469,   839,  -469,  -469,
     591,   840,   790,  -469,  -469,  -469,  -469,   829,  -469,   829,
     819,  -469,  -469,   777,  -469,   850,  1059,   624,   851,  -469,
    -469,  -469,  -469,  -469,   859,   861,  -469,   863,  -469,   591,
      27,  -469,  -469,   634,   634,  1111,   841,  -469,  -469,  -469,
    -469,   866,   624,  -469,  -469,  -469,   868,  -469,   425,  -469,
    -469,   875,  -469,   513,  -469,   876,  -469,  -469,  -469,  -469,
    -469,  1111,   818,  1111,  -469,   880,  -469,  -469,   875,  -469,
     111,   821,  -469
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      12,     0,     0,     0,     2,     0,     0,     3,     6,     9,
       0,     1,     5,     0,     0,     0,     0,     0,     0,     0,
     202,     0,     0,     0,     0,    11,    17,    10,     0,     0,
       0,     0,    36,    37,     0,     0,    14,    39,   198,     0,
     168,    16,   144,     0,     0,   168,     0,   223,     0,    15,
      56,     0,     0,     0,     0,     7,     0,    12,    22,    13,
       0,     0,    98,    38,    18,     0,     0,   171,   143,    98,
      19,     0,   171,   200,   199,     0,     0,   202,     0,     0,
       0,   202,     0,     0,   238,   202,   233,   222,   203,   206,
     219,   220,   221,   210,   240,   209,     0,   211,   212,   213,
     214,   215,   216,   217,   218,    98,    55,     0,     0,     0,
       0,     8,     0,     0,    34,    35,     0,     0,     0,     0,
       0,     0,   325,   328,   326,   327,   263,     0,   330,   324,
     323,     0,   292,   301,   308,   321,   189,    97,   189,     0,
       0,     0,     0,    51,    54,    52,    53,   100,    80,   109,
       0,     0,    60,     0,   104,   197,   168,     0,     0,     0,
     168,   154,     0,   223,   267,   268,     0,     0,     0,     0,
     291,     0,     0,     0,     0,   223,   223,     0,     0,   234,
     235,     0,     0,   262,     0,     0,   260,     0,     0,     0,
       0,     0,    80,     0,     0,     0,   168,   168,     4,     0,
       0,    12,     0,     0,     0,     0,    21,   331,   302,   303,
     333,   337,     0,   335,     0,   332,     0,    40,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    50,   105,   106,    61,     0,     0,     0,     0,     0,
       0,     0,     0,    74,     0,    71,     0,     0,     0,     0,
       0,     0,     0,   171,     0,     0,     0,     0,   170,   198,
       0,   173,     0,   185,   145,     0,     0,   171,    12,     0,
     201,     0,     0,     0,   204,   204,     0,   271,   266,   204,
     238,   207,   208,   242,     0,   248,   249,   202,   202,     0,
     257,   261,   252,   239,     0,   265,   253,   254,   255,   256,
      58,     0,    57,   168,   168,   171,   171,    24,     0,    31,
     168,    26,   168,    25,     0,     0,   334,   329,   259,   293,
     294,   295,   296,   297,   298,   299,   300,   306,   304,   305,
     307,     0,   311,     0,   312,     0,   313,     0,   309,     0,
     310,   319,   320,   322,     0,     0,     0,     0,   188,     0,
     191,     0,     0,     0,    99,   102,     0,     0,     0,    50,
     125,     0,   115,   120,     0,     0,     0,    76,     0,    82,
       0,     0,     0,     0,    81,     0,     0,     0,    42,    98,
       0,    98,     0,   133,   107,   108,   103,   112,   113,   111,
     114,     0,     0,   189,     0,   189,     0,     0,   169,     0,
       0,   150,     0,     0,     0,     0,    78,     0,     0,     0,
     285,   284,   223,   289,   290,   279,   269,     0,   241,     0,
       0,     0,     0,   228,     0,   236,     0,     0,     0,     0,
     258,     0,   171,   171,     0,     0,    23,   202,   202,    33,
     171,   171,   338,   336,   316,   317,   318,   314,   315,     0,
     189,     0,   189,     0,   187,     0,    67,    68,   116,   117,
       0,     0,     0,     0,   101,     0,     0,     0,     0,   110,
      80,   168,   168,    83,     0,     0,   168,   168,    92,    98,
     263,     0,     0,    42,    41,    65,     0,    75,    73,     0,
     138,   130,     0,   185,     0,   174,     0,     0,     0,   172,
       0,   182,   177,   148,     0,    12,   146,   147,     0,   155,
       0,   185,     0,     0,   204,   205,   282,   273,   277,     0,
     281,   204,   247,   250,     0,   224,   230,   225,   202,   227,
     237,   246,     0,   244,     0,   264,    80,     0,     0,   185,
     185,    29,    30,   202,     0,     0,     0,     0,   195,     0,
       0,   190,   192,     0,   127,     0,   118,   119,   122,   123,
     121,   124,     0,   171,   171,   168,   168,   171,   171,     0,
       0,     0,     0,     0,     0,    44,     0,     0,    72,   133,
       0,   128,   131,   156,   178,   176,     0,   181,     0,     0,
       0,   186,     0,     0,    79,     0,   152,     0,     0,   288,
       0,   279,   202,   202,     0,     0,   204,   270,     0,   204,
       0,   229,     0,   245,   243,    59,   185,     0,   160,   162,
      32,     0,    27,     0,   194,   193,     0,    69,   126,    77,
       0,     0,   171,   171,     0,     0,     0,     0,    84,    48,
      46,     0,    45,     0,     0,    62,   129,     0,    98,    12,
       0,   175,   184,   183,   179,   151,   185,   149,    12,   204,
     204,   283,   276,   275,   274,   272,   280,   278,   251,   232,
       0,   226,   164,     0,    12,    12,    20,     0,   196,    85,
       0,     0,     0,    87,    88,    93,    94,     0,    43,     0,
       0,    63,   135,     0,   132,     0,   180,   158,     0,   287,
     286,   204,    12,   185,     0,     0,    28,     0,    89,     0,
       0,    47,    49,    98,    98,   142,     0,   157,    12,   153,
     231,     0,   166,   161,   163,    86,     0,    91,     0,    70,
      64,   137,   140,     0,   135,     0,   165,    12,    90,    96,
      95,   142,     0,   142,   159,     0,   139,   134,   136,   167,
     133,     0,   141
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -469,  -469,  -469,  -469,  -469,   -56,   686,  -469,  -469,  -469,
    -469,  -469,  -469,  -469,   690,   -26,  -269,  -469,  -469,   -60,
     689,   -37,   -65,  -469,  -469,  -469,  -348,  -442,  -469,  -469,
    -469,  -469,  -379,  -469,   452,  -196,  -183,  -218,   135,  -469,
     303,  -469,  -469,   153,  -469,   145,   155,   705,   -34,  -469,
    -469,  -469,  -469,  -469,  -469,  -469,  -469,  -469,  -469,   -40,
     -46,   332,   506,  -411,  -468,  -122,  -469,   451,   -15,  -469,
     -57,  -469,  -469,  -268,  -469,  -145,   429,  -469,  -469,   379,
    -469,   -16,  -469,   -24,  -263,   497,  -469,   -10,  -469,  -469,
    -469,  -469,  -469,  -469,  -469,  -469,   312,  -406,   310,  -469,
    -469,  -469,  -469,  -469,   -53,   848,   -47,  -146,   -82,  -469,
     608
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     3,     4,     9,    30,     5,     6,    25,     7,   113,
     206,   449,   554,    34,    36,    37,   387,   651,   492,   370,
      49,    50,   150,   701,   724,   393,   417,   250,   384,   580,
     720,   151,   152,   153,   154,   526,   372,   373,   499,   500,
     501,   658,   760,   725,   591,   741,   742,    41,    42,    26,
     515,   668,   278,   659,   728,   684,   685,   712,   747,    67,
     158,   270,   271,   512,   411,   239,   359,   360,    43,    27,
      46,   163,    47,   421,   422,    88,    89,    90,   432,   433,
      91,   128,    93,   129,   294,   295,    95,   130,   304,    97,
      98,    99,   100,   425,   614,   527,   528,   743,   530,   101,
     102,   103,   104,   171,   388,   132,   133,   134,   135,   212,
     213
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      39,   112,   149,    44,   159,    72,   412,    68,   131,   149,
      63,   496,   106,   498,    56,   467,   240,   423,   280,   529,
     167,   426,    73,    94,   173,   168,   162,   172,   178,   174,
     510,    92,   737,   179,   207,   593,   436,    96,   572,   215,
     193,   397,   398,   399,   400,   149,   407,   463,   274,   584,
     407,   371,   374,   606,   562,   169,   161,   483,    59,   310,
      64,    70,   396,   589,   652,   389,   211,   214,   446,   170,
     208,   209,    28,   394,   395,   653,    73,    29,     8,   149,
     149,   628,   629,   590,   342,   344,   346,   348,   350,   351,
     352,   285,    10,   594,   283,   595,   275,   597,   619,   390,
     511,   297,   298,   738,   572,   264,   408,   464,   695,   585,
    -198,    60,  -198,    65,    65,   157,   263,   491,   311,   276,
     277,    65,   181,   265,   296,    11,   264,   245,   300,   266,
      51,   302,   286,   267,   305,   306,   307,   308,   309,    94,
      52,   620,   272,   182,   265,   183,   634,    92,   635,    12,
     266,    94,    94,    96,   267,   353,   315,   316,   682,    92,
      92,   702,   268,    31,   536,    96,    96,   217,   354,    35,
     603,     1,   470,   269,   542,   544,   259,   260,     2,   337,
     338,   339,   340,   468,   469,   318,   355,    38,   261,   537,
      40,   611,   356,   703,   269,   454,   357,   455,   707,   456,
     538,   457,   612,   458,   218,   529,   484,   654,    73,   262,
     219,   220,   221,   222,   223,   224,   485,   401,    45,    73,
     475,   476,   415,   361,    65,   358,    69,   613,   418,   419,
     420,   414,   477,    74,   375,   385,    38,   199,    57,   200,
     431,   434,   201,   225,   288,   732,   296,   325,   661,   402,
      48,   404,   326,   478,   406,   202,   609,   568,   569,   570,
     571,   203,   169,   617,   216,   204,   186,   187,   205,   444,
     445,   452,   211,   442,   443,   565,   424,   525,    73,   683,
     450,   505,   451,   507,    75,   687,   106,    68,   688,   566,
     567,    63,   354,    53,    73,    32,   441,   256,   257,    33,
      76,    77,    78,   535,   184,    79,    80,   439,    54,   258,
     355,    55,   440,   649,   650,   226,   356,    58,    81,   539,
     357,    82,   227,   228,   495,   185,   497,   186,   187,   149,
     149,   149,   149,   493,   188,   189,   190,   191,   558,   459,
     560,   461,   717,    61,    73,    65,   229,    66,   677,  -263,
      62,   679,   111,    83,   472,   473,    65,    84,   114,   105,
      38,   281,   115,   364,    85,   282,   610,    86,    87,   681,
    -263,   736,  -263,  -263,   296,   177,   533,    71,   502,  -263,
    -263,  -263,  -263,   107,   296,   296,   545,   427,    65,   108,
     551,   552,   272,   427,   428,   480,   547,   548,    94,    75,
     540,   709,   710,   376,   555,   556,    92,   377,   472,   473,
     109,    65,    96,   378,   671,    76,    77,    78,   546,   479,
      79,    80,   472,   473,   581,   230,   231,   232,   721,   149,
     722,   474,   110,    81,   472,   473,    82,   233,   234,   472,
     473,   573,   574,   730,   427,   564,   577,   578,   361,   427,
     638,   623,   182,   379,   183,   586,   624,   380,   235,   602,
     587,    13,    14,   236,    15,   381,   382,   383,   447,   607,
     608,   697,   290,   284,    65,    65,   409,   465,   698,    85,
      16,   622,    86,    87,   164,   524,    17,   155,   165,    18,
      19,   448,    65,    21,   489,   582,   630,   749,   218,   177,
      65,   750,   504,   368,   219,   220,   221,   222,   223,   224,
     218,   143,   144,   145,   146,   369,   219,   220,   221,   222,
     223,   224,    65,    65,   506,   508,   149,   640,   641,   472,
     473,   644,   645,   156,    22,   642,   643,   225,   252,    65,
     157,   557,    23,    24,    65,   160,   559,   664,   166,   225,
      13,    14,   669,    15,   126,   673,   674,   385,    65,   175,
     592,   253,   194,   254,   272,   678,   255,   180,   615,    16,
     616,   615,   196,   752,   502,    17,   195,   218,    18,    19,
      20,   197,    21,   219,   220,   221,   222,   223,   224,   198,
     376,   242,   243,   704,   639,   177,   691,   692,   149,   237,
     378,   238,   670,   705,   291,   292,   249,   706,   251,   244,
     273,   279,   708,   299,   301,   522,   225,   303,   312,   523,
     313,   314,   320,    22,   322,   363,   328,   218,   714,   715,
     362,    23,    24,   219,   220,   221,   222,   223,   224,   218,
     379,   248,   365,   391,   380,   219,   220,   221,   222,   223,
     224,   392,   381,   382,   383,   403,   731,   405,   739,   740,
     136,   137,   410,   149,   149,   413,   225,   416,   437,   430,
     138,   438,   745,   143,   144,   145,   146,   241,   225,   460,
     471,   494,   462,   136,   137,   466,   481,   482,   139,   140,
     486,   755,   503,   138,   487,   141,   488,   513,   142,   516,
     514,   517,   287,   518,   519,   520,   143,   144,   145,   146,
     147,   139,   140,   521,   148,   534,   218,   549,   141,   531,
     550,   142,   219,   220,   221,   222,   223,   224,   553,   143,
     144,   145,   146,   147,   289,   563,   575,   192,   576,   579,
     583,   588,   598,   218,   596,   502,   116,   599,   600,   219,
     220,   221,   222,   223,   224,   225,   601,   117,   118,   604,
     625,   605,   116,   626,   627,   119,   210,   120,    73,   631,
     121,   632,   648,   117,   118,   122,   123,   124,   125,   126,
     633,   119,   225,   120,   293,   636,   121,   647,   116,   660,
     127,   122,   123,   124,   125,   126,   655,   637,   657,   117,
     118,   665,   662,   666,   116,   663,   127,   119,   675,   120,
     435,   667,   121,   680,   689,   117,   118,   122,   123,   124,
     125,   126,   690,   119,   686,   120,   541,   693,   121,   694,
     116,   699,   127,   122,   123,   124,   125,   126,   700,   713,
     696,   117,   118,   711,   716,   718,   116,   723,   127,   119,
     719,   120,   543,   726,   121,   727,   729,   117,   118,   122,
     123,   124,   125,   126,   733,   119,   734,   120,   735,   744,
     121,   746,   116,   748,   127,   122,   123,   124,   125,   126,
     751,   754,   757,   117,   118,   759,   762,   319,   116,   317,
     127,   119,   656,   386,   323,   761,   121,   753,   758,   117,
     118,   122,   123,   124,   125,   126,   756,   119,   321,   386,
     116,   646,   121,   509,   561,   621,   127,   122,   123,   124,
     125,   490,   341,   672,   532,   676,   116,     0,     0,   119,
       0,   120,   127,   453,   121,     0,     0,     0,   343,   122,
     123,   124,   125,   126,     0,   119,     0,   120,   116,     0,
     121,     0,     0,     0,   127,   122,   123,   124,   125,   126,
     345,     0,     0,     0,   116,     0,     0,   119,     0,   120,
     127,     0,   121,     0,     0,     0,   347,   122,   123,   124,
     125,   126,     0,   119,     0,   120,   116,     0,   121,     0,
       0,     0,   127,   122,   123,   124,   125,   126,   349,     0,
       0,     0,   116,     0,     0,   119,     0,   120,   127,     0,
     121,     0,     0,     0,     0,   122,   123,   124,   125,   126,
       0,   119,     0,   120,     0,     0,   121,     0,     0,     0,
     127,   122,   123,   124,   125,   126,     0,     0,     0,     0,
       0,   218,     0,     0,     0,     0,   127,   219,   220,   221,
     222,   223,   224,   218,     0,     0,     0,     0,     0,   219,
     220,   221,   222,   223,   224,   324,   329,   330,   331,   332,
     333,   334,   335,   336,     0,   218,   327,     0,     0,     0,
     225,   219,   220,   221,   222,   223,   224,     0,   218,     0,
       0,     0,   225,   429,   219,   220,   221,   222,   223,   224,
    -263,   218,     0,     0,     0,     0,   618,   219,   220,   221,
     222,   223,   224,     0,   225,     0,     0,     0,     0,     0,
       0,  -263,   176,  -263,  -263,     0,   177,   225,     0,     0,
    -263,  -263,  -263,  -263,   -50,   -50,     0,     0,  -259,     0,
     225,     0,     0,     0,   -50,   -50,   -50,     0,     0,   246,
       0,     0,   247,     0,   248,     0,   -50,     0,     0,  -259,
       0,  -259,  -259,     0,   438,   366,   367,   -50,  -259,  -259,
    -259,  -259,   139,   140,     0,   368,     0,     0,     0,     0,
       0,     0,   142,   143,   144,   145,   146,   369,     0,     0,
     143,   144,   145,   146,   147
};

static const yytype_int16 yycheck[] =
{
      15,    57,    62,    18,    69,    45,   275,    41,    61,    69,
      36,   390,    49,   392,    29,   363,   138,   285,   163,   425,
      77,   289,     5,    47,    81,    78,    72,    80,    85,    82,
       6,    47,     5,    86,   116,   503,   299,    47,   480,   121,
     105,   259,   260,   261,   262,   105,     5,     5,     5,     5,
       5,   247,   248,   521,   465,    79,     5,     5,     5,     5,
       5,     5,   258,     5,    65,    28,   119,   120,     5,    79,
     117,   118,    59,   256,   257,    76,     5,    64,    76,   139,
     140,   549,   550,    25,   230,   231,   232,   233,   234,   235,
     236,    17,    76,   504,    23,   506,    53,   508,    17,    62,
      76,    84,    85,    76,   546,     8,    65,    65,    63,    65,
      58,    58,    60,    58,    58,    64,   156,   386,    64,    76,
     160,    58,    38,    26,   177,     0,     8,   142,   181,    32,
      26,   184,    58,    36,   187,   188,   189,   190,   191,   163,
      36,    60,   157,    59,    26,    61,   557,   163,   559,    59,
      32,   175,   176,   163,    36,   237,   196,   197,   626,   175,
     176,    28,    65,     5,     5,   175,   176,     5,     8,    76,
     518,     4,   368,    76,   437,   438,    44,    45,    11,   226,
     227,   228,   229,   366,   367,   200,    26,    76,    56,    30,
      76,     5,    32,    60,    76,   341,    36,   343,   666,   345,
      41,   347,    16,   349,    42,   611,    26,   586,     5,    77,
      48,    49,    50,    51,    52,    53,    36,   263,    76,     5,
      44,    45,   278,   238,    58,    65,    60,    41,   281,   282,
     283,   277,    56,    30,   249,   250,    76,     8,     5,    10,
     297,   298,    13,    81,    30,   713,   299,    58,   596,   264,
      76,   266,    63,    77,   269,    26,   524,   475,   476,   477,
     478,    32,   286,   531,    59,    36,    61,    62,    39,   315,
     316,   324,   325,   313,   314,   471,   286,   422,     5,   627,
     320,   403,   322,   405,     3,   633,   323,   321,   636,   472,
     473,   317,     8,    76,     5,    72,   311,    54,    55,    76,
      19,    20,    21,    30,    38,    24,    25,    58,    76,    66,
      26,    76,    63,   582,   583,    47,    32,    12,    37,    30,
      36,    40,    54,    55,   389,    59,   391,    61,    62,   389,
     390,   391,   392,   386,    68,    69,    70,    71,   460,   354,
     462,   356,   690,    53,     5,    58,    78,    59,   616,    38,
      60,   619,    65,    72,    54,    55,    58,    76,    72,    53,
      76,    38,    76,    65,    83,    42,    66,    86,    87,    30,
      59,   719,    61,    62,   427,    64,   429,    59,   393,    68,
      69,    70,    71,    76,   437,   438,   439,    58,    58,    76,
     447,   448,   407,    58,    65,    65,   442,   443,   422,     3,
      65,   669,   670,    26,   450,   451,   422,    30,    54,    55,
      59,    58,   422,    36,   610,    19,    20,    21,    65,    65,
      24,    25,    54,    55,   489,    44,    45,    46,   697,   489,
     699,    63,    59,    37,    54,    55,    40,    56,    57,    54,
      55,   481,   482,   711,    58,    65,   486,   487,   463,    58,
      65,    65,    59,    76,    61,    58,    65,    80,    77,   515,
      63,     7,     8,    82,    10,    88,    89,    90,    14,   522,
     523,    58,    76,    17,    58,    58,    60,    60,    65,    83,
      26,   538,    86,    87,    72,    17,    32,    76,    76,    35,
      36,    37,    58,    39,    60,    60,   553,    72,    42,    64,
      58,    76,    60,    64,    48,    49,    50,    51,    52,    53,
      42,    72,    73,    74,    75,    76,    48,    49,    50,    51,
      52,    53,    58,    58,    60,    60,   586,   573,   574,    54,
      55,   577,   578,    76,    80,   575,   576,    81,     6,    58,
      64,    60,    88,    89,    58,    76,    60,   600,    76,    81,
       7,     8,    17,    10,    76,   612,   613,   572,    58,    60,
      60,    29,    59,    31,   579,   618,    34,    76,    58,    26,
      60,    58,    76,    60,   589,    32,    59,    42,    35,    36,
      37,    76,    39,    48,    49,    50,    51,    52,    53,    59,
      26,   139,   140,   658,    30,    64,   642,   643,   658,    79,
      36,    64,    17,   659,   175,   176,    64,   660,    53,    76,
       5,    60,   668,    64,    76,    18,    81,    76,     5,    22,
      76,    76,    76,    80,    76,    60,    76,    42,   684,   685,
      28,    88,    89,    48,    49,    50,    51,    52,    53,    42,
      76,    64,    76,    28,    80,    48,    49,    50,    51,    52,
      53,    28,    88,    89,    90,    76,   712,    76,   723,   724,
      26,    27,     9,   723,   724,    76,    81,    76,    64,    91,
      36,    64,   728,    72,    73,    74,    75,    76,    81,    76,
      64,     5,    76,    26,    27,    76,    76,    76,    54,    55,
      76,   747,     5,    36,    76,    61,    76,     5,    64,     5,
      76,     5,    28,    60,     5,    59,    72,    73,    74,    75,
      76,    54,    55,     5,    80,    76,    42,     5,    61,    41,
       5,    64,    48,    49,    50,    51,    52,    53,    15,    72,
      73,    74,    75,    76,    33,    28,    76,    80,    76,    62,
      58,    30,    28,    42,    60,   760,    43,    59,    53,    48,
      49,    50,    51,    52,    53,    81,     5,    54,    55,    76,
       5,     9,    43,     5,    60,    62,    63,    64,     5,    30,
      67,     5,     5,    54,    55,    72,    73,    74,    75,    76,
      60,    62,    81,    64,    65,    60,    67,    60,    43,    53,
      87,    72,    73,    74,    75,    76,    28,    76,    76,    54,
      55,     5,    76,     5,    43,    76,    87,    62,    30,    64,
      65,     5,    67,    76,     5,    54,    55,    72,    73,    74,
      75,    76,    60,    62,    59,    64,    65,     5,    67,     5,
      43,    60,    87,    72,    73,    74,    75,    76,    63,     5,
      76,    54,    55,    17,     5,     5,    43,    28,    87,    62,
      60,    64,    65,    76,    67,     5,     5,    54,    55,    72,
      73,    74,    75,    76,     5,    62,     5,    64,     5,    28,
      67,     5,    43,     5,    87,    72,    73,    74,    75,    76,
       5,     5,    64,    54,    55,     5,    65,   201,    43,   199,
      87,    62,   589,    64,   205,   760,    67,   744,   753,    54,
      55,    72,    73,    74,    75,    76,   751,    62,   203,    64,
      43,   579,    67,   407,   463,   536,    87,    72,    73,    74,
      75,    76,    55,   611,   427,   615,    43,    -1,    -1,    62,
      -1,    64,    87,   325,    67,    -1,    -1,    -1,    55,    72,
      73,    74,    75,    76,    -1,    62,    -1,    64,    43,    -1,
      67,    -1,    -1,    -1,    87,    72,    73,    74,    75,    76,
      55,    -1,    -1,    -1,    43,    -1,    -1,    62,    -1,    64,
      87,    -1,    67,    -1,    -1,    -1,    55,    72,    73,    74,
      75,    76,    -1,    62,    -1,    64,    43,    -1,    67,    -1,
      -1,    -1,    87,    72,    73,    74,    75,    76,    55,    -1,
      -1,    -1,    43,    -1,    -1,    62,    -1,    64,    87,    -1,
      67,    -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,
      -1,    62,    -1,    64,    -1,    -1,    67,    -1,    -1,    -1,
      87,    72,    73,    74,    75,    76,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    87,    48,    49,    50,
      51,    52,    53,    42,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    66,   218,   219,   220,   221,
     222,   223,   224,   225,    -1,    42,    65,    -1,    -1,    -1,
      81,    48,    49,    50,    51,    52,    53,    -1,    42,    -1,
      -1,    -1,    81,    60,    48,    49,    50,    51,    52,    53,
      38,    42,    -1,    -1,    -1,    -1,    60,    48,    49,    50,
      51,    52,    53,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    59,    60,    61,    62,    -1,    64,    81,    -1,    -1,
      68,    69,    70,    71,    44,    45,    -1,    -1,    38,    -1,
      81,    -1,    -1,    -1,    54,    55,    56,    -1,    -1,    59,
      -1,    -1,    62,    -1,    64,    -1,    66,    -1,    -1,    59,
      -1,    61,    62,    -1,    64,    54,    55,    77,    68,    69,
      70,    71,    54,    55,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    72,    73,    74,    75,    76,    -1,    -1,
      72,    73,    74,    75,    76
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    11,    93,    94,    97,    98,   100,    76,    95,
      76,     0,    59,     7,     8,    10,    26,    32,    35,    36,
      37,    39,    80,    88,    89,    99,   141,   161,    59,    64,
      96,     5,    72,    76,   105,    76,   106,   107,    76,   160,
      76,   139,   140,   160,   160,    76,   162,   164,    76,   112,
     113,    26,    36,    76,    76,    76,   160,     5,    12,     5,
      58,    53,    60,   107,     5,    58,    59,   151,   140,    60,
       5,    59,   151,     5,    30,     3,    19,    20,    21,    24,
      25,    37,    40,    72,    76,    83,    86,    87,   167,   168,
     169,   172,   173,   174,   175,   178,   179,   181,   182,   183,
     184,   191,   192,   193,   194,    53,   113,    76,    76,    59,
      59,    65,    97,   101,    72,    76,    43,    54,    55,    62,
      64,    67,    72,    73,    74,    75,    76,    87,   173,   175,
     179,   196,   197,   198,   199,   200,    26,    27,    36,    54,
      55,    61,    64,    72,    73,    74,    75,    76,    80,   111,
     114,   123,   124,   125,   126,    76,    76,    64,   152,   114,
      76,     5,   152,   163,    72,    76,    76,   162,   196,   175,
     179,   195,   196,   162,   196,    60,    60,    64,   162,   196,
      76,    38,    59,    61,    38,    59,    61,    62,    68,    69,
      70,    71,    80,   114,    59,    59,    76,    76,    59,     8,
      10,    13,    26,    32,    36,    39,   102,   200,   198,   198,
      63,   196,   201,   202,   196,   200,    59,     5,    42,    48,
      49,    50,    51,    52,    53,    81,    47,    54,    55,    78,
      44,    45,    46,    56,    57,    77,    82,    79,    64,   157,
     157,    76,   126,   126,    76,   160,    59,    62,    64,    64,
     119,    53,     6,    29,    31,    34,    54,    55,    66,    44,
      45,    56,    77,   151,     8,    26,    32,    36,    65,    76,
     153,   154,   160,     5,     5,    53,    76,   151,   144,    60,
     167,    38,    42,    23,    17,    17,    58,    28,    30,    33,
      76,   168,   168,    65,   176,   177,   196,    84,    85,    64,
     196,    76,   196,    76,   180,   196,   196,   196,   196,   196,
       5,    64,     5,    76,    76,   151,   151,   106,   160,    98,
      76,   139,    76,   112,    66,    58,    63,    65,    76,   197,
     197,   197,   197,   197,   197,   197,   197,   198,   198,   198,
     198,    55,   199,    55,   199,    55,   199,    55,   199,    55,
     199,   199,   199,   200,     8,    26,    32,    36,    65,   158,
     159,   160,    28,    60,    65,    76,    54,    55,    64,    76,
     111,   127,   128,   129,   127,   160,    26,    30,    36,    76,
      80,    88,    89,    90,   120,   160,    64,   108,   196,    28,
      62,    28,    28,   117,   128,   128,   127,   129,   129,   129,
     129,   152,   160,    76,   160,    76,   160,     5,    65,    60,
       9,   156,   108,    76,   152,    97,    76,   118,   196,   196,
     196,   165,   166,   165,   179,   185,   165,    58,    65,    60,
      91,   162,   170,   171,   162,    65,   176,    64,    64,    58,
      63,   160,   151,   151,   152,   152,     5,    14,    37,   103,
     151,   151,   196,   202,   199,   199,   199,   199,   199,   160,
      76,   160,    76,     5,    65,    60,    76,   118,   128,   128,
     127,    64,    54,    55,    63,    44,    45,    56,    77,    65,
      65,    76,    76,     5,    26,    36,    76,    76,    76,    60,
      76,   108,   110,   196,     5,   114,   124,   114,   124,   130,
     131,   132,   160,     5,    60,   157,    60,   157,    60,   154,
       6,    76,   155,     5,    76,   142,     5,     5,    60,     5,
      59,     5,    18,    22,    17,   167,   127,   187,   188,   189,
     190,    41,   177,   196,    76,    30,     5,    30,    41,    30,
      65,    65,   176,    65,   176,   196,    65,   152,   152,     5,
       5,   162,   162,    15,   104,   152,   152,    60,   157,    60,
     157,   159,   155,    28,    65,   127,   128,   128,   129,   129,
     129,   129,   119,   151,   151,    76,    76,   151,   151,    62,
     121,   114,    60,    58,     5,    65,    58,    63,    30,     5,
      25,   136,    60,   156,   155,   155,    60,   155,    28,    59,
      53,     5,    97,   118,    76,     9,   156,   196,   196,   165,
      66,     5,    16,    41,   186,    58,    60,   165,    60,    17,
      60,   171,   162,    65,    65,     5,     5,    60,   156,   156,
     162,    30,     5,    60,   155,   155,    60,    76,    65,    30,
     152,   152,   151,   151,   152,   152,   153,    60,     5,   108,
     108,   109,    65,    76,   124,    28,   132,    76,   133,   145,
      53,   118,    76,    76,   196,     5,     5,     5,   143,    17,
      17,   127,   188,   162,   162,    30,   190,   165,   196,   165,
      76,    30,   156,   118,   147,   148,    59,   118,   118,     5,
      60,   152,   152,     5,     5,    63,    76,    58,    65,    60,
      63,   115,    28,    60,   114,    97,   196,   156,    97,   165,
     165,    17,   149,     5,    97,    97,     5,   118,     5,    60,
     122,   108,   108,    28,   116,   135,    76,     5,   146,     5,
     165,    97,   156,     5,     5,     5,   118,     5,    76,   114,
     114,   137,   138,   189,    28,    97,     5,   150,     5,    72,
      76,     5,    60,   135,     5,    97,   138,    64,   137,     5,
     134,   130,    65
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    92,    93,    93,    94,    94,    95,    95,    96,    96,
      97,    98,    98,    99,    99,    99,    99,    99,    99,    99,
     100,   101,   101,   102,   102,   102,   102,   102,   102,   103,
     103,   103,   104,   104,   105,   105,   105,   105,   106,   106,
     107,   107,   108,   108,   108,   108,   109,   109,   110,   110,
     111,   111,   111,   111,   111,   112,   112,   113,   113,   113,
     114,   114,   115,   116,   114,   114,   114,   114,   114,   114,
     114,   117,   114,   114,   114,   114,   114,   114,   118,   118,
     119,   119,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   121,   121,   122,   122,   122,   123,   123,   124,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   126,   126,   127,   127,   127,   127,   127,
     128,   128,   128,   128,   128,   129,   129,   129,   130,   131,
     131,   133,   132,   132,   134,   135,   136,   136,   136,   137,
     137,   138,   138,   139,   139,   140,   140,   140,   141,   141,
     142,   141,   143,   141,   144,   141,   145,   141,   146,   141,
     147,   141,   148,   141,   149,   141,   150,   141,   151,   152,
     152,   152,   153,   153,   154,   154,   154,   154,   154,   154,
     154,   154,   155,   155,   155,   156,   156,   157,   157,   157,
     158,   158,   159,   159,   159,   159,   159,   160,   160,   161,
     163,   162,   164,   162,   166,   165,   167,   167,   167,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   169,   169,   169,   169,   170,   170,
     170,   171,   171,   172,   172,   173,   173,   173,   174,   174,
     174,   175,   175,   175,   175,   175,   175,   176,   176,   177,
     177,   177,   178,   178,   178,   178,   178,   178,   179,   179,
     179,   179,   179,   179,   180,   180,   181,   182,   182,   183,
     183,   185,   184,   186,   186,   186,   187,   187,   188,   188,
     189,   189,   190,   190,   191,   192,   193,   193,   193,   194,
     195,   195,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   197,   197,   197,   197,   197,   197,   197,   198,   198,
     198,   198,   198,   198,   198,   198,   198,   198,   198,   198,
     198,   199,   199,   200,   200,   200,   200,   200,   200,   200,
     200,   200,   200,   200,   200,   201,   201,   202,   202
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     6,     2,     1,     3,     3,     0,
       2,     2,     0,     3,     2,     2,     2,     1,     3,     3,
      11,     2,     0,     3,     2,     2,     2,     5,     7,     2,
       2,     0,     2,     0,     3,     3,     1,     1,     2,     1,
       4,     6,     1,     5,     3,     4,     1,     3,     3,     5,
       1,     1,     1,     1,     1,     2,     1,     4,     4,     7,
       1,     2,     0,     0,     9,     4,     2,     4,     4,     6,
       9,     0,     5,     4,     2,     4,     3,     6,     1,     3,
       0,     2,     1,     2,     4,     5,     7,     5,     5,     6,
       8,     7,     0,     3,     0,     3,     3,     1,     0,     3,
       1,     4,     3,     3,     1,     2,     2,     3,     3,     1,
       4,     3,     3,     3,     3,     1,     2,     2,     3,     3,
       1,     3,     3,     3,     3,     1,     4,     3,     2,     3,
       1,     0,     4,     0,     0,     0,     7,     5,     0,     3,
       1,     6,     0,     2,     1,     4,     6,     6,     7,     9,
       0,     9,     0,    11,     0,     7,     0,    11,     0,    13,
       0,    11,     0,    11,     0,    12,     0,    14,     0,     3,
       2,     0,     3,     1,     3,     5,     4,     3,     4,     5,
       6,     4,     1,     3,     3,     0,     3,     3,     2,     0,
       3,     1,     3,     4,     4,     3,     5,     3,     1,     3,
       0,     4,     0,     2,     0,     2,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     5,     5,     7,     5,     1,     3,
       2,     6,     4,     1,     2,     2,     4,     5,     1,     3,
       1,     4,     3,     6,     5,     6,     5,     3,     1,     1,
       3,     5,     3,     3,     3,     3,     3,     3,     4,     3,
       2,     3,     2,     1,     3,     1,     3,     2,     2,     4,
       6,     0,     7,     0,     2,     2,     3,     1,     3,     0,
       3,     1,     1,     3,     4,     4,     8,     8,     6,     4,
       3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     2,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     4,     4,     4,     4,     4,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       1,     2,     2,     2,     3,     1,     3,     1,     3
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
#line 204 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2031 "pascal.tab.c"
    break;

  case 3: /* compilation_unit: unit  */
#line 205 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2037 "pascal.tab.c"
    break;

  case 4: /* program: PROGRAMSY program_name file_id_list_opt SEMICOLON block PERIOD  */
#line 208 "pascal.y"
                                                                   { pascal_stamp_dialect((yyvsp[-4].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), (yyvsp[-4].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2043 "pascal.tab.c"
    break;

  case 5: /* program: block PERIOD  */
#line 209 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PROGRAM), (yyvsp[-1].node)); pascal_stamp_dialect((yyval.node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2049 "pascal.tab.c"
    break;

  case 6: /* program_name: IDENT  */
#line 212 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2055 "pascal.tab.c"
    break;

  case 7: /* program_name: program_name PERIOD IDENT  */
#line 213 "pascal.y"
                               { (yyval.node) = (yyvsp[-2].node); }
#line 2061 "pascal.tab.c"
    break;

  case 8: /* file_id_list_opt: LPARENT id_list RPARENT  */
#line 216 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2067 "pascal.tab.c"
    break;

  case 9: /* file_id_list_opt: %empty  */
#line 217 "pascal.y"
     { (yyval.node) = NULL; }
#line 2073 "pascal.tab.c"
    break;

  case 10: /* block: decl_part_list body  */
#line 220 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2079 "pascal.tab.c"
    break;

  case 11: /* decl_part_list: decl_part_list decl_part  */
#line 223 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2085 "pascal.tab.c"
    break;

  case 12: /* decl_part_list: %empty  */
#line 224 "pascal.y"
     { (yyval.node) = pt_new(TT_BLOCK); }
#line 2091 "pascal.tab.c"
    break;

  case 13: /* decl_part: LABELSY label_list SEMICOLON  */
#line 227 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 2097 "pascal.tab.c"
    break;

  case 14: /* decl_part: CONSTSY const_decl_list  */
#line 228 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2103 "pascal.tab.c"
    break;

  case 15: /* decl_part: TYPESY type_decl_list  */
#line 229 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2109 "pascal.tab.c"
    break;

  case 16: /* decl_part: VARSY var_decl_list  */
#line 230 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2115 "pascal.tab.c"
    break;

  case 17: /* decl_part: procedure_decl  */
#line 231 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2121 "pascal.tab.c"
    break;

  case 18: /* decl_part: USESSY id_list SEMICOLON  */
#line 232 "pascal.y"
                              { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2127 "pascal.tab.c"
    break;

  case 19: /* decl_part: EXPORTSSY id_list SEMICOLON  */
#line 233 "pascal.y"
                                 { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "exports"), (yyvsp[-1].node)); }
#line 2133 "pascal.tab.c"
    break;

  case 20: /* unit: UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD  */
#line 236 "pascal.y"
                                                                                                                     { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), (yyvsp[-9].node)), (yyvsp[-6].node)), pt_sval(pt_retag((yyvsp[-4].node), TT_PART), "implementation")), (yyvsp[-3].node)), (yyvsp[-2].node)); pt_flush_pragmas((yyval.node)); pascal_prog_result = (yyval.node); }
#line 2139 "pascal.tab.c"
    break;

  case 21: /* intf_part_list: intf_part_list intf_part  */
#line 239 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2145 "pascal.tab.c"
    break;

  case 22: /* intf_part_list: %empty  */
#line 240 "pascal.y"
     { (yyval.node) = pt_sval(pt_new(TT_PART), "interface"); }
#line 2151 "pascal.tab.c"
    break;

  case 23: /* intf_part: USESSY id_list SEMICOLON  */
#line 243 "pascal.y"
                             { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "uses"), (yyvsp[-1].node)); }
#line 2157 "pascal.tab.c"
    break;

  case 24: /* intf_part: CONSTSY const_decl_list  */
#line 244 "pascal.y"
                             { (yyval.node) = (yyvsp[0].node); }
#line 2163 "pascal.tab.c"
    break;

  case 25: /* intf_part: TYPESY type_decl_list  */
#line 245 "pascal.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2169 "pascal.tab.c"
    break;

  case 26: /* intf_part: VARSY var_decl_list  */
#line 246 "pascal.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2175 "pascal.tab.c"
    break;

  case 27: /* intf_part: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 247 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2181 "pascal.tab.c"
    break;

  case 28: /* intf_part: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 248 "pascal.y"
                                                                             { pascal_note_type_ident((yyvsp[-1].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2187 "pascal.tab.c"
    break;

  case 29: /* init_opt: INITIALIZATIONSY statement_list  */
#line 251 "pascal.y"
                                    { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2193 "pascal.tab.c"
    break;

  case 30: /* init_opt: BEGINSY statement_list  */
#line 252 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "initialization"), (yyvsp[0].node)); }
#line 2199 "pascal.tab.c"
    break;

  case 31: /* init_opt: %empty  */
#line 253 "pascal.y"
     { (yyval.node) = NULL; }
#line 2205 "pascal.tab.c"
    break;

  case 32: /* fini_opt: FINALIZATIONSY statement_list  */
#line 256 "pascal.y"
                                  { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "finalization"), (yyvsp[0].node)); }
#line 2211 "pascal.tab.c"
    break;

  case 33: /* fini_opt: %empty  */
#line 257 "pascal.y"
     { (yyval.node) = NULL; }
#line 2217 "pascal.tab.c"
    break;

  case 34: /* label_list: label_list COMMA INTCONST  */
#line 260 "pascal.y"
                              { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2223 "pascal.tab.c"
    break;

  case 35: /* label_list: label_list COMMA IDENT  */
#line 261 "pascal.y"
                            { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2229 "pascal.tab.c"
    break;

  case 36: /* label_list: INTCONST  */
#line 262 "pascal.y"
              { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2235 "pascal.tab.c"
    break;

  case 37: /* label_list: IDENT  */
#line 263 "pascal.y"
           { (yyval.node) = pt_part("label", (yyvsp[0].node)); }
#line 2241 "pascal.tab.c"
    break;

  case 38: /* const_decl_list: const_decl_list const_decl  */
#line 266 "pascal.y"
                               { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2247 "pascal.tab.c"
    break;

  case 39: /* const_decl_list: const_decl  */
#line 267 "pascal.y"
                { (yyval.node) = pt_part("const", (yyvsp[0].node)); }
#line 2253 "pascal.tab.c"
    break;

  case 40: /* const_decl: IDENT EQOP expression SEMICOLON  */
#line 270 "pascal.y"
                                    { (yyval.node) = pt_decl("const", (yyvsp[-3].node), (yyvsp[-1].node)); cenv_note((yyvsp[-3].node)->v.sval, (yyvsp[-1].node)); }
#line 2259 "pascal.tab.c"
    break;

  case 41: /* const_decl: IDENT COLON type EQOP typed_value SEMICOLON  */
#line 271 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_decl("tconst", pt_add(pt_new(TT_VLIST), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2265 "pascal.tab.c"
    break;

  case 42: /* typed_value: expression  */
#line 274 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2271 "pascal.tab.c"
    break;

  case 43: /* typed_value: LPARENT typed_value COMMA typed_value_list RPARENT  */
#line 275 "pascal.y"
                                                        { (yyval.node) = pt_cat(pt_add(pt_sval(pt_new(TT_VLIST), "aggregate"), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2277 "pascal.tab.c"
    break;

  case 44: /* typed_value: LPARENT field_value_list RPARENT  */
#line 276 "pascal.y"
                                      { (yyval.node) = (yyvsp[-1].node); }
#line 2283 "pascal.tab.c"
    break;

  case 45: /* typed_value: LPARENT field_value_list SEMICOLON RPARENT  */
#line 277 "pascal.y"
                                                { (yyval.node) = (yyvsp[-2].node); }
#line 2289 "pascal.tab.c"
    break;

  case 46: /* typed_value_list: typed_value  */
#line 280 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2295 "pascal.tab.c"
    break;

  case 47: /* typed_value_list: typed_value_list COMMA typed_value  */
#line 281 "pascal.y"
                                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2301 "pascal.tab.c"
    break;

  case 48: /* field_value_list: IDENT COLON typed_value  */
#line 284 "pascal.y"
                            { (yyval.node) = pt_add(pt_sval(pt_new(TT_VLIST), "record aggregate"), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2307 "pascal.tab.c"
    break;

  case 49: /* field_value_list: field_value_list SEMICOLON IDENT COLON typed_value  */
#line 285 "pascal.y"
                                                        { (yyval.node) = pt_add((yyvsp[-4].node), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 2313 "pascal.tab.c"
    break;

  case 50: /* scalar_constant: IDENT  */
#line 288 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2319 "pascal.tab.c"
    break;

  case 51: /* scalar_constant: INTCONST  */
#line 289 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2325 "pascal.tab.c"
    break;

  case 52: /* scalar_constant: REALCONST  */
#line 290 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 2331 "pascal.tab.c"
    break;

  case 53: /* scalar_constant: STRINGCONST  */
#line 291 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2337 "pascal.tab.c"
    break;

  case 54: /* scalar_constant: CHARCODE  */
#line 292 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 2343 "pascal.tab.c"
    break;

  case 55: /* type_decl_list: type_decl_list type_decl  */
#line 295 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2349 "pascal.tab.c"
    break;

  case 56: /* type_decl_list: type_decl  */
#line 296 "pascal.y"
               { (yyval.node) = pt_part("type", (yyvsp[0].node)); }
#line 2355 "pascal.tab.c"
    break;

  case 57: /* type_decl: IDENT EQOP type SEMICOLON  */
#line 299 "pascal.y"
                              { pascal_note_type_decl((yyvsp[-3].node)); (yyval.node) = pt_decl("type", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2361 "pascal.tab.c"
    break;

  case 58: /* type_decl: IDENT EQOP CLASSSY SEMICOLON  */
#line 300 "pascal.y"
                                  { (yyval.node) = pt_decl("type", (yyvsp[-3].node), pt_sval(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-1].node)), "forward")); }
#line 2367 "pascal.tab.c"
    break;

  case 59: /* type_decl: IDENT EQOP CLASSSY LPARENT id_list RPARENT SEMICOLON  */
#line 301 "pascal.y"
                                                          { (yyval.node) = pt_decl("type", (yyvsp[-6].node), pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-4].node)), pt_retag((yyvsp[-2].node), TT_ARGS)), pt_new(TT_VLIST))); }
#line 2373 "pascal.tab.c"
    break;

  case 60: /* type: simple_type  */
#line 304 "pascal.y"
                { (yyval.node) = (yyvsp[0].node); }
#line 2379 "pascal.tab.c"
    break;

  case 61: /* type: ARROW IDENT  */
#line 305 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PTR_TYPE), (yyvsp[0].node)); }
#line 2385 "pascal.tab.c"
    break;

  case 62: /* $@1: %empty  */
#line 306 "pascal.y"
                                                       { }
#line 2391 "pascal.tab.c"
    break;

  case 63: /* $@2: %empty  */
#line 306 "pascal.y"
                                                           { }
#line 2397 "pascal.tab.c"
    break;

  case 64: /* type: packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY $@1 $@2 type  */
#line 306 "pascal.y"
                                                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[0].node)); }
#line 2403 "pascal.tab.c"
    break;

  case 65: /* type: packed_opt ARRAYSY OFSY type  */
#line 307 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2409 "pascal.tab.c"
    break;

  case 66: /* type: PROCEDURESY pf_params  */
#line 308 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[0].node)); }
#line 2415 "pascal.tab.c"
    break;

  case 67: /* type: PROCEDURESY pf_params OFSY IDENT  */
#line 309 "pascal.y"
                                      { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_sval(pt_new(TT_DECL), "proctype"), (yyvsp[-2].node)); }
#line 2421 "pascal.tab.c"
    break;

  case 68: /* type: FUNCTIONSY pf_params COLON result_type  */
#line 310 "pascal.y"
                                            { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2427 "pascal.tab.c"
    break;

  case 69: /* type: FUNCTIONSY pf_params COLON result_type OFSY IDENT  */
#line 311 "pascal.y"
                                                       { if (strcasecmp((yyvsp[0].node)->v.sval, "object")) { pascal_yyerror("syntax error"); YYERROR; } pascal_note_type_ident((yyvsp[-2].node)); (yyval.node) = pt_add(pt_add(pt_sval(pt_new(TT_DECL), "functype"), (yyvsp[-4].node)), (yyvsp[-2].node)); }
#line 2433 "pascal.tab.c"
    break;

  case 70: /* type: packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type  */
#line 312 "pascal.y"
                                                                              { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[-8].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2439 "pascal.tab.c"
    break;

  case 71: /* $@3: %empty  */
#line 313 "pascal.y"
                         { }
#line 2445 "pascal.tab.c"
    break;

  case 72: /* type: packed_opt RECORDSY $@3 record_body ENDSY  */
#line 313 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_RECORD), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2451 "pascal.tab.c"
    break;

  case 73: /* type: packed_opt SETSY OFSY simple_type  */
#line 314 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_SET_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2457 "pascal.tab.c"
    break;

  case 74: /* type: packed_opt FILESY  */
#line 315 "pascal.y"
                       { (yyval.node) = pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-1].node)); }
#line 2463 "pascal.tab.c"
    break;

  case 75: /* type: packed_opt FILESY OFSY type  */
#line 316 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FILE_TYPE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2469 "pascal.tab.c"
    break;

  case 76: /* type: CLASSSY class_members ENDSY  */
#line 317 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 2475 "pascal.tab.c"
    break;

  case 77: /* type: CLASSSY LPARENT id_list RPARENT class_members ENDSY  */
#line 318 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CLASS_DECL), (yyvsp[-5].node)), pt_retag((yyvsp[-3].node), TT_ARGS)), (yyvsp[-1].node)); }
#line 2481 "pascal.tab.c"
    break;

  case 78: /* result_type: IDENT  */
#line 321 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2487 "pascal.tab.c"
    break;

  case 79: /* result_type: IDENT PERIOD IDENT  */
#line 322 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2493 "pascal.tab.c"
    break;

  case 80: /* class_members: %empty  */
#line 325 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2499 "pascal.tab.c"
    break;

  case 81: /* class_members: class_members class_member  */
#line 326 "pascal.y"
                                { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2505 "pascal.tab.c"
    break;

  case 82: /* class_member: IDENT  */
#line 329 "pascal.y"
          { (yyval.node) = pt_retag((yyvsp[0].node), TT_KEYWORD); }
#line 2511 "pascal.tab.c"
    break;

  case 83: /* class_member: IDENT SEMICOLON  */
#line 330 "pascal.y"
                     { (yyval.node) = pt_retag((yyvsp[-1].node), TT_KEYWORD); }
#line 2517 "pascal.tab.c"
    break;

  case 84: /* class_member: id_list COLON type SEMICOLON  */
#line 331 "pascal.y"
                                  { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2523 "pascal.tab.c"
    break;

  case 85: /* class_member: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 332 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2529 "pascal.tab.c"
    break;

  case 86: /* class_member: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 333 "pascal.y"
                                                                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2535 "pascal.tab.c"
    break;

  case 87: /* class_member: CONSTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 334 "pascal.y"
                                                              { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "constructor"); }
#line 2541 "pascal.tab.c"
    break;

  case 88: /* class_member: DESTRUCTORSY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 335 "pascal.y"
                                                             { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "destructor"); }
#line 2547 "pascal.tab.c"
    break;

  case 89: /* class_member: CLASSSY PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON  */
#line 336 "pascal.y"
                                                                    { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2553 "pascal.tab.c"
    break;

  case 90: /* class_member: CLASSSY FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON  */
#line 337 "pascal.y"
                                                                                     { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[-1].node)), "class"); }
#line 2559 "pascal.tab.c"
    break;

  case 91: /* class_member: PROPERTYSY IDENT prop_index_opt COLON IDENT prop_specs SEMICOLON  */
#line 338 "pascal.y"
                                                                      { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_sval(pt_new(TT_DECL), "property"), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-4].node)), (yyvsp[-1].node)); }
#line 2565 "pascal.tab.c"
    break;

  case 92: /* prop_index_opt: %empty  */
#line 341 "pascal.y"
    { (yyval.node) = NULL; }
#line 2571 "pascal.tab.c"
    break;

  case 93: /* prop_index_opt: LBRACK parameter_decl_list RBRACK  */
#line 342 "pascal.y"
                                       { (yyval.node) = (yyvsp[-1].node); }
#line 2577 "pascal.tab.c"
    break;

  case 94: /* prop_specs: %empty  */
#line 345 "pascal.y"
    { (yyval.node) = pt_new(TT_VLIST); }
#line 2583 "pascal.tab.c"
    break;

  case 95: /* prop_specs: prop_specs IDENT IDENT  */
#line 346 "pascal.y"
                            { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2589 "pascal.tab.c"
    break;

  case 96: /* prop_specs: prop_specs IDENT INTCONST  */
#line 347 "pascal.y"
                               { (yyval.node) = pt_add(pt_add((yyvsp[-2].node), (yyvsp[-1].node)), (yyvsp[0].node)); }
#line 2595 "pascal.tab.c"
    break;

  case 97: /* packed_opt: PACKEDSY  */
#line 350 "pascal.y"
             { (yyval.node) = pt_kw("packed"); }
#line 2601 "pascal.tab.c"
    break;

  case 98: /* packed_opt: %empty  */
#line 351 "pascal.y"
     { (yyval.node) = NULL; }
#line 2607 "pascal.tab.c"
    break;

  case 99: /* simple_type: LPARENT id_list RPARENT  */
#line 354 "pascal.y"
                            { (yyval.node) = pt_retag((yyvsp[-1].node), TT_ENUM_TYPE); for (int _i = 0; (yyval.node) && _i < (yyval.node)->n; _i++) cenv_int((yyval.node)->c[_i]->v.sval, _i); }
#line 2613 "pascal.tab.c"
    break;

  case 100: /* simple_type: IDENT  */
#line 355 "pascal.y"
           { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 2619 "pascal.tab.c"
    break;

  case 101: /* simple_type: IDENT LBRACK b_expr RBRACK  */
#line 356 "pascal.y"
                                { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2625 "pascal.tab.c"
    break;

  case 102: /* simple_type: IDENT PERIOD IDENT  */
#line 357 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2631 "pascal.tab.c"
    break;

  case 103: /* simple_type: lo_bound DOTDOT b_expr  */
#line 358 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2637 "pascal.tab.c"
    break;

  case 104: /* lo_bound: lo_term  */
#line 361 "pascal.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 2643 "pascal.tab.c"
    break;

  case 105: /* lo_bound: PLUS lo_term  */
#line 362 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2649 "pascal.tab.c"
    break;

  case 106: /* lo_bound: MINUS lo_term  */
#line 363 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2655 "pascal.tab.c"
    break;

  case 107: /* lo_bound: lo_bound PLUS b_term  */
#line 364 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2661 "pascal.tab.c"
    break;

  case 108: /* lo_bound: lo_bound MINUS b_term  */
#line 365 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2667 "pascal.tab.c"
    break;

  case 109: /* lo_term: scalar_constant  */
#line 368 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2673 "pascal.tab.c"
    break;

  case 110: /* lo_term: IDENT LPARENT b_expr RPARENT  */
#line 369 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2679 "pascal.tab.c"
    break;

  case 111: /* lo_term: lo_term MUL b_factor  */
#line 370 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2685 "pascal.tab.c"
    break;

  case 112: /* lo_term: lo_term IDIV b_factor  */
#line 371 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2691 "pascal.tab.c"
    break;

  case 113: /* lo_term: lo_term IMOD b_factor  */
#line 372 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2697 "pascal.tab.c"
    break;

  case 114: /* lo_term: lo_term SHIFTOP b_factor  */
#line 373 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2703 "pascal.tab.c"
    break;

  case 115: /* b_expr: b_term  */
#line 376 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2709 "pascal.tab.c"
    break;

  case 116: /* b_expr: PLUS b_term  */
#line 377 "pascal.y"
                 { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 2715 "pascal.tab.c"
    break;

  case 117: /* b_expr: MINUS b_term  */
#line 378 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 2721 "pascal.tab.c"
    break;

  case 118: /* b_expr: b_expr PLUS b_term  */
#line 379 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2727 "pascal.tab.c"
    break;

  case 119: /* b_expr: b_expr MINUS b_term  */
#line 380 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2733 "pascal.tab.c"
    break;

  case 120: /* b_term: b_factor  */
#line 383 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2739 "pascal.tab.c"
    break;

  case 121: /* b_term: b_term MUL b_factor  */
#line 384 "pascal.y"
                         { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2745 "pascal.tab.c"
    break;

  case 122: /* b_term: b_term IDIV b_factor  */
#line 385 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2751 "pascal.tab.c"
    break;

  case 123: /* b_term: b_term IMOD b_factor  */
#line 386 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2757 "pascal.tab.c"
    break;

  case 124: /* b_term: b_term SHIFTOP b_factor  */
#line 387 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 2763 "pascal.tab.c"
    break;

  case 125: /* b_factor: scalar_constant  */
#line 390 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 2769 "pascal.tab.c"
    break;

  case 126: /* b_factor: IDENT LPARENT b_expr RPARENT  */
#line 391 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2775 "pascal.tab.c"
    break;

  case 127: /* b_factor: LPARENT b_expr RPARENT  */
#line 392 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 2781 "pascal.tab.c"
    break;

  case 128: /* record_body: record_field_list record_case_opt  */
#line 395 "pascal.y"
                                      { (yyval.node) = pt_add(pt_retag((yyvsp[-1].node), TT_FIELDS), (yyvsp[0].node)); }
#line 2787 "pascal.tab.c"
    break;

  case 129: /* record_field_list: record_field_list SEMICOLON record_field  */
#line 398 "pascal.y"
                                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2793 "pascal.tab.c"
    break;

  case 130: /* record_field_list: record_field  */
#line 399 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2799 "pascal.tab.c"
    break;

  case 131: /* $@4: %empty  */
#line 402 "pascal.y"
                  { }
#line 2805 "pascal.tab.c"
    break;

  case 132: /* record_field: id_list COLON $@4 type  */
#line 402 "pascal.y"
                           { (yyval.node) = pt_decl("field", (yyvsp[-3].node), (yyvsp[0].node)); }
#line 2811 "pascal.tab.c"
    break;

  case 133: /* record_field: %empty  */
#line 403 "pascal.y"
     { (yyval.node) = NULL; }
#line 2817 "pascal.tab.c"
    break;

  case 134: /* case_arm_mark: %empty  */
#line 406 "pascal.y"
    { }
#line 2823 "pascal.tab.c"
    break;

  case 135: /* case_open: %empty  */
#line 409 "pascal.y"
    { }
#line 2829 "pascal.tab.c"
    break;

  case 136: /* record_case_opt: CASESY IDENT COLON IDENT OFSY case_open record_case_list  */
#line 412 "pascal.y"
                                                             { (yyval.node) = pt_cat(pt_add(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2835 "pascal.tab.c"
    break;

  case 137: /* record_case_opt: CASESY IDENT OFSY case_open record_case_list  */
#line 413 "pascal.y"
                                                  { (yyval.node) = pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-3].node)), (yyvsp[0].node)); }
#line 2841 "pascal.tab.c"
    break;

  case 138: /* record_case_opt: %empty  */
#line 414 "pascal.y"
     { (yyval.node) = NULL; }
#line 2847 "pascal.tab.c"
    break;

  case 139: /* record_case_list: record_case_list SEMICOLON record_case_arm  */
#line 417 "pascal.y"
                                               { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 2853 "pascal.tab.c"
    break;

  case 140: /* record_case_list: record_case_arm  */
#line 418 "pascal.y"
                     { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 2859 "pascal.tab.c"
    break;

  case 141: /* record_case_arm: constant_list COLON LPARENT case_arm_mark record_body RPARENT  */
#line 421 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2865 "pascal.tab.c"
    break;

  case 142: /* record_case_arm: %empty  */
#line 422 "pascal.y"
     { (yyval.node) = NULL; }
#line 2871 "pascal.tab.c"
    break;

  case 143: /* var_decl_list: var_decl_list var_decl  */
#line 425 "pascal.y"
                           { (yyval.node) = pt_add((yyvsp[-1].node), (yyvsp[0].node)); }
#line 2877 "pascal.tab.c"
    break;

  case 144: /* var_decl_list: var_decl  */
#line 426 "pascal.y"
              { (yyval.node) = pt_part("var", (yyvsp[0].node)); }
#line 2883 "pascal.tab.c"
    break;

  case 145: /* var_decl: id_list COLON type SEMICOLON  */
#line 429 "pascal.y"
                                 { (yyval.node) = pt_decl("var", (yyvsp[-3].node), (yyvsp[-1].node)); }
#line 2889 "pascal.tab.c"
    break;

  case 146: /* var_decl: id_list COLON type EQOP typed_value SEMICOLON  */
#line 430 "pascal.y"
                                                   { (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 2895 "pascal.tab.c"
    break;

  case 147: /* var_decl: id_list COLON type IDENT IDENT SEMICOLON  */
#line 431 "pascal.y"
                                              { if (strcasecmp((yyvsp[-2].node)->v.sval, "absolute")) { pascal_yyerror("syntax error"); YYERROR; } (yyval.node) = pt_add(pt_decl("var", (yyvsp[-5].node), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_DECL), "absolute"), (yyvsp[-1].node))); }
#line 2901 "pascal.tab.c"
    break;

  case 148: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON  */
#line 434 "pascal.y"
                                                                               { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2907 "pascal.tab.c"
    break;

  case 149: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON FORWARDSY SEMICOLON  */
#line 435 "pascal.y"
                                                                                                 { pascal_note_type_ident((yyvsp[-3].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_kw("forward")); }
#line 2913 "pascal.tab.c"
    break;

  case 150: /* $@5: %empty  */
#line 436 "pascal.y"
                                                                               { }
#line 2919 "pascal.tab.c"
    break;

  case 151: /* procedure_decl: PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@5 block SEMICOLON  */
#line 436 "pascal.y"
                                                                                                   { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2925 "pascal.tab.c"
    break;

  case 152: /* $@6: %empty  */
#line 437 "pascal.y"
                                                                                                { pascal_note_type_ident((yyvsp[-2].node)); }
#line 2931 "pascal.tab.c"
    break;

  case 153: /* procedure_decl: FUNCTIONSY IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@6 block SEMICOLON  */
#line 437 "pascal.y"
                                                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-9].node)), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2937 "pascal.tab.c"
    break;

  case 154: /* $@7: %empty  */
#line 438 "pascal.y"
                                        { }
#line 2943 "pascal.tab.c"
    break;

  case 155: /* procedure_decl: FUNCTIONSY IDENT pv_mark SEMICOLON $@7 block SEMICOLON  */
#line 438 "pascal.y"
                                                            { (yyval.node) = pt_add(pt_add(pt_new(TT_FUNCTION), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2949 "pascal.tab.c"
    break;

  case 156: /* $@8: %empty  */
#line 439 "pascal.y"
                                                                                            { }
#line 2955 "pascal.tab.c"
    break;

  case 157: /* procedure_decl: PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@8 block SEMICOLON  */
#line 439 "pascal.y"
                                                                                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)); }
#line 2961 "pascal.tab.c"
    break;

  case 158: /* $@9: %empty  */
#line 440 "pascal.y"
                                                                                                             { }
#line 2967 "pascal.tab.c"
    break;

  case 159: /* procedure_decl: FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@9 block SEMICOLON  */
#line 440 "pascal.y"
                                                                                                                                 {
        (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node));
    }
#line 2975 "pascal.tab.c"
    break;

  case 160: /* $@10: %empty  */
#line 443 "pascal.y"
                                                                                              { }
#line 2981 "pascal.tab.c"
    break;

  case 161: /* procedure_decl: CONSTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@10 block SEMICOLON  */
#line 443 "pascal.y"
                                                                                                                  {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "constructor");
    }
#line 2989 "pascal.tab.c"
    break;

  case 162: /* $@11: %empty  */
#line 446 "pascal.y"
                                                                                             { }
#line 2995 "pascal.tab.c"
    break;

  case 163: /* procedure_decl: DESTRUCTORSY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@11 block SEMICOLON  */
#line 446 "pascal.y"
                                                                                                                 {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "destructor");
    }
#line 3003 "pascal.tab.c"
    break;

  case 164: /* $@12: %empty  */
#line 449 "pascal.y"
                                                                                                    { }
#line 3009 "pascal.tab.c"
    break;

  case 165: /* procedure_decl: CLASSSY PROCEDURESY IDENT PERIOD IDENT pv_mark parameter_list_opt SEMICOLON routine_directives $@12 block SEMICOLON  */
#line 449 "pascal.y"
                                                                                                                        {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-9].node)), (yyvsp[-7].node))), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3017 "pascal.tab.c"
    break;

  case 166: /* $@13: %empty  */
#line 452 "pascal.y"
                                                                                                                     { }
#line 3023 "pascal.tab.c"
    break;

  case 167: /* procedure_decl: CLASSSY FUNCTIONSY IDENT PERIOD IDENT pv_mark parameter_list_opt COLON result_type SEMICOLON routine_directives $@13 block SEMICOLON  */
#line 452 "pascal.y"
                                                                                                                                         {
        (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-11].node)), (yyvsp[-9].node))), (yyvsp[-7].node)), (yyvsp[-5].node)), (yyvsp[-1].node)), "class");
    }
#line 3031 "pascal.tab.c"
    break;

  case 168: /* pv_mark: %empty  */
#line 457 "pascal.y"
    { }
#line 3037 "pascal.tab.c"
    break;

  case 169: /* parameter_list_opt: LPARENT parameter_decl_list RPARENT  */
#line 460 "pascal.y"
                                        { (yyval.node) = (yyvsp[-1].node); }
#line 3043 "pascal.tab.c"
    break;

  case 170: /* parameter_list_opt: LPARENT RPARENT  */
#line 461 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3049 "pascal.tab.c"
    break;

  case 171: /* parameter_list_opt: %empty  */
#line 462 "pascal.y"
     { (yyval.node) = NULL; }
#line 3055 "pascal.tab.c"
    break;

  case 172: /* parameter_decl_list: parameter_decl_list SEMICOLON parameter_decl  */
#line 465 "pascal.y"
                                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3061 "pascal.tab.c"
    break;

  case 173: /* parameter_decl_list: parameter_decl  */
#line 466 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3067 "pascal.tab.c"
    break;

  case 174: /* parameter_decl: PROCEDURESY IDENT pf_params  */
#line 469 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3073 "pascal.tab.c"
    break;

  case 175: /* parameter_decl: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 470 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3079 "pascal.tab.c"
    break;

  case 176: /* parameter_decl: VARSY id_list COLON param_type  */
#line 471 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3085 "pascal.tab.c"
    break;

  case 177: /* parameter_decl: id_list COLON param_type  */
#line 472 "pascal.y"
                              { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3091 "pascal.tab.c"
    break;

  case 178: /* parameter_decl: CONSTSY id_list COLON param_type  */
#line 473 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3097 "pascal.tab.c"
    break;

  case 179: /* parameter_decl: id_list COLON param_type EQOP expression  */
#line 474 "pascal.y"
                                              { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3103 "pascal.tab.c"
    break;

  case 180: /* parameter_decl: CONSTSY id_list COLON param_type EQOP expression  */
#line 475 "pascal.y"
                                                      { pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3109 "pascal.tab.c"
    break;

  case 181: /* parameter_decl: IDENT id_list COLON param_type  */
#line 476 "pascal.y"
                                    { if (!(yyvsp[-3].node)->v.sval || strcasecmp((yyvsp[-3].node)->v.sval, "out")) { pascal_yyerror("syntax error"); YYERROR; } pascal_flip_dialect(PAS_DIALECT_OBJFPC); (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3115 "pascal.tab.c"
    break;

  case 182: /* param_type: IDENT  */
#line 479 "pascal.y"
          { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = (yyvsp[0].node); }
#line 3121 "pascal.tab.c"
    break;

  case 183: /* param_type: IDENT PERIOD IDENT  */
#line 480 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3127 "pascal.tab.c"
    break;

  case 184: /* param_type: ARRAYSY OFSY IDENT  */
#line 481 "pascal.y"
                        { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_new(TT_ARRAY_TYPE), (yyvsp[0].node)); }
#line 3133 "pascal.tab.c"
    break;

  case 185: /* routine_directives: %empty  */
#line 484 "pascal.y"
    { (yyval.node) = NULL; }
#line 3139 "pascal.tab.c"
    break;

  case 186: /* routine_directives: routine_directives IDENT SEMICOLON  */
#line 485 "pascal.y"
                                        {
        static const char *const ok[] = { "inline", "cdecl", "register", "stdcall", "pascal", "overload", "safecall", "far", "near", "noinline", "nostackframe" };
        int hit = 0;
        for (size_t k = 0; k < sizeof ok / sizeof ok[0]; k++) if ((yyvsp[-1].node)->v.sval && !strcasecmp((yyvsp[-1].node)->v.sval, ok[k])) hit = 1;
        if (!hit) { pascal_yyerror("syntax error"); YYERROR; }
        pascal_flip_dialect(PAS_DIALECT_FPC);
        (yyval.node) = NULL;
    }
#line 3152 "pascal.tab.c"
    break;

  case 187: /* pf_params: LPARENT pf_sections RPARENT  */
#line 495 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 3158 "pascal.tab.c"
    break;

  case 188: /* pf_params: LPARENT RPARENT  */
#line 496 "pascal.y"
                     { (yyval.node) = NULL; }
#line 3164 "pascal.tab.c"
    break;

  case 189: /* pf_params: %empty  */
#line 497 "pascal.y"
     { (yyval.node) = NULL; }
#line 3170 "pascal.tab.c"
    break;

  case 190: /* pf_sections: pf_sections SEMICOLON pf_section  */
#line 500 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3176 "pascal.tab.c"
    break;

  case 191: /* pf_sections: pf_section  */
#line 501 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_PARAMS), (yyvsp[0].node)); }
#line 3182 "pascal.tab.c"
    break;

  case 192: /* pf_section: id_list COLON param_type  */
#line 504 "pascal.y"
                             { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "value"); }
#line 3188 "pascal.tab.c"
    break;

  case 193: /* pf_section: VARSY id_list COLON param_type  */
#line 505 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "var"); }
#line 3194 "pascal.tab.c"
    break;

  case 194: /* pf_section: CONSTSY id_list COLON param_type  */
#line 506 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "const"); }
#line 3200 "pascal.tab.c"
    break;

  case 195: /* pf_section: PROCEDURESY IDENT pf_params  */
#line 507 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-1].node)), (yyvsp[0].node)); pt_sval((yyval.node), "procedure"); }
#line 3206 "pascal.tab.c"
    break;

  case 196: /* pf_section: FUNCTIONSY IDENT pf_params COLON result_type  */
#line 508 "pascal.y"
                                                  { pascal_note_type_ident((yyvsp[0].node)); (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_DECL), (yyvsp[-3].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "function"); }
#line 3212 "pascal.tab.c"
    break;

  case 197: /* id_list: id_list COMMA IDENT  */
#line 511 "pascal.y"
                        { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3218 "pascal.tab.c"
    break;

  case 198: /* id_list: IDENT  */
#line 512 "pascal.y"
           { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3224 "pascal.tab.c"
    break;

  case 199: /* body: BEGINSY statement_list ENDSY  */
#line 515 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3230 "pascal.tab.c"
    break;

  case 200: /* $@14: %empty  */
#line 518 "pascal.y"
                             { pas_stmt_mark(); }
#line 3236 "pascal.tab.c"
    break;

  case 201: /* statement_list: statement_list SEMICOLON $@14 statement  */
#line 518 "pascal.y"
                                                            { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add((yyvsp[-3].node), (yyvsp[0].node)); }
#line 3242 "pascal.tab.c"
    break;

  case 202: /* $@15: %empty  */
#line 519 "pascal.y"
     { pas_stmt_mark(); }
#line 3248 "pascal.tab.c"
    break;

  case 203: /* statement_list: $@15 statement  */
#line 519 "pascal.y"
                                    { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = pt_add(pt_new(TT_SEQ_EXPR), (yyvsp[0].node)); }
#line 3254 "pascal.tab.c"
    break;

  case 204: /* $@16: %empty  */
#line 522 "pascal.y"
    { pas_stmt_mark(); }
#line 3260 "pascal.tab.c"
    break;

  case 205: /* body_stmt: $@16 statement  */
#line 522 "pascal.y"
                                   { int _ln = pas_stmt_line_pop(); if ((yyvsp[0].node) && !(yyvsp[0].node)->line) (yyvsp[0].node)->line = _ln; (yyval.node) = (yyvsp[0].node); }
#line 3266 "pascal.tab.c"
    break;

  case 206: /* statement: statement_no_label  */
#line 525 "pascal.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3272 "pascal.tab.c"
    break;

  case 207: /* statement: INTCONST COLON statement_no_label  */
#line 526 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3278 "pascal.tab.c"
    break;

  case 208: /* statement: IDENT COLON statement_no_label  */
#line 527 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_new(TT_LABEL_DEF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3284 "pascal.tab.c"
    break;

  case 209: /* statement_no_label: assignment  */
#line 530 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3290 "pascal.tab.c"
    break;

  case 210: /* statement_no_label: call  */
#line 531 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3296 "pascal.tab.c"
    break;

  case 211: /* statement_no_label: compound_statement  */
#line 532 "pascal.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 3302 "pascal.tab.c"
    break;

  case 212: /* statement_no_label: goto_statement  */
#line 533 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3308 "pascal.tab.c"
    break;

  case 213: /* statement_no_label: if_statement  */
#line 534 "pascal.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 3314 "pascal.tab.c"
    break;

  case 214: /* statement_no_label: case_statement  */
#line 535 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3320 "pascal.tab.c"
    break;

  case 215: /* statement_no_label: while_statement  */
#line 536 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3326 "pascal.tab.c"
    break;

  case 216: /* statement_no_label: repeat_statement  */
#line 537 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3332 "pascal.tab.c"
    break;

  case 217: /* statement_no_label: for_statement  */
#line 538 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3338 "pascal.tab.c"
    break;

  case 218: /* statement_no_label: with_statement  */
#line 539 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3344 "pascal.tab.c"
    break;

  case 219: /* statement_no_label: try_statement  */
#line 540 "pascal.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 3350 "pascal.tab.c"
    break;

  case 220: /* statement_no_label: raise_statement  */
#line 541 "pascal.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3356 "pascal.tab.c"
    break;

  case 221: /* statement_no_label: inherited_call  */
#line 542 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3362 "pascal.tab.c"
    break;

  case 222: /* statement_no_label: INHERITEDSY  */
#line 543 "pascal.y"
                 { (yyval.node) = pt_kw("inherited"); }
#line 3368 "pascal.tab.c"
    break;

  case 223: /* statement_no_label: %empty  */
#line 544 "pascal.y"
     { (yyval.node) = pt_new(TT_SUCCEED); }
#line 3374 "pascal.tab.c"
    break;

  case 224: /* try_statement: TRYSY statement_list EXCEPTSY statement_list ENDSY  */
#line 547 "pascal.y"
                                                       { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3380 "pascal.tab.c"
    break;

  case 225: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ENDSY  */
#line 548 "pascal.y"
                                                      { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "except"); }
#line 3386 "pascal.tab.c"
    break;

  case 226: /* try_statement: TRYSY statement_list EXCEPTSY handler_list ELSESY statement_list ENDSY  */
#line 549 "pascal.y"
                                                                            { (yyval.node) = pt_sval(pt_add(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-5].node)), (yyvsp[-3].node)), pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[-1].node))), "except"); }
#line 3392 "pascal.tab.c"
    break;

  case 227: /* try_statement: TRYSY statement_list FINALLYSY statement_list ENDSY  */
#line 550 "pascal.y"
                                                         { (yyval.node) = pt_sval(pt_add(pt_add(pt_new(TT_TRY), (yyvsp[-3].node)), (yyvsp[-1].node)), "finally"); }
#line 3398 "pascal.tab.c"
    break;

  case 228: /* handler_list: handler  */
#line 553 "pascal.y"
            { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3404 "pascal.tab.c"
    break;

  case 229: /* handler_list: handler_list SEMICOLON handler  */
#line 554 "pascal.y"
                                    { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3410 "pascal.tab.c"
    break;

  case 230: /* handler_list: handler_list SEMICOLON  */
#line 555 "pascal.y"
                            { (yyval.node) = (yyvsp[-1].node); }
#line 3416 "pascal.tab.c"
    break;

  case 231: /* handler: ONSY IDENT COLON IDENT DOSY body_stmt  */
#line 558 "pascal.y"
                                          { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3422 "pascal.tab.c"
    break;

  case 232: /* handler: ONSY IDENT DOSY body_stmt  */
#line 559 "pascal.y"
                               { (yyval.node) = pt_add(pt_add(pt_new(TT_CATCH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3428 "pascal.tab.c"
    break;

  case 233: /* raise_statement: RAISESY  */
#line 562 "pascal.y"
            { (yyval.node) = pt_new(TT_DIE); }
#line 3434 "pascal.tab.c"
    break;

  case 234: /* raise_statement: RAISESY expression  */
#line 563 "pascal.y"
                        { (yyval.node) = pt_add(pt_new(TT_DIE), (yyvsp[0].node)); }
#line 3440 "pascal.tab.c"
    break;

  case 235: /* inherited_call: INHERITEDSY IDENT  */
#line 566 "pascal.y"
                      { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[0].node))); }
#line 3446 "pascal.tab.c"
    break;

  case 236: /* inherited_call: INHERITEDSY IDENT LPARENT RPARENT  */
#line 567 "pascal.y"
                                       { (yyval.node) = pt_add(pt_kw("inherited"), pt_add(pt_new(TT_FNC), (yyvsp[-2].node))); }
#line 3452 "pascal.tab.c"
    break;

  case 237: /* inherited_call: INHERITEDSY IDENT LPARENT argument_list RPARENT  */
#line 568 "pascal.y"
                                                     { (yyval.node) = pt_add(pt_kw("inherited"), pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node))); }
#line 3458 "pascal.tab.c"
    break;

  case 238: /* call: IDENT  */
#line 571 "pascal.y"
          { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[0].node)); }
#line 3464 "pascal.tab.c"
    break;

  case 239: /* call: selector PERIOD IDENT  */
#line 572 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node))); }
#line 3470 "pascal.tab.c"
    break;

  case 240: /* call: call_with_args  */
#line 573 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3476 "pascal.tab.c"
    break;

  case 241: /* call_with_args: IDENT LPARENT argument_list RPARENT  */
#line 576 "pascal.y"
                                        { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3482 "pascal.tab.c"
    break;

  case 242: /* call_with_args: IDENT LPARENT RPARENT  */
#line 577 "pascal.y"
                           { (yyval.node) = pt_add(pt_new(TT_FNC), (yyvsp[-2].node)); }
#line 3488 "pascal.tab.c"
    break;

  case 243: /* call_with_args: selector PERIOD IDENT LPARENT argument_list RPARENT  */
#line 578 "pascal.y"
                                                         { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3494 "pascal.tab.c"
    break;

  case 244: /* call_with_args: selector PERIOD IDENT LPARENT RPARENT  */
#line 579 "pascal.y"
                                           { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3500 "pascal.tab.c"
    break;

  case 245: /* call_with_args: call_with_args PERIOD IDENT LPARENT argument_list RPARENT  */
#line 580 "pascal.y"
                                                               { (yyval.node) = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-5].node)), (yyvsp[-3].node))), (yyvsp[-1].node)); }
#line 3506 "pascal.tab.c"
    break;

  case 246: /* call_with_args: call_with_args PERIOD IDENT LPARENT RPARENT  */
#line 581 "pascal.y"
                                                 { (yyval.node) = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-4].node)), (yyvsp[-2].node))); }
#line 3512 "pascal.tab.c"
    break;

  case 247: /* argument_list: argument_list COMMA argument  */
#line 584 "pascal.y"
                                 { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3518 "pascal.tab.c"
    break;

  case 248: /* argument_list: argument  */
#line 585 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3524 "pascal.tab.c"
    break;

  case 249: /* argument: expression  */
#line 588 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3530 "pascal.tab.c"
    break;

  case 250: /* argument: expression COLON expression  */
#line 589 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3536 "pascal.tab.c"
    break;

  case 251: /* argument: expression COLON expression COLON expression  */
#line 590 "pascal.y"
                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FMT), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3542 "pascal.tab.c"
    break;

  case 252: /* assignment: selector BECOMES expression  */
#line 593 "pascal.y"
                                { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3548 "pascal.tab.c"
    break;

  case 253: /* assignment: selector ADDBECOMES expression  */
#line 594 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("+=")); }
#line 3554 "pascal.tab.c"
    break;

  case 254: /* assignment: selector SUBBECOMES expression  */
#line 595 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("-=")); }
#line 3560 "pascal.tab.c"
    break;

  case 255: /* assignment: selector MULBECOMES expression  */
#line 596 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("*=")); }
#line 3566 "pascal.tab.c"
    break;

  case 256: /* assignment: selector DIVBECOMES expression  */
#line 597 "pascal.y"
                                    { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)), pt_kw("/=")); }
#line 3572 "pascal.tab.c"
    break;

  case 257: /* assignment: call_with_args BECOMES expression  */
#line 598 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_ASSIGN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3578 "pascal.tab.c"
    break;

  case 258: /* selector: selector LBRACK expression_list RBRACK  */
#line 601 "pascal.y"
                                           { (yyval.node) = pt_cat(pt_add(pt_new(TT_IDX), (yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3584 "pascal.tab.c"
    break;

  case 259: /* selector: selector PERIOD IDENT  */
#line 602 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3590 "pascal.tab.c"
    break;

  case 260: /* selector: selector ARROW  */
#line 603 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3596 "pascal.tab.c"
    break;

  case 261: /* selector: call_with_args PERIOD IDENT  */
#line 604 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_FIELD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3602 "pascal.tab.c"
    break;

  case 262: /* selector: call_with_args ARROW  */
#line 605 "pascal.y"
                          { (yyval.node) = pt_add(pt_new(TT_DEREF), (yyvsp[-1].node)); }
#line 3608 "pascal.tab.c"
    break;

  case 263: /* selector: IDENT  */
#line 606 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3614 "pascal.tab.c"
    break;

  case 264: /* expression_list: expression_list COMMA expression  */
#line 609 "pascal.y"
                                     { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3620 "pascal.tab.c"
    break;

  case 265: /* expression_list: expression  */
#line 610 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3626 "pascal.tab.c"
    break;

  case 266: /* compound_statement: BEGINSY statement_list ENDSY  */
#line 613 "pascal.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 3632 "pascal.tab.c"
    break;

  case 267: /* goto_statement: GOTOSY INTCONST  */
#line 616 "pascal.y"
                    { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3638 "pascal.tab.c"
    break;

  case 268: /* goto_statement: GOTOSY IDENT  */
#line 617 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_GOTO_U), (yyvsp[0].node)); (yyval.node)->line = pascal_get_lineno(); }
#line 3644 "pascal.tab.c"
    break;

  case 269: /* if_statement: IFSY expression THENSY body_stmt  */
#line 620 "pascal.y"
                                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IF), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3650 "pascal.tab.c"
    break;

  case 270: /* if_statement: IFSY expression THENSY body_stmt ELSESY body_stmt  */
#line 621 "pascal.y"
                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_IF), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3656 "pascal.tab.c"
    break;

  case 271: /* $@17: %empty  */
#line 624 "pascal.y"
                           { }
#line 3662 "pascal.tab.c"
    break;

  case 272: /* case_statement: CASESY expression OFSY $@17 case_list case_tail ENDSY  */
#line 624 "pascal.y"
                                                         { (yyval.node) = pt_add(pt_cat(pt_add(pt_new(TT_CASE), (yyvsp[-5].node)), (yyvsp[-2].node)), (yyvsp[-1].node)); }
#line 3668 "pascal.tab.c"
    break;

  case 273: /* case_tail: %empty  */
#line 627 "pascal.y"
    { (yyval.node) = NULL; }
#line 3674 "pascal.tab.c"
    break;

  case 274: /* case_tail: ELSESY statement_list  */
#line 628 "pascal.y"
                           { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "else"), (yyvsp[0].node)); }
#line 3680 "pascal.tab.c"
    break;

  case 275: /* case_tail: OTHERWISESY statement_list  */
#line 629 "pascal.y"
                                { (yyval.node) = pt_add(pt_sval(pt_new(TT_PART), "otherwise"), (yyvsp[0].node)); }
#line 3686 "pascal.tab.c"
    break;

  case 276: /* case_list: case_list SEMICOLON case_elem  */
#line 632 "pascal.y"
                                  { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3692 "pascal.tab.c"
    break;

  case 277: /* case_list: case_elem  */
#line 633 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3698 "pascal.tab.c"
    break;

  case 278: /* case_elem: constant_list COLON body_stmt  */
#line 636 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_ARM), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3704 "pascal.tab.c"
    break;

  case 279: /* case_elem: %empty  */
#line 637 "pascal.y"
     { (yyval.node) = NULL; }
#line 3710 "pascal.tab.c"
    break;

  case 280: /* constant_list: constant_list COMMA case_label  */
#line 640 "pascal.y"
                                   { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3716 "pascal.tab.c"
    break;

  case 281: /* constant_list: case_label  */
#line 641 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3722 "pascal.tab.c"
    break;

  case 282: /* case_label: b_expr  */
#line 644 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3728 "pascal.tab.c"
    break;

  case 283: /* case_label: b_expr DOTDOT b_expr  */
#line 645 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3734 "pascal.tab.c"
    break;

  case 284: /* while_statement: WHILESY expression DOSY body_stmt  */
#line 648 "pascal.y"
                                      { (yyval.node) = pt_add(pt_add(pt_new(TT_WHILE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3740 "pascal.tab.c"
    break;

  case 285: /* repeat_statement: REPEATSY statement_list UNTILSY expression  */
#line 651 "pascal.y"
                                               { (yyval.node) = pt_add(pt_add(pt_new(TT_REPEAT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3746 "pascal.tab.c"
    break;

  case 286: /* for_statement: FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt  */
#line 654 "pascal.y"
                                                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "to"); }
#line 3752 "pascal.tab.c"
    break;

  case 287: /* for_statement: FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt  */
#line 655 "pascal.y"
                                                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-6].node)), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "downto"); }
#line 3758 "pascal.tab.c"
    break;

  case 288: /* for_statement: FORSY IDENT INOP expression DOSY body_stmt  */
#line 656 "pascal.y"
                                                { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FOR), (yyvsp[-4].node)), (yyvsp[-2].node)), (yyvsp[0].node)); pt_sval((yyval.node), "in"); }
#line 3764 "pascal.tab.c"
    break;

  case 289: /* with_statement: WITHSY with_open DOSY body_stmt  */
#line 659 "pascal.y"
                                    { (yyval.node) = pt_add(pt_cat(pt_new(TT_WITH), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3770 "pascal.tab.c"
    break;

  case 290: /* with_open: with_open COMMA selector  */
#line 662 "pascal.y"
                             { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 3776 "pascal.tab.c"
    break;

  case 291: /* with_open: selector  */
#line 663 "pascal.y"
              { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 3782 "pascal.tab.c"
    break;

  case 292: /* expression: simple_expression  */
#line 670 "pascal.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 3788 "pascal.tab.c"
    break;

  case 293: /* expression: expression INOP simple_expression  */
#line 671 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_IN), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3794 "pascal.tab.c"
    break;

  case 294: /* expression: expression LTOP simple_expression  */
#line 672 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3800 "pascal.tab.c"
    break;

  case 295: /* expression: expression LEOP simple_expression  */
#line 673 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_LE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3806 "pascal.tab.c"
    break;

  case 296: /* expression: expression GTOP simple_expression  */
#line 674 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3812 "pascal.tab.c"
    break;

  case 297: /* expression: expression GEOP simple_expression  */
#line 675 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_GE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3818 "pascal.tab.c"
    break;

  case 298: /* expression: expression NEOP simple_expression  */
#line 676 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_NE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3824 "pascal.tab.c"
    break;

  case 299: /* expression: expression EQOP simple_expression  */
#line 677 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_new(TT_EQ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3830 "pascal.tab.c"
    break;

  case 300: /* expression: expression ISOP simple_expression  */
#line 678 "pascal.y"
                                       { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3836 "pascal.tab.c"
    break;

  case 301: /* simple_expression: term  */
#line 681 "pascal.y"
         { (yyval.node) = (yyvsp[0].node); }
#line 3842 "pascal.tab.c"
    break;

  case 302: /* simple_expression: PLUS term  */
#line 682 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_PLS), (yyvsp[0].node)); }
#line 3848 "pascal.tab.c"
    break;

  case 303: /* simple_expression: MINUS term  */
#line 683 "pascal.y"
                { (yyval.node) = pt_add(pt_new(TT_MNS), (yyvsp[0].node)); }
#line 3854 "pascal.tab.c"
    break;

  case 304: /* simple_expression: simple_expression PLUS term  */
#line 684 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ADD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3860 "pascal.tab.c"
    break;

  case 305: /* simple_expression: simple_expression MINUS term  */
#line 685 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUB), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3866 "pascal.tab.c"
    break;

  case 306: /* simple_expression: simple_expression OROP term  */
#line 686 "pascal.y"
                                 { (yyval.node) = pt_add(pt_add(pt_new(TT_ALT), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3872 "pascal.tab.c"
    break;

  case 307: /* simple_expression: simple_expression XOROP term  */
#line 687 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3878 "pascal.tab.c"
    break;

  case 308: /* term: power  */
#line 690 "pascal.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 3884 "pascal.tab.c"
    break;

  case 309: /* term: term MUL power  */
#line 691 "pascal.y"
                    { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3890 "pascal.tab.c"
    break;

  case 310: /* term: term RDIV power  */
#line 692 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3896 "pascal.tab.c"
    break;

  case 311: /* term: term IDIV power  */
#line 693 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3902 "pascal.tab.c"
    break;

  case 312: /* term: term IMOD power  */
#line 694 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3908 "pascal.tab.c"
    break;

  case 313: /* term: term ANDOP power  */
#line 695 "pascal.y"
                      { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3914 "pascal.tab.c"
    break;

  case 314: /* term: term MUL MINUS power  */
#line 696 "pascal.y"
                          { (yyval.node) = pt_add(pt_add(pt_new(TT_MUL), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 3920 "pascal.tab.c"
    break;

  case 315: /* term: term RDIV MINUS power  */
#line 697 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_DIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 3926 "pascal.tab.c"
    break;

  case 316: /* term: term IDIV MINUS power  */
#line 698 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_IDIV), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 3932 "pascal.tab.c"
    break;

  case 317: /* term: term IMOD MINUS power  */
#line 699 "pascal.y"
                           { (yyval.node) = pt_add(pt_add(pt_new(TT_MOD), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 3938 "pascal.tab.c"
    break;

  case 318: /* term: term ANDOP MINUS power  */
#line 700 "pascal.y"
                            { (yyval.node) = pt_add(pt_add(pt_new(TT_CONJ), (yyvsp[-3].node)), pt_add(pt_new(TT_MNS), (yyvsp[0].node))); }
#line 3944 "pascal.tab.c"
    break;

  case 319: /* term: term SHIFTOP power  */
#line 701 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3950 "pascal.tab.c"
    break;

  case 320: /* term: term ASOP power  */
#line 702 "pascal.y"
                     { (yyval.node) = pt_add(pt_add(pt_add(pt_new(TT_FNC), (yyvsp[-1].node)), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3956 "pascal.tab.c"
    break;

  case 321: /* power: factor  */
#line 705 "pascal.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 3962 "pascal.tab.c"
    break;

  case 322: /* power: power POWOP factor  */
#line 706 "pascal.y"
                        { (yyval.node) = pt_add(pt_add(pt_new(TT_POW), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 3968 "pascal.tab.c"
    break;

  case 323: /* factor: selector  */
#line 709 "pascal.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 3974 "pascal.tab.c"
    break;

  case 324: /* factor: call_with_args  */
#line 710 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 3980 "pascal.tab.c"
    break;

  case 325: /* factor: INTCONST  */
#line 711 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 3986 "pascal.tab.c"
    break;

  case 326: /* factor: REALCONST  */
#line 712 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 3992 "pascal.tab.c"
    break;

  case 327: /* factor: STRINGCONST  */
#line 713 "pascal.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 3998 "pascal.tab.c"
    break;

  case 328: /* factor: CHARCODE  */
#line 714 "pascal.y"
              { (yyval.node) = (yyvsp[0].node); }
#line 4004 "pascal.tab.c"
    break;

  case 329: /* factor: LPARENT expression RPARENT  */
#line 715 "pascal.y"
                                { (yyval.node) = (yyvsp[-1].node); }
#line 4010 "pascal.tab.c"
    break;

  case 330: /* factor: inherited_call  */
#line 716 "pascal.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 4016 "pascal.tab.c"
    break;

  case 331: /* factor: NOTSY factor  */
#line 717 "pascal.y"
                  { (yyval.node) = pt_add(pt_new(TT_NOT), (yyvsp[0].node)); }
#line 4022 "pascal.tab.c"
    break;

  case 332: /* factor: ATSIGN factor  */
#line 718 "pascal.y"
                   { (yyval.node) = pt_add(pt_new(TT_ADDR), (yyvsp[0].node)); }
#line 4028 "pascal.tab.c"
    break;

  case 333: /* factor: LBRACK RBRACK  */
#line 719 "pascal.y"
                   { (yyval.node) = pt_new(TT_SET); }
#line 4034 "pascal.tab.c"
    break;

  case 334: /* factor: LBRACK set_member_list RBRACK  */
#line 720 "pascal.y"
                                   { (yyval.node) = pt_cat(pt_new(TT_SET), (yyvsp[-1].node)); }
#line 4040 "pascal.tab.c"
    break;

  case 335: /* set_member_list: set_member  */
#line 723 "pascal.y"
               { (yyval.node) = pt_add(pt_new(TT_VLIST), (yyvsp[0].node)); }
#line 4046 "pascal.tab.c"
    break;

  case 336: /* set_member_list: set_member_list COMMA set_member  */
#line 724 "pascal.y"
                                      { (yyval.node) = pt_add((yyvsp[-2].node), (yyvsp[0].node)); }
#line 4052 "pascal.tab.c"
    break;

  case 337: /* set_member: expression  */
#line 727 "pascal.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 4058 "pascal.tab.c"
    break;

  case 338: /* set_member: expression DOTDOT expression  */
#line 728 "pascal.y"
                                  { (yyval.node) = pt_add(pt_add(pt_new(TT_SUBRANGE), (yyvsp[-2].node)), (yyvsp[0].node)); }
#line 4064 "pascal.tab.c"
    break;


#line 4068 "pascal.tab.c"

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

#line 734 "pascal.y"

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
