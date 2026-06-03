%define api.prefix {pascal_yy}
%code requires {
#include "ast.h"
#include "../snobol4/scrip_cc.h"
typedef struct PNodeList { tree_t **items; int count; int cap; } PNodeList;
}
%{
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "pascal.tab.h"
#include "pascal_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int  pascal_yylex(void);
extern int  pascal_get_lineno(void);
void pascal_yyerror(const char *msg) { fprintf(stderr, "pascal parse error line %d: %s\n", pascal_get_lineno(), msg); }
tree_t   *pascal_prog_result = NULL;
static PNodeList g_pascal_procs;
static PNodeList *pnl_new(void) { PNodeList *l = (PNodeList *)calloc(1, sizeof *l); return l; }
static void pnl_push(PNodeList *l, tree_t *e) {
    if (!l) return;
    if (l->count >= l->cap) { l->cap = l->cap ? l->cap * 2 : 8; l->items = (tree_t **)realloc(l->items, (size_t)l->cap * sizeof(tree_t *)); }
    l->items[l->count++] = e;
}
static PNodeList *pnl_concat(PNodeList *a, PNodeList *b) {
    if (!b) return a; for (int i = 0; i < b->count; i++) pnl_push(a, b->items[i]); return a;
}
static tree_t *leaf_s(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = (char *)(s ? s : ""); return e; }
static tree_t *ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *flit(double v) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = v; return e; }
static tree_t *bin(tree_e k, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(k); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *un(tree_e k, tree_t *a) { tree_t *e = ast_node_new(k); ast_push(e, a); return e; }
static tree_t *prog_of(PNodeList *l) {
    tree_t *e = ast_node_new(TT_PROGRAM);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static tree_t *seq_of(PNodeList *l) {
    if (l && l->count == 1) return l->items[0];
    tree_t *e = ast_node_new(TT_SEQ_EXPR);
    if (l) for (int i = 0; i < l->count; i++) ast_push(e, l->items[i]);
    return e;
}
static const char *map_io(const char *fn) {
    if (fn && !strcmp(fn, "writeln")) return "__pas_writeln";
    if (fn && !strcmp(fn, "write"))   return "__pas_write";
    if (fn && !strcmp(fn, "sqr"))     return "__pas_sqr";
    return fn;
}
static int is_pas_io(const char *fn) {
    return fn && (!strcmp(fn, "__pas_writeln") || !strcmp(fn, "__pas_write"));
}
static tree_t *mk_call(const char *name, PNodeList *args) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, map_io(name)));
    if (args) {
        if (is_pas_io(map_io(name))) {
            for (int i = 0; i < args->count; i++) ast_push(e, args->items[i]);
        } else {
            for (int i = 0; i < args->count; i += 2) ast_push(e, args->items[i]);
        }
    }
    return e;
}
static void emit_proc(PNodeList *procs, tree_t *proc) {
    tree_t *st = ast_stmt_new(TT_STMT);
    ast_push(st, ast_attr_int(":lang", LANG_PASCAL));
    ast_push(st, ast_attr_int(":line", 0));
    ast_push(st, ast_attr_int(":stno", 0));
    ast_push(st, ast_attr_expr(":subj", proc));
    pnl_push(procs, st);
}
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function) {
    tree_t *body_prog = ast_node_new(TT_PROGRAM);
    if (body_stmt && body_stmt->t == TT_PROGRAM) { for (int i = 0; i < body_stmt->n; i++) ast_push(body_prog, body_stmt->c[i]); }
    else if (body_stmt) { ast_push(body_prog, body_stmt); }
    tree_t *proc = ast_node_new(TT_PROC_DECL);
    proc->v.sval = (char *)name;
    ast_push(proc, leaf_s(TT_VAR, name));
    tree_t *vlist = ast_node_new(TT_VLIST);
    long long byref = 0;
    if (params) for (int i = 0; i < params->count; i++) {
        tree_t *pv = params->items[i];
        if (pv && pv->n > 0) { if (i < 64) byref |= (1LL << i); pv->n = 0; }
        ast_push(vlist, pv);
    }
    vlist->v.ival = byref;
    ast_push(proc, vlist);
    ast_push(proc, body_prog);
    if (is_function) ast_push(proc, leaf_s(TT_VAR, name));
    return proc;
}
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
static int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; long long high; } g_pas_arrays[256]; static int g_pas_narray;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_narray++; } }
static tree_t *mk_ident(const char *name) {
    if (name && !strcmp(name, "true"))  return ilit(1);
    if (name && !strcmp(name, "false")) return ilit(0);
    long long cv; if (pas_const_get(name, &cv)) return ilit(cv);
    return leaf_s(TT_VAR, name);
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
static tree_t *mk_array_fill(long long high) {
    long long n = high + 1; if (n < 1) n = 1;
    size_t len = (size_t)(n * 2 - 1);
    char *buf = (char *)malloc(len + 1); size_t p = 0;
    for (long long k = 0; k < n; k++) { if (k) buf[p++] = '\001'; buf[p++] = '0'; }
    buf[p] = '\0';
    tree_t *q = ast_node_new(TT_QLIT); q->v.sval = buf; return q;
}
%}
%union {
    tree_t    *node;
    PNodeList *list;
    char      *str;
    long long  ival;
    double     dval;
}
%token GOTOSY PROGRAMSY SEMICOLON ARRAYSY LABELSY CONSTSY FORWARDSY
%token DOSY DOWNTOSY FORSY REPEATSY WHILESY TOSY UNTILSY WITHSY CASESY
%token PROCEDURESY PACKEDSY OFSY FILESY ENDSY SETSY VARSY THENSY RECORDSY
%token FUNCTIONSY BEGINSY BECOMES TYPESY IFSY ELSESY INOP NOTSY IDIV IMOD ANDOP OROP
%token LTOP LEOP GTOP GEOP NEOP EQOP PLUS MINUS MUL RDIV
%token COMMA PERIOD COLON ARROW LBRACK RBRACK LPARENT RPARENT DOTDOT
%token <ival> INTCONST
%token <dval> REALCONST
%token <str>  STRINGCONST IDENT
%type <node> block body statement statement_no_label compound_statement
%type <node> assignment call call_with_args if_statement while_statement
%type <node> repeat_statement for_statement with_statement case_statement goto_statement
%type <node> expression simple_expression term factor selector
%type <list> statement_list argument_list expression_list id_list argument
%type <list> parameter_list_opt parameter_decl_list parameter_decl
%type <ival> constant scalar_constant simple_type type
%start program
%%
program:
    PROGRAMSY IDENT file_id_list_opt SEMICOLON block PERIOD
        { tree_t *body = $5;
          if (g_pas_narray > 0) {
              tree_t *combined = ast_node_new(TT_PROGRAM);
              for (int i = 0; i < g_pas_narray; i++) ast_push(combined, bin(TT_ASSIGN, leaf_s(TT_VAR, g_pas_arrays[i].name), mk_array_fill(g_pas_arrays[i].high)));
              if (body && body->t == TT_PROGRAM) { for (int i = 0; i < body->n; i++) ast_push(combined, body->c[i]); }
              else if (body) ast_push(combined, body);
              body = combined;
          }
          tree_t *mainp = mk_proc("main", NULL, body, 0); emit_proc(&g_pascal_procs, mainp);
          tree_t *root = ast_stmt_new(TT_PROGRAM);
          for (int i = 0; i < g_pascal_procs.count; i++) ast_push(root, g_pascal_procs.items[i]);
          pascal_prog_result = root; }
    ;
file_id_list_opt:
    LPARENT id_list RPARENT
    |
    ;
block:
    decl_part_list body { $$ = $2; }
    ;
decl_part_list:
    decl_part_list decl_part
    |
    ;
decl_part:
    LABELSY label_list SEMICOLON
    | CONSTSY const_decl_list
    | TYPESY type_decl_list
    | VARSY var_decl_list
    | procedure_decl
    ;
label_list:
    label_list COMMA INTCONST
    | INTCONST
    ;
const_decl_list:
    const_decl_list const_decl
    | const_decl
    ;
const_decl: IDENT EQOP constant SEMICOLON { pas_const_add($1, $3); } ;
constant:
    STRINGCONST { $$ = 0; } | scalar_constant { $$ = $1; } | PLUS scalar_constant { $$ = $2; } | MINUS scalar_constant { $$ = -$2; } ;
scalar_constant: IDENT { long long cv = 0; pas_const_get($1, &cv); $$ = cv; } | INTCONST { $$ = $1; } | REALCONST { $$ = (long long)$1; } ;
type_decl_list:
    type_decl_list type_decl
    | type_decl
    ;
type_decl: IDENT EQOP type SEMICOLON ;
type:
    simple_type { $$ = -1; }
    | ARROW IDENT { $$ = -1; }
    | packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type { $$ = $4; }
    | packed_opt RECORDSY record_body ENDSY { $$ = -1; }
    | packed_opt SETSY OFSY simple_type { $$ = -1; }
    | packed_opt FILESY { $$ = -1; }
    ;
packed_opt: PACKEDSY | ;
simple_type:
    LPARENT id_list RPARENT { $$ = -1; }
    | IDENT { $$ = -1; }
    | constant DOTDOT constant { $$ = $3; }
    ;
record_body:
    record_field_list
    ;
record_field_list:
    record_field_list SEMICOLON record_field
    | record_field
    ;
record_field:
    id_list COLON type
    |
    ;
var_decl_list:
    var_decl_list var_decl
    | var_decl
    ;
var_decl: id_list COLON type SEMICOLON { if ($3 >= 0 && $1) for (int i = 0; i < $1->count; i++) { tree_t *id = $1->items[i]; if (id && id->v.sval) pas_array_add(id->v.sval, $3); } } ;
procedure_decl:
    PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON { }
    | FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON { }
    | PROCEDURESY IDENT parameter_list_opt SEMICOLON block SEMICOLON
        { emit_proc(&g_pascal_procs, mk_proc($2, $3, $5, 0)); }
    | FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON block SEMICOLON
        { emit_proc(&g_pascal_procs, mk_proc($2, $3, $7, 1)); }
    ;
parameter_list_opt:
    LPARENT parameter_decl_list RPARENT { $$ = $2; }
    | { $$ = pnl_new(); }
    ;
parameter_decl_list:
    parameter_decl_list SEMICOLON parameter_decl { $$ = pnl_concat($1, $3); }
    | parameter_decl { $$ = $1; }
    ;
parameter_decl:
    PROCEDURESY id_list { $$ = $2; }
    | FUNCTIONSY id_list COLON IDENT { $$ = $2; }
    | VARSY id_list COLON IDENT { for (int i = 0; i < $2->count; i++) if ($2->items[i]) ast_push($2->items[i], ast_node_new(TT_SUCCEED)); $$ = $2; }
    | id_list COLON IDENT { $$ = $1; }
    ;
id_list:
    id_list COMMA IDENT { pnl_push($1, leaf_s(TT_VAR, $3)); $$ = $1; }
    | IDENT { PNodeList *l = pnl_new(); pnl_push(l, leaf_s(TT_VAR, $1)); $$ = l; }
    ;
body:
    BEGINSY statement_list ENDSY { $$ = prog_of($2); }
    ;
statement_list:
    statement_list SEMICOLON statement { if ($3) pnl_push($1, $3); $$ = $1; }
    | statement { PNodeList *l = pnl_new(); if ($1) pnl_push(l, $1); $$ = l; }
    ;
statement:
    statement_no_label { $$ = $1; }
    | INTCONST COLON statement_no_label { $$ = $3; }
    ;
statement_no_label:
    assignment { $$ = $1; }
    | call { $$ = $1; }
    | compound_statement { $$ = $1; }
    | goto_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | case_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | repeat_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | with_statement { $$ = $1; }
    | { $$ = ast_node_new(TT_SUCCEED); }
    ;
call:
    IDENT { $$ = mk_call($1, NULL); }
    | call_with_args { $$ = $1; }
    ;
call_with_args:
    IDENT LPARENT argument_list RPARENT { $$ = mk_call($1, $3); }
    ;
argument_list:
    argument_list COMMA argument { $$ = pnl_concat($1, $3); }
    | argument { $$ = $1; }
    ;
argument:
    expression { PNodeList *_al = pnl_new(); pnl_push(_al, $1); pnl_push(_al, ilit(-1)); $$ = _al; }
    | expression COLON expression { PNodeList *_al = pnl_new(); pnl_push(_al, $1); pnl_push(_al, $3); $$ = _al; }
    ;
assignment:
    selector BECOMES expression { $$ = bin(TT_ASSIGN, $1, $3); }
    ;
selector:
    selector LBRACK expression_list RBRACK { tree_t *e = ast_node_new(TT_IDX); ast_push(e, $1); if ($3) for (int i = 0; i < $3->count; i++) ast_push(e, $3->items[i]); $$ = e; }
    | selector PERIOD IDENT { $$ = bin(TT_FIELD, $1, leaf_s(TT_VAR, $3)); }
    | selector ARROW { $$ = $1; }
    | IDENT { $$ = mk_ident($1); }
    ;
expression_list:
    expression_list COMMA expression { pnl_push($1, $3); $$ = $1; }
    | expression { PNodeList *l = pnl_new(); pnl_push(l, $1); $$ = l; }
    ;
compound_statement:
    BEGINSY statement_list ENDSY { $$ = seq_of($2); }
    ;
goto_statement:
    GOTOSY INTCONST { $$ = ast_node_new(TT_SUCCEED); }
    ;
if_statement:
    IFSY expression THENSY statement { $$ = bin(TT_IF, pas_cond($2), $4); }
    | IFSY expression THENSY statement ELSESY statement { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond($2)); ast_push(e, $4); ast_push(e, $6); $$ = e; }
    ;
case_statement:
    CASESY expression OFSY case_list ENDSY { $$ = ast_node_new(TT_SUCCEED); }
    ;
case_list:
    case_list SEMICOLON case_elem
    | case_elem
    ;
case_elem:
    constant_list COLON statement
    |
    ;
constant_list:
    constant_list COMMA constant
    | constant
    ;
while_statement:
    WHILESY expression DOSY statement { $$ = bin(TT_WHILE, pas_cond($2), $4); }
    ;
repeat_statement:
    REPEATSY statement_list UNTILSY expression { $$ = bin(TT_REPEAT, seq_of($2), pas_cond($4)); }
    ;
for_statement:
    FORSY IDENT BECOMES expression TOSY expression DOSY statement
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, $2)); ast_push(e, $4); ast_push(e, $6); ast_push(e, $8); $$ = e; }
    | FORSY IDENT BECOMES expression DOWNTOSY expression DOSY statement
        { tree_t *e = ast_node_new(TT_FOR); ast_push(e, leaf_s(TT_VAR, $2)); ast_push(e, $4); ast_push(e, $6); ast_push(e, $8); e->v.ival = 1; $$ = e; }
    ;
with_statement:
    WITHSY selector_list DOSY statement { $$ = $4; }
    ;
selector_list:
    selector_list COMMA selector
    | selector
    ;
expression:
    simple_expression { $$ = $1; }
    | expression INOP simple_expression { $$ = bin(TT_FNC, $1, $3); }
    | expression LTOP simple_expression { $$ = bin(TT_LT, $1, $3); }
    | expression LEOP simple_expression { $$ = bin(TT_LE, $1, $3); }
    | expression GTOP simple_expression { $$ = bin(TT_GT, $1, $3); }
    | expression GEOP simple_expression { $$ = bin(TT_GE, $1, $3); }
    | expression NEOP simple_expression { $$ = bin(TT_NE, $1, $3); }
    | expression EQOP simple_expression { $$ = bin(TT_EQ, $1, $3); }
    ;
simple_expression:
    term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = un(TT_MNS, $2); }
    | simple_expression PLUS term { $$ = bin(TT_ADD, $1, $3); }
    | simple_expression MINUS term { $$ = bin(TT_SUB, $1, $3); }
    | simple_expression OROP term { $$ = bin(TT_ADD, $1, $3); }
    ;
term:
    factor { $$ = $1; }
    | term MUL factor { $$ = bin(TT_MUL, $1, $3); }
    | term RDIV factor { $$ = bin(TT_DIV, $1, $3); }
    | term IDIV factor { $$ = bin(TT_DIV, $1, $3); }
    | term IMOD factor { $$ = bin(TT_MOD, $1, $3); }
    | term ANDOP factor { $$ = bin(TT_MUL, $1, $3); }
    ;
factor:
    selector { $$ = $1; }
    | call_with_args { $$ = $1; }
    | INTCONST { $$ = ilit($1); }
    | REALCONST { $$ = flit($1); }
    | STRINGCONST { $$ = leaf_s(TT_QLIT, $1); }
    | LPARENT expression RPARENT { $$ = $2; }
    | NOTSY factor { $$ = un(TT_NOT, $2); }
    | LBRACK expression_list_opt RBRACK { $$ = ast_node_new(TT_SUCCEED); }
    ;
expression_list_opt:
    expression_list
    |
    ;
%%
extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
