%define api.prefix {pascal_yy}
%code requires {
#include "ast.h"
#include "../snobol4/scrip_cc.h"
enum { PAS_DIALECT_ISO_DEFAULT, PAS_DIALECT_ISO, PAS_DIALECT_FPC, PAS_DIALECT_OBJFPC, PAS_DIALECT_DELPHI };
int pascal_dialect(void);
void pascal_dialect_set(int d);
}
%{
#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
extern void pascal_pragma_flush(tree_t *root);
#define PAS_STMT_STACK 512
static int pas_stmt_line_stk[PAS_STMT_STACK];
static int pas_stmt_sp = 0;
static void pas_stmt_mark(void) { if (pas_stmt_sp < PAS_STMT_STACK) pas_stmt_line_stk[pas_stmt_sp] = -1; pas_stmt_sp++; }
static void pas_stmt_line_fill(void) { if (pas_stmt_sp > 0 && pas_stmt_sp <= PAS_STMT_STACK && pas_stmt_line_stk[pas_stmt_sp - 1] < 0) pas_stmt_line_stk[pas_stmt_sp - 1] = pascal_get_lineno(); }
static int  pas_stmt_line_pop(void) { int l = -1; if (pas_stmt_sp > 0) { pas_stmt_sp--; if (pas_stmt_sp < PAS_STMT_STACK) l = pas_stmt_line_stk[pas_stmt_sp]; } return l >= 0 ? l : pascal_get_lineno(); }
int pascal_lex_wrapped(void) { int t = pascal_yylex(); pas_stmt_line_fill(); return t; }
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
static struct { PasCEnvE *e; int n; int cap; int ni; int nr; int ns; int unit_mode; int dialect; } g_pascal_cenv;
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
void pascal_cenv_reset(void) { g_pascal_cenv.n = 0; g_pascal_cenv.ni = 0; g_pascal_cenv.nr = 0; g_pascal_cenv.ns = 0; g_pascal_cenv.unit_mode = 0; g_pascal_cenv.dialect = PAS_DIALECT_ISO_DEFAULT; }
int pascal_unit_mode(void) { return g_pascal_cenv.unit_mode; }
void pascal_unit_mode_set(int m) { g_pascal_cenv.unit_mode = m; }
int pascal_dialect(void) { return g_pascal_cenv.dialect; }
void pascal_dialect_set(int d) { g_pascal_cenv.dialect = d; }
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
%}
%union { tree_t *node; }
%token GOTOSY PROGRAMSY SEMICOLON ARRAYSY LABELSY CONSTSY FORWARDSY
%token USESSY UNITSY INTERFACESY IMPLEMENTATIONSY INITIALIZATIONSY FINALIZATIONSY
%token DOSY DOWNTOSY FORSY REPEATSY WHILESY TOSY UNTILSY WITHSY CASESY
%token PROCEDURESY PACKEDSY OFSY FILESY ENDSY SETSY VARSY THENSY RECORDSY
%token FUNCTIONSY BEGINSY BECOMES TYPESY IFSY ELSESY INOP NOTSY IDIV IMOD ANDOP OROP
%token LTOP LEOP GTOP GEOP NEOP EQOP PLUS MINUS MUL RDIV
%token COMMA PERIOD COLON ARROW LBRACK RBRACK LPARENT RPARENT DOTDOT ATSIGN
%token <node> INTCONST CHARCODE REALCONST STRINGCONST IDENT
%type <node> program file_id_list_opt block decl_part_list decl_part label_list const_decl_list const_decl constant scalar_constant type_decl_list
%type <node> type_decl type packed_opt simple_type record_body record_field_list record_field case_arm_mark case_open record_case_opt record_case_list
%type <node> record_case_arm var_decl_list var_decl procedure_decl pv_mark parameter_list_opt parameter_decl_list parameter_decl pf_params pf_sections
%type <node> pf_section id_list body statement_list body_stmt statement statement_no_label call call_with_args argument_list argument assignment
%type <node> selector expression_list compound_statement goto_statement if_statement case_statement case_list case_elem constant_list while_statement
%type <node> repeat_statement for_statement with_statement with_open selector_list expression simple_expression term factor set_member_list set_member
%type <node> expression_list_opt compilation_unit unit intf_part_list intf_part init_opt fini_opt
%start compilation_unit
%%
compilation_unit:
    program { $$ = $1; }
    |unit { $$ = $1; }
    ;
program:
    PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD { $$ = pt_add(pt_add(pt_add(pt_new(TT_PROGRAM), $2), $3), $5); pt_flush_pragmas($$); pascal_prog_result = $$; }
    ;
file_id_list_opt:
    LPARENT id_list RPARENT { $$ = $2; }
    |{ $$ = NULL; }
    ;
block:
    decl_part_list body { $$ = pt_add($1, $2); }
    ;
decl_part_list:
    decl_part_list decl_part { $$ = pt_add($1, $2); }
    |{ $$ = pt_new(TT_BLOCK); }
    ;
decl_part:
    LABELSY label_list SEMICOLON { $$ = $2; }
    |CONSTSY const_decl_list { $$ = $2; }
    |TYPESY type_decl_list { $$ = $2; }
    |VARSY var_decl_list { $$ = $2; }
    |procedure_decl { $$ = $1; }
    |USESSY id_list SEMICOLON { $$ = pt_add(pt_sval(pt_new(TT_PART), "uses"), $2); }
    ;
unit:
    UNITSY IDENT SEMICOLON INTERFACESY intf_part_list IMPLEMENTATIONSY decl_part_list init_opt fini_opt ENDSY PERIOD { $$ = pt_add(pt_add(pt_add(pt_add(pt_add(pt_new(TT_MODULE_DECL), $2), $5), pt_sval(pt_retag($7, TT_PART), "implementation")), $8), $9); pt_flush_pragmas($$); pascal_prog_result = $$; }
    ;
intf_part_list:
    intf_part_list intf_part { $$ = pt_add($1, $2); }
    |{ $$ = pt_sval(pt_new(TT_PART), "interface"); }
    ;
intf_part:
    USESSY id_list SEMICOLON { $$ = pt_add(pt_sval(pt_new(TT_PART), "uses"), $2); }
    |CONSTSY const_decl_list { $$ = $2; }
    |TYPESY type_decl_list { $$ = $2; }
    |VARSY var_decl_list { $$ = $2; }
    |PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON { $$ = pt_add(pt_add(pt_new(TT_PROCEDURE), $2), $4); }
    |FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON { $$ = pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), $2), $4), $6); }
    ;
init_opt:
    INITIALIZATIONSY statement_list { $$ = pt_add(pt_sval(pt_new(TT_PART), "initialization"), $2); }
    |BEGINSY statement_list { $$ = pt_add(pt_sval(pt_new(TT_PART), "initialization"), $2); }
    |{ $$ = NULL; }
    ;
fini_opt:
    FINALIZATIONSY statement_list { $$ = pt_add(pt_sval(pt_new(TT_PART), "finalization"), $2); }
    |{ $$ = NULL; }
    ;
label_list:
    label_list COMMA INTCONST { $$ = pt_add($1, $3); }
    |INTCONST { $$ = pt_part("label", $1); }
    ;
const_decl_list:
    const_decl_list const_decl { $$ = pt_add($1, $2); }
    |const_decl { $$ = pt_part("const", $1); }
    ;
const_decl:
    IDENT EQOP REALCONST SEMICOLON { $$ = pt_decl("const", $1, $3); cenv_real($1->v.sval, $3->v.dval); }
    |IDENT EQOP PLUS REALCONST SEMICOLON { $$ = pt_decl("const", $1, pt_add(pt_new(TT_PLS), $4)); cenv_real($1->v.sval, $4->v.dval); }
    |IDENT EQOP MINUS REALCONST SEMICOLON { $$ = pt_decl("const", $1, pt_add(pt_new(TT_MNS), $4)); cenv_real($1->v.sval, -$4->v.dval); }
    |IDENT EQOP STRINGCONST SEMICOLON { $$ = pt_decl("const", $1, $3); cenv_string($1->v.sval, $3->v.sval); }
    |IDENT EQOP CHARCODE SEMICOLON { $$ = pt_decl("const", $1, $3); cenv_int($1->v.sval, $3->v.ival); }
    |IDENT EQOP constant SEMICOLON { $$ = pt_decl("const", $1, $3); cenv_int($1->v.sval, cenv_value($3)); }
    ;
constant:
    scalar_constant { $$ = $1; }
    |PLUS scalar_constant { $$ = pt_add(pt_new(TT_PLS), $2); }
    |MINUS scalar_constant { $$ = pt_add(pt_new(TT_MNS), $2); }
    ;
scalar_constant:
    IDENT { $$ = $1; }
    |INTCONST { $$ = $1; }
    |REALCONST { $$ = $1; }
    |STRINGCONST { $$ = $1; }
    |CHARCODE { $$ = $1; }
    ;
type_decl_list:
    type_decl_list type_decl { $$ = pt_add($1, $2); }
    |type_decl { $$ = pt_part("type", $1); }
    ;
type_decl:
    IDENT EQOP type SEMICOLON { $$ = pt_decl("type", $1, $3); }
    ;
type:
    simple_type { $$ = $1; }
    |ARROW IDENT { $$ = pt_add(pt_new(TT_PTR_TYPE), $2); }
    |packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY { } { } type { $$ = pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), $1), $4), $9); }
    |packed_opt ARRAYSY LBRACK simple_type COMMA simple_type RBRACK OFSY type { $$ = pt_add(pt_add(pt_add(pt_add(pt_new(TT_ARRAY_TYPE), $1), $4), $6), $9); }
    |packed_opt RECORDSY { } record_body ENDSY { $$ = pt_add(pt_add(pt_new(TT_RECORD), $1), $4); }
    |packed_opt SETSY OFSY simple_type { $$ = pt_add(pt_add(pt_new(TT_SET_TYPE), $1), $4); }
    |packed_opt FILESY { $$ = pt_add(pt_new(TT_FILE_TYPE), $1); }
    |packed_opt FILESY OFSY type { $$ = pt_add(pt_add(pt_new(TT_FILE_TYPE), $1), $4); }
    ;
packed_opt:
    PACKEDSY { $$ = pt_kw("packed"); }
    |{ $$ = NULL; }
    ;
simple_type:
    LPARENT id_list RPARENT { $$ = pt_retag($2, TT_ENUM_TYPE); for (int _i = 0; $$ && _i < $$->n; _i++) cenv_int($$->c[_i]->v.sval, _i); }
    |IDENT { $$ = $1; }
    |constant DOTDOT constant { $$ = pt_add(pt_add(pt_new(TT_SUBRANGE), $1), $3); }
    |STRINGCONST DOTDOT constant { $$ = pt_add(pt_add(pt_new(TT_SUBRANGE), $1), $3); }
    |CHARCODE DOTDOT constant { $$ = pt_add(pt_add(pt_new(TT_SUBRANGE), $1), $3); }
    ;
record_body:
    record_field_list record_case_opt { $$ = pt_add(pt_retag($1, TT_FIELDS), $2); }
    ;
record_field_list:
    record_field_list SEMICOLON record_field { $$ = pt_add($1, $3); }
    |record_field { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
record_field:
    id_list COLON { } type { $$ = pt_decl("field", $1, $4); }
    |{ $$ = NULL; }
    ;
case_arm_mark:
    { }
    ;
case_open:
    { }
    ;
record_case_opt:
    CASESY IDENT COLON IDENT OFSY case_open record_case_list { $$ = pt_cat(pt_add(pt_add(pt_new(TT_CASE), $2), $4), $7); }
    |CASESY IDENT OFSY case_open record_case_list { $$ = pt_cat(pt_add(pt_new(TT_CASE), $2), $5); }
    |{ $$ = NULL; }
    ;
record_case_list:
    record_case_list SEMICOLON record_case_arm { $$ = pt_add($1, $3); }
    |record_case_arm { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
record_case_arm:
    constant_list COLON LPARENT case_arm_mark record_body RPARENT { $$ = pt_add(pt_add(pt_new(TT_ARM), $1), $5); }
    |{ $$ = NULL; }
    ;
var_decl_list:
    var_decl_list var_decl { $$ = pt_add($1, $2); }
    |var_decl { $$ = pt_part("var", $1); }
    ;
var_decl:
    id_list COLON type SEMICOLON { $$ = pt_decl("var", $1, $3); }
    ;
procedure_decl:
    PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON FORWARDSY SEMICOLON { $$ = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), $2), $4), pt_kw("forward")); }
    |FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON { $$ = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), $2), $4), $6), pt_kw("forward")); }
    |PROCEDURESY IDENT pv_mark parameter_list_opt SEMICOLON { } block SEMICOLON { $$ = pt_add(pt_add(pt_add(pt_new(TT_PROCEDURE), $2), $4), $7); }
    |FUNCTIONSY IDENT pv_mark parameter_list_opt COLON IDENT SEMICOLON { } block SEMICOLON { $$ = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FUNCTION), $2), $4), $6), $9); }
    |FUNCTIONSY IDENT pv_mark SEMICOLON { } block SEMICOLON { $$ = pt_add(pt_add(pt_new(TT_FUNCTION), $2), $6); }
    ;
pv_mark:
    { }
    ;
parameter_list_opt:
    LPARENT parameter_decl_list RPARENT { $$ = $2; }
    |{ $$ = NULL; }
    ;
parameter_decl_list:
    parameter_decl_list SEMICOLON parameter_decl { $$ = pt_add($1, $3); }
    |parameter_decl { $$ = pt_add(pt_new(TT_PARAMS), $1); }
    ;
parameter_decl:
    PROCEDURESY IDENT pf_params { $$ = pt_add(pt_add(pt_new(TT_DECL), $2), $3); pt_sval($$, "procedure"); }
    |FUNCTIONSY IDENT pf_params COLON IDENT { $$ = pt_add(pt_add(pt_add(pt_new(TT_DECL), $2), $3), $5); pt_sval($$, "function"); }
    |VARSY id_list COLON IDENT { $$ = pt_add(pt_add(pt_new(TT_DECL), $2), $4); pt_sval($$, "var"); }
    |id_list COLON IDENT { $$ = pt_add(pt_add(pt_new(TT_DECL), $1), $3); pt_sval($$, "value"); }
    ;
pf_params:
    LPARENT pf_sections RPARENT { $$ = $2; }
    |{ $$ = NULL; }
    ;
pf_sections:
    pf_sections SEMICOLON pf_section { $$ = pt_add($1, $3); }
    |pf_section { $$ = pt_add(pt_new(TT_PARAMS), $1); }
    ;
pf_section:
    id_list COLON IDENT { $$ = pt_add(pt_add(pt_new(TT_DECL), $1), $3); pt_sval($$, "value"); }
    |VARSY id_list COLON IDENT { $$ = pt_add(pt_add(pt_new(TT_DECL), $2), $4); pt_sval($$, "var"); }
    |PROCEDURESY IDENT pf_params { $$ = pt_add(pt_add(pt_new(TT_DECL), $2), $3); pt_sval($$, "procedure"); }
    |FUNCTIONSY IDENT pf_params COLON IDENT { $$ = pt_add(pt_add(pt_add(pt_new(TT_DECL), $2), $3), $5); pt_sval($$, "function"); }
    ;
id_list:
    id_list COMMA IDENT { $$ = pt_add($1, $3); }
    |IDENT { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
body:
    BEGINSY statement_list ENDSY { $$ = $2; }
    ;
statement_list:
    statement_list SEMICOLON { pas_stmt_mark(); } statement { int _ln = pas_stmt_line_pop(); if ($4 && !$4->line) $4->line = _ln; $$ = pt_add($1, $4); }
    |{ pas_stmt_mark(); } statement { int _ln = pas_stmt_line_pop(); if ($2 && !$2->line) $2->line = _ln; $$ = pt_add(pt_new(TT_SEQ_EXPR), $2); }
    ;
body_stmt:
    { pas_stmt_mark(); } statement { int _ln = pas_stmt_line_pop(); if ($2 && !$2->line) $2->line = _ln; $$ = $2; }
    ;
statement:
    statement_no_label { $$ = $1; }
    |INTCONST COLON statement_no_label { $$ = pt_add(pt_add(pt_new(TT_LABEL_DEF), $1), $3); }
    ;
statement_no_label:
    assignment { $$ = $1; }
    |call { $$ = $1; }
    |compound_statement { $$ = $1; }
    |goto_statement { $$ = $1; }
    |if_statement { $$ = $1; }
    |case_statement { $$ = $1; }
    |while_statement { $$ = $1; }
    |repeat_statement { $$ = $1; }
    |for_statement { $$ = $1; }
    |with_statement { $$ = $1; }
    |{ $$ = pt_new(TT_SUCCEED); }
    ;
call:
    IDENT { $$ = pt_add(pt_new(TT_FNC), $1); }
    |selector PERIOD IDENT { $$ = pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), $1), $3)); }
    |call_with_args { $$ = $1; }
    ;
call_with_args:
    IDENT LPARENT argument_list RPARENT { $$ = pt_cat(pt_add(pt_new(TT_FNC), $1), $3); }
    |selector PERIOD IDENT LPARENT argument_list RPARENT { $$ = pt_cat(pt_add(pt_new(TT_FNC), pt_add(pt_add(pt_new(TT_FIELD), $1), $3)), $5); }
    ;
argument_list:
    argument_list COMMA argument { $$ = pt_add($1, $3); }
    |argument { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
argument:
    expression { $$ = $1; }
    |expression COLON expression { $$ = pt_add(pt_add(pt_new(TT_FMT), $1), $3); }
    |expression COLON expression COLON expression { $$ = pt_add(pt_add(pt_add(pt_new(TT_FMT), $1), $3), $5); }
    ;
assignment:
    selector BECOMES expression { $$ = pt_add(pt_add(pt_new(TT_ASSIGN), $1), $3); }
    ;
selector:
    selector LBRACK expression_list RBRACK { $$ = pt_cat(pt_add(pt_new(TT_IDX), $1), $3); }
    |selector PERIOD IDENT { $$ = pt_add(pt_add(pt_new(TT_FIELD), $1), $3); }
    |selector ARROW { $$ = pt_add(pt_new(TT_DEREF), $1); }
    |IDENT { $$ = $1; }
    ;
expression_list:
    expression_list COMMA expression { $$ = pt_add($1, $3); }
    |expression { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
compound_statement:
    BEGINSY statement_list ENDSY { $$ = $2; }
    ;
goto_statement:
    GOTOSY INTCONST { $$ = pt_add(pt_new(TT_GOTO_U), $2); $$->line = pascal_get_lineno(); }
    ;
if_statement:
    IFSY expression THENSY body_stmt { $$ = pt_add(pt_add(pt_new(TT_IF), $2), $4); }
    |IFSY expression THENSY body_stmt ELSESY body_stmt { $$ = pt_add(pt_add(pt_add(pt_new(TT_IF), $2), $4), $6); }
    ;
case_statement:
    CASESY expression OFSY { } case_list ENDSY { $$ = pt_cat(pt_add(pt_new(TT_CASE), $2), $5); }
    ;
case_list:
    case_list SEMICOLON case_elem { $$ = pt_add($1, $3); }
    |case_elem { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
case_elem:
    constant_list COLON body_stmt { $$ = pt_add(pt_add(pt_new(TT_ARM), $1), $3); }
    |{ $$ = NULL; }
    ;
constant_list:
    constant_list COMMA constant { $$ = pt_add($1, $3); }
    |constant { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
while_statement:
    WHILESY expression DOSY body_stmt { $$ = pt_add(pt_add(pt_new(TT_WHILE), $2), $4); }
    ;
repeat_statement:
    REPEATSY statement_list UNTILSY expression { $$ = pt_add(pt_add(pt_new(TT_REPEAT), $2), $4); }
    ;
for_statement:
    FORSY IDENT BECOMES expression TOSY expression DOSY body_stmt { $$ = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), $2), $4), $6), $8); pt_sval($$, "to"); }
    |FORSY IDENT BECOMES expression DOWNTOSY expression DOSY body_stmt { $$ = pt_add(pt_add(pt_add(pt_add(pt_new(TT_FOR), $2), $4), $6), $8); pt_sval($$, "downto"); }
    ;
with_statement:
    WITHSY with_open DOSY body_stmt { $$ = pt_add(pt_cat(pt_new(TT_WITH), $2), $4); }
    ;
with_open:
    with_open COMMA selector { $$ = pt_add($1, $3); }
    |selector { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
selector_list:
    selector_list COMMA selector { $$ = pt_add($1, $3); }
    |selector { $$ = pt_add(pt_new(TT_VLIST), $1); }
    ;
expression:
    simple_expression { $$ = $1; }
    |expression INOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_IN), $1), $3); }
    |expression LTOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_LT), $1), $3); }
    |expression LEOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_LE), $1), $3); }
    |expression GTOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_GT), $1), $3); }
    |expression GEOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_GE), $1), $3); }
    |expression NEOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_NE), $1), $3); }
    |expression EQOP simple_expression { $$ = pt_add(pt_add(pt_new(TT_EQ), $1), $3); }
    ;
simple_expression:
    term { $$ = $1; }
    |PLUS term { $$ = pt_add(pt_new(TT_PLS), $2); }
    |MINUS term { $$ = pt_add(pt_new(TT_MNS), $2); }
    |simple_expression PLUS term { $$ = pt_add(pt_add(pt_new(TT_ADD), $1), $3); }
    |simple_expression MINUS term { $$ = pt_add(pt_add(pt_new(TT_SUB), $1), $3); }
    |simple_expression OROP term { $$ = pt_add(pt_add(pt_new(TT_ALT), $1), $3); }
    ;
term:
    factor { $$ = $1; }
    |term MUL factor { $$ = pt_add(pt_add(pt_new(TT_MUL), $1), $3); }
    |term RDIV factor { $$ = pt_add(pt_add(pt_new(TT_DIV), $1), $3); }
    |term IDIV factor { $$ = pt_add(pt_add(pt_new(TT_IDIV), $1), $3); }
    |term IMOD factor { $$ = pt_add(pt_add(pt_new(TT_MOD), $1), $3); }
    |term ANDOP factor { $$ = pt_add(pt_add(pt_new(TT_CONJ), $1), $3); }
    ;
factor:
    selector { $$ = $1; }
    |call_with_args { $$ = $1; }
    |INTCONST { $$ = $1; }
    |REALCONST { $$ = $1; }
    |STRINGCONST { $$ = $1; }
    |CHARCODE { $$ = $1; }
    |LPARENT expression RPARENT { $$ = $2; }
    |NOTSY factor { $$ = pt_add(pt_new(TT_NOT), $2); }
    |ATSIGN factor { $$ = pt_add(pt_new(TT_ADDR), $2); }
    |LBRACK RBRACK { $$ = pt_new(TT_SET); }
    |LBRACK set_member_list RBRACK { $$ = pt_cat(pt_new(TT_SET), $2); }
    ;
set_member_list:
    set_member { $$ = pt_add(pt_new(TT_VLIST), $1); }
    |set_member_list COMMA set_member { $$ = pt_add($1, $3); }
    ;
set_member:
    expression { $$ = $1; }
    |expression DOTDOT expression { $$ = pt_add(pt_add(pt_new(TT_SUBRANGE), $1), $3); }
    ;
expression_list_opt:
    expression_list { $$ = $1; }
    |{ $$ = NULL; }
    ;
%%
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
