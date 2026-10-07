%{
#include "ct_arena.h"
%}
%code top {
}
%code requires {
#include "scrip_cc.h"
#include "ct_vec.h"
struct LexCtx;
struct IfHead;
struct WhileHead;
struct DoHead;
struct FuncHead;
typedef struct LoopFrame {
    char    *cont_label;
    char    *end_label;
    int      is_loop;
    int      cont_used;
    struct LoopFrame *outer;
} LoopFrame;
typedef struct ScParseState {
    struct LexCtx *ctx;
    tree_t        *block;
    const char    *filename;
    int            nerrors;
    char          *cur_func_name;
    LoopFrame    *loop_top;
    struct SwitchHead *cur_switch;
    cv_t           labels;
} ScParseState;
}
%code {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "snocone_lex.h"
#include "../icon/icon_lex.h"
#define expr_new(k)            ast_node_new(k)
#define expr_add_child(p,c)    ast_push((p),(c))
#define TT_ASSIGN  TT_ASSIGN
#define TT_ADD     TT_ADD
#define TT_SUB     TT_SUB
#define TT_MUL     TT_MUL
#define TT_DIV     TT_DIV
#define TT_POW     TT_POW
#define TT_SEQ     TT_SEQ
#define TT_ALT     TT_ALT
#define TT_SCAN    TT_SCAN
#define TT_FNC     TT_FNC
#define TT_VAR     TT_VAR
#define TT_KEYWORD TT_KEYWORD
#define TT_QLIT    TT_QLIT
#define TT_ILIT    TT_ILIT
#define TT_FLIT    TT_FLIT
#define TT_NUL     TT_NUL
#define TT_VLIST   TT_VLIST
#define TT_IDX     TT_IDX
#define TT_INDIRECT   TT_INDIRECT
#define TT_DEFER      TT_DEFER
#define TT_NAME       TT_NAME
#define TT_CAPT_CURSOR      TT_CAPT_CURSOR
#define TT_CAPT_IMMED_ASGN  TT_CAPT_IMMED_ASGN
#define TT_CAPT_COND_ASGN   TT_CAPT_COND_ASGN
#define TT_PLS        TT_PLS
#define TT_MNS        TT_MNS
#define TT_NOT        TT_NOT
#define TT_INTERROGATE TT_INTERROGATE
#define TT_OPSYN      TT_OPSYN
#define kind       t
#define nchildren  n
#define children   c
#define sval       v.sval
#define ival       v.ival
#define dval       v.dval
int  sc_lex  (SC_STYPE *yylval, ScParseState *st);
void sc_error(ScParseState *st, const char *msg);
static void     sc_append_stmt        (ScParseState *st, tree_t *top);
static tree_t  *sc_collect_body       (ScParseState *st, int mark);
static void     sc_finalize_if_no_else_pst(ScParseState *st, struct IfHead *h);
static void     sc_finalize_if_else_pst(ScParseState *st, struct IfHead *h, int before_else);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t  *sc_int_literal        (const char *txt);
static tree_t  *sc_real_literal       (const char *txt);
static tree_t  *sc_str_literal        (const char *txt);
struct IfHead {
    tree_t *cond;
    int     mark;
    int     lineno;
};
struct WhileHead {
    tree_t *cond;
    int     mark;
};
struct DoHead {
    int     mark;
};
struct ForHead {
    tree_t *init;
    tree_t *cond;
    tree_t *step;
    int     mark;
};
struct FuncHead {
    char   *name;
    tree_t *params;
    tree_t *locals;
    char   *prev_func;
    int     mark;
};
struct CaseEntry {
    char   *case_label;
    tree_t *value;
    int     mark;
};
struct SwitchHead {
    tree_t *disc;
    char   *tmp_name;
    char   *end_label;
    char   *default_label;
    int     has_default;
    struct CaseEntry *cases;
    int     cases_count;
    int     cases_cap;
    struct SwitchHead *prev_switch;
    int     lineno;
};
static struct IfHead    *sc_if_head_new    (ScParseState *st, tree_t *cond);
static void     sc_finalize_while_pst  (ScParseState *st, struct WhileHead *h, tree_t *cond);
static void     sc_finalize_do_while_pst(ScParseState *st, struct DoHead *h, tree_t *cond);
static struct ForHead   *sc_for_head_new_pst(ScParseState *st, tree_t *init, tree_t *cond, tree_t *step, int mark);
static void     sc_finalize_for_pst   (ScParseState *st, struct ForHead *h);
static struct FuncHead *sc_func_head_new_pst(ScParseState *st, char *name, tree_t *params, tree_t *locals);
static void     sc_finalize_function_pst(ScParseState *st, struct FuncHead *h);
static void     sc_append_label_node  (ScParseState *st, const char *name);
static void     sc_loop_push           (ScParseState *st, char *cont_label, char *end_label, int is_loop);
static void     sc_loop_pop            (ScParseState *st);
static void     sc_append_break        (ScParseState *st, char *user_label);
static void     sc_append_continue     (ScParseState *st, char *user_label);
static struct SwitchHead *sc_switch_head_new(ScParseState *st, tree_t *disc);
static void     sc_switch_case_label   (ScParseState *st, tree_t *value);
static void     sc_switch_default_label(ScParseState *st);
static void     sc_finalize_switch_pst (ScParseState *st, struct SwitchHead *h);
static void     sc_emit_struct         (ScParseState *st, char *name, tree_t *fields);
static tree_t  *sc_qlit                (const char *txt);
static tree_t  *sc_list_one            (tree_e k, char *ident);
static tree_t  *sc_list_add            (tree_t *l, char *ident);
}
%define api.prefix {sc_}
%define api.pure full
%parse-param { ScParseState *st }
%lex-param   { ScParseState *st }
%union {
    tree_t *expr;
    char   *str;
    long    ival;
    int     markv;
    double  dval;
    struct IfHead    *ifhead;
    struct WhileHead *whilehead;
    struct DoHead    *dohead;
    struct ForHead   *forhead;
    struct FuncHead  *funchead;
    struct SwitchHead *switchhead;
}
%token <str> T_IDENT
%token <str> T_KEYWORD
%token <str> T_INT
%token <str> T_REAL
%token <str> T_STR
%token <str> T_CALL
%token T_2PLUS
%token T_2MINUS
%token T_2STAR
%token T_2SLASH
%token T_2CARET
%token T_1PLUS
%token T_1MINUS
%token T_2EQUAL
%token T_PLUS_ASSIGN T_MINUS_ASSIGN T_STAR_ASSIGN T_SLASH_ASSIGN T_CARET_ASSIGN
%token T_2QUEST
%token T_2PIPE
%token T_CONCAT
%token T_LPAREN
%token T_RPAREN
%token T_SEMICOLON
%token T_COMMA
%token T_LBRACK T_RBRACK
%token T_2DOLLAR T_2DOT
%token T_2AMP T_2AT T_2POUND T_2PERCENT T_2TILDE
%token T_1STAR T_1SLASH T_1PERCENT
%token T_1AT T_1TILDE T_1DOLLAR T_1DOT T_1POUND
%token T_1PIPE T_1EQUAL T_1QUEST T_1AMP T_1BANG
%token T_COLON
%token T_DO T_FOR
%token T_SWITCH T_CASE T_DEFAULT
%token T_BREAK T_CONTINUE T_GOTO
%token T_DEFINE T_RETURN T_FRETURN T_NRETURN T_STRUCT
%token T_UNKNOWN
%token T_LBRACE T_RBRACE
%token T_IF T_ELSE T_WHILE
%type <expr> expr0 expr1 expr3 expr4 expr5 expr6 expr9 expr11 expr12 expr14 expr15 expr17 exprlist exprlist_c optexpr
%type <whilehead> while_head
%type <dohead>    do_head
%type <ifhead>    if_head
%type <markv>     else_keyword
%type <forhead>   for_head
%type <funchead>  func_head
%type <expr>      func_arglist func_arglist_ne func_locals func_locals_ne
%type <switchhead> switch_head
%type <expr>      struct_field_list
%%
program     : stmt_list
            |
            ;
stmt_list   : stmt_list stmt
            | stmt
            ;
stmt        : matched_stmt
            | unmatched_stmt
            ;
matched_stmt
            : simple_stmt
            | block_stmt
            | if_head matched_stmt else_keyword matched_stmt
                                        { sc_finalize_if_else_pst(st, $1, $3); }
            | while_head matched_stmt
                                        { sc_finalize_while_pst(st, $1, $1->cond); }
            | do_head do_body T_WHILE T_LPAREN expr0 T_RPAREN T_SEMICOLON
                                        { sc_finalize_do_while_pst(st, $1, $5); }
            | for_head matched_stmt
                                        { sc_finalize_for_pst(st, $1); }
            | func_head T_LBRACE stmt_list T_RBRACE
                                        { sc_finalize_function_pst(st, $1); }
            | func_head T_LBRACE T_RBRACE
                                        { sc_finalize_function_pst(st, $1); }
            | switch_head T_LBRACE switch_body T_RBRACE
                                        { sc_finalize_switch_pst(st, $1); }
            | switch_head T_LBRACE T_RBRACE
                                        { sc_finalize_switch_pst(st, $1); }
            | T_STRUCT T_IDENT T_LBRACE struct_field_list T_RBRACE
                                        { sc_emit_struct(st, $2, $4); ct_drop($2); }
            | T_STRUCT T_IDENT T_LBRACE T_RBRACE
                                        { sc_emit_struct(st, $2, ast_node_new(TT_FIELDS)); ct_drop($2); }
            | label_decl
            ;
unmatched_stmt
            : if_head stmt
                                        { sc_finalize_if_no_else_pst(st, $1); }
            | if_head matched_stmt else_keyword unmatched_stmt
                                        { sc_finalize_if_else_pst(st, $1, $3); }
            | while_head unmatched_stmt
                                        { sc_finalize_while_pst(st, $1, $1->cond); }
            | for_head unmatched_stmt
                                        { sc_finalize_for_pst(st, $1); }
            ;
if_head     : T_IF T_LPAREN expr0 T_RPAREN opt_head_sep
                                        { $$ = sc_if_head_new(st, $3); }
            ;
while_head  : T_WHILE T_LPAREN expr0 T_RPAREN opt_head_sep
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          struct WhileHead *wh = ct_zalloc(1, sizeof *wh);
                                          wh->cond        = $3;
                                          wh->mark        = st->block->n;
                                          $$ = wh; }
            ;
do_head     : T_DO                  { sc_loop_push(st, NULL, NULL, 1);
                                      struct DoHead *dh = ct_zalloc(1, sizeof *dh);
                                      dh->mark = st->block->n;
                                      $$ = dh; }
            ;
do_body     : T_LBRACE stmt_list T_RBRACE
            | T_LBRACE T_RBRACE
            ;
for_lead    : T_FOR                  { }
            ;
for_head    : for_lead T_LPAREN expr0 T_SEMICOLON expr0 T_SEMICOLON expr0 T_RPAREN opt_head_sep
                                        { sc_loop_push(st, NULL, NULL, 1);
                                          $$ = sc_for_head_new_pst(st, $3, $5, $7, st->block->n); }
            ;
switch_head : T_SWITCH T_LPAREN expr0 T_RPAREN
                                        { $$ = sc_switch_head_new(st, $3); }
            ;
switch_body : case_clause
            | switch_body case_clause
            ;
case_clause : case_or_default_label
            | case_clause stmt
            ;
case_or_default_label
            : T_CASE expr0 T_COLON      { sc_switch_case_label(st, $2); }
            | T_DEFAULT T_COLON         { sc_switch_default_label(st); }
            ;
opt_head_sep
            :
            | T_CONCAT
            ;
func_head   : T_DEFINE T_IDENT T_LPAREN func_arglist func_locals
                                        { $$ = sc_func_head_new_pst(st, $2, $4, $5); ct_drop($2); }
            ;
func_locals
            : opt_head_sep                              { $$ = ast_node_new(TT_LOCALS); }
            | opt_head_sep func_locals_ne opt_head_sep  { $$ = $2; }
            ;
func_locals_ne
            : T_IDENT                  { $$ = sc_list_one(TT_LOCALS, $1); }
            | func_locals_ne T_COMMA T_IDENT
                { $$ = sc_list_add($1, $3); }
            ;
func_arglist
            : T_RPAREN                 { $$ = ast_node_new(TT_PARAMS); }
            | T_IDENT T_RPAREN         { $$ = sc_list_one(TT_PARAMS, $1); }
            | func_arglist_ne T_RPAREN { $$ = $1; }
            ;
func_arglist_ne
            : T_IDENT T_COMMA T_IDENT
                { $$ = sc_list_add(sc_list_one(TT_PARAMS, $1), $3); }
            | func_arglist_ne T_COMMA T_IDENT
                { $$ = sc_list_add($1, $3); }
            ;
struct_field_list
            : T_IDENT
                { $$ = sc_list_one(TT_FIELDS, $1); }
            | struct_field_list T_COMMA T_IDENT
                { $$ = sc_list_add($1, $3); }
            ;
else_keyword
            : T_ELSE                 { $$ = st->block->n; }
            ;
label_decl
            : T_IDENT T_COLON        { sc_append_label_node(st, $1); ct_drop($1); }
            ;
simple_stmt : expr0 T_SEMICOLON                { sc_append_stmt(st, $1); }
            | T_SEMICOLON                      {         }
            | T_RETURN expr0 T_SEMICOLON    { tree_t *r = ast_node_new(TT_RETURN); ast_push(r, $2);
                                             sc_append_stmt(st, r); }
            | T_RETURN T_SEMICOLON          { sc_append_stmt(st, ast_node_new(TT_RETURN)); }
            | T_FRETURN T_SEMICOLON         { sc_append_stmt(st, ast_node_new(TT_PROC_FAIL)); }
            | T_NRETURN T_SEMICOLON         { sc_append_stmt(st, ast_node_new(TT_NRETURN)); }
            | T_GOTO T_IDENT T_SEMICOLON    { tree_t *g = ast_node_new(TT_GOTO_U); ast_push(g, sc_qlit($2)); ct_drop($2); sc_append_stmt(st, g); }
            | T_BREAK T_SEMICOLON           { sc_append_break(st, NULL); }
            | T_BREAK T_IDENT T_SEMICOLON   { sc_append_break(st, $2); ct_drop($2); }
            | T_CONTINUE T_SEMICOLON        { sc_append_continue(st, NULL); }
            | T_CONTINUE T_IDENT T_SEMICOLON { sc_append_continue(st, $2); ct_drop($2); }
            ;
block_stmt  : T_LBRACE stmt_list T_RBRACE      { }
            | T_LBRACE T_RBRACE                {                  }
            ;
expr0       : expr1 T_2EQUAL    expr0
                                { $$ = expr_binary(TT_ASSIGN, $1, $3); }
            | expr1 T_2EQUAL
                                { tree_t *empty = expr_new(TT_QLIT);
                                  empty->sval = ct_strdup("");
                                  $$ = expr_binary(TT_ASSIGN, $1, empty); }
            | expr1 T_PLUS_ASSIGN   expr0
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPLUS;
                                  ast_push(a, $1); ast_push(a, $3); $$ = a; }
            | expr1 T_MINUS_ASSIGN  expr0
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGMINUS;
                                  ast_push(a, $1); ast_push(a, $3); $$ = a; }
            | expr1 T_STAR_ASSIGN   expr0
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSTAR;
                                  ast_push(a, $1); ast_push(a, $3); $$ = a; }
            | expr1 T_SLASH_ASSIGN  expr0
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGSLASH;
                                  ast_push(a, $1); ast_push(a, $3); $$ = a; }
            | expr1 T_CARET_ASSIGN  expr0
                                { tree_t *a = ast_node_new(TT_AUGOP); a->ival = TK_AUGPOW;
                                  ast_push(a, $1); ast_push(a, $3); $$ = a; }
            | expr1
                                { $$ = $1; }
            ;
expr1       : expr1 T_2QUEST expr3
                                { $$ = expr_binary(TT_SCAN, $1, $3); }
            | expr3
                                { $$ = $1; }
            ;
expr3       : expr3 T_2PIPE expr4
                                { $$ = expr_binary(TT_ALT, $1, $3); }
            | expr4
                                { $$ = $1; }
            ;
expr4       : expr4 T_CONCAT expr5
                                { $$ = expr_binary(TT_SEQ, $1, $3); }
            | expr5
                                { $$ = $1; }
            ;
expr5       : expr6
                                { $$ = $1; }
            ;
expr6       : expr6 T_2PLUS    expr9
                                { $$ = expr_binary(TT_ADD, $1, $3); }
            | expr6 T_2MINUS expr9
                                { $$ = expr_binary(TT_SUB, $1, $3); }
            | expr9
                                { $$ = $1; }
            ;
expr9       : expr9 T_2STAR expr11
                                { $$ = expr_binary(TT_MUL, $1, $3); }
            | expr9 T_2SLASH       expr11
                                { $$ = expr_binary(TT_DIV, $1, $3); }
            | expr11
                                { $$ = $1; }
            ;
expr11      : expr12 T_2CARET expr11
                                { $$ = expr_binary(TT_POW, $1, $3); }
            | expr12
                                { $$ = $1; }
            ;
expr12      : expr12 T_2DOLLAR expr14
                                { $$ = expr_binary(TT_CAPT_IMMED_ASGN, $1, $3); }
            | expr12 T_2DOT    expr14
                                { $$ = expr_binary(TT_CAPT_COND_ASGN,  $1, $3); }
            | expr14
                                { $$ = $1; }
            ;
expr14      : T_1PLUS  expr14
                                { $$ = expr_unary(TT_PLS, $2); }
            | T_1MINUS expr14
                                { $$ = expr_unary(TT_MNS, $2); }
            | T_1STAR   expr14  { $$ = expr_unary(TT_DEFER,       $2); }
            | T_1DOT    expr14  { if ($2 && ($2->t == TT_ILIT || $2->t == TT_FLIT || $2->t == TT_QLIT)) sc_error(st, "value used where name is required"); $$ = expr_unary(TT_NAME, $2); }
            | T_1DOLLAR expr14  { $$ = expr_unary(TT_INDIRECT,    $2); }
            | T_1AT     expr14  { $$ = expr_unary(TT_CAPT_CURSOR, $2); }
            | T_1TILDE  expr14  { $$ = expr_unary(TT_NOT,         $2); }
            | T_1QUEST  expr14  { $$ = expr_unary(TT_INTERROGATE, $2); }
            | T_1AMP    expr14  { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("&"); $$ = _e; }
            | T_1PERCENT expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("%"); $$ = _e; }
            | T_1SLASH   expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("/"); $$ = _e; }
            | T_1POUND   expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("#"); $$ = _e; }
            | T_1PIPE    expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("|"); $$ = _e; }
            | T_1EQUAL   expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("="); $$ = _e; }
            | T_1BANG    expr14 { tree_t *_e = expr_unary(TT_OPSYN, $2);
                                  _e->sval = ct_strdup("!"); $$ = _e; }
            | expr15
                                { $$ = $1; }
            ;
expr15      : expr15 T_LBRACK exprlist T_RBRACK
                                { tree_t *idx = expr_new(TT_IDX);
                                  tree_t *sub = expr_new(TT_VLIST);
                                  expr_add_child(idx, $1);
                                  for (int i = 0; i < $3->nchildren; i++)
                                      expr_add_child(sub, $3->children[i]);
                                  if ($3->c) ct_drop((char*)$3->c - sizeof(size_t)); ct_drop($3);
                                  expr_add_child(idx, sub);
                                  $$ = idx; }
            | expr17
                                { $$ = $1; }
            ;
exprlist    : exprlist_c
                                { $$ = $1; }
            | expr0
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, $1); $$ = l; }
            |
                                { $$ = expr_new(TT_NUL); }
            ;
exprlist_c  : exprlist_c T_COMMA optexpr
                                { expr_add_child($1, $3); $$ = $1; }
            | optexpr T_COMMA optexpr
                                { tree_t *l = expr_new(TT_NUL); expr_add_child(l, $1); expr_add_child(l, $3); $$ = l; }
            ;
optexpr     : expr0
                                { $$ = $1; }
            |
                                { $$ = expr_new(TT_NUL); }
            ;
expr17      : T_CALL exprlist T_RPAREN
                                { tree_t *e = expr_new(TT_FNC);
                                  tree_t *a = expr_new(TT_ARGS);
                                  expr_add_child(e, sc_qlit($1)); ct_drop($1);
                                  for (int i = 0; i < $2->nchildren; i++)
                                      expr_add_child(a, $2->children[i]);
                                  if ($2->c) ct_drop((char*)$2->c - sizeof(size_t)); ct_drop($2);
                                  expr_add_child(e, a);
                                  $$ = e; }
            | T_IDENT
                                { tree_t *e = expr_new(TT_VAR);
                                  e->sval = $1;
                                  $$ = e; }
            | T_KEYWORD
                                { tree_t *e = expr_new(TT_KEYWORD);
                                  e->sval = $1;
                                  $$ = e; }
            | T_INT
                                { $$ = sc_int_literal($1); ct_drop($1); }
            | T_REAL
                                { $$ = sc_real_literal($1); ct_drop($1); }
            | T_STR
                                { $$ = sc_str_literal($1); ct_drop($1); }
            | T_LPAREN expr0 T_RPAREN
                                { $$ = $2; }
            | T_LPAREN exprlist_c T_RPAREN
                                { tree_t *a = expr_new(TT_VLIST);
                                  for (int i = 0; i < $2->nchildren; i++)
                                      expr_add_child(a, $2->children[i]);
                                  if ($2->c) ct_drop((char*)$2->c - sizeof(size_t)); ct_drop($2);
                                  $$ = a; }
            | T_LPAREN T_RPAREN
                                { $$ = expr_new(TT_NUL); }
            ;
%%
void sc_error(ScParseState *st, const char *msg) {
    fprintf(stderr, "%s:%d: snocone parse error: %s\n",
            st->filename ? st->filename : "<stdin>",
            st->ctx ? st->ctx->line : 0,
            msg);
    st->nerrors++;
}
static void sc_append_stmt(ScParseState *st, tree_t *top) {
    if (!top) return;
    top->line = st->ctx ? st->ctx->line : 0;
    ast_push(st->block, top);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_qlit(const char *txt) {
    tree_t *q = ast_node_new(TT_QLIT);
    q->sval = ct_strdup(txt);
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_list_add(tree_t *l, char *ident) {
    tree_t *v = ast_node_new(TT_VAR);
    v->sval = ident;
    ast_push(l, v);
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_list_one(tree_e k, char *ident) {
    return sc_list_add(ast_node_new(k), ident);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_int_literal(const char *txt) {
    tree_t *e = expr_new(TT_ILIT);
    e->ival = strtol(txt, NULL, 10);
    return e;
}
static tree_t *sc_real_literal(const char *txt) {
    extern int rt_gtn_real(const char *, long, double *);
    tree_t *e = expr_new(TT_FLIT);
    if (!rt_gtn_real(txt, (long)strlen(txt), &e->dval)) e->dval = strtod(txt, NULL);
    return e;
}
static tree_t *sc_str_literal(const char *txt) {
    tree_t *e = expr_new(TT_QLIT);
    e->sval = ct_strdup(txt);
    return e;
}
static struct IfHead *sc_if_head_new(ScParseState *st, tree_t *cond) {
    struct IfHead *h = ct_zalloc(1, sizeof *h);
    h->cond        = cond;
    h->mark        = st->block->n;
    h->lineno      = st->ctx ? st->ctx->line : 0;
    return h;
}
static struct ForHead *sc_for_head_new_pst(ScParseState *st, tree_t *init, tree_t *cond, tree_t *step, int mark) {
    (void)st;
    struct ForHead *h = ct_zalloc(1, sizeof *h);
    h->init        = init;
    h->cond        = cond;
    h->step        = step;
    h->mark        = mark;
    return h;
}
static struct FuncHead *sc_func_head_new_pst(ScParseState *st, char *name, tree_t *params, tree_t *locals) {
    struct FuncHead *h  = ct_zalloc(1, sizeof *h);
    h->name             = ct_strdup(name);
    h->params           = params;
    h->locals           = locals;
    h->prev_func        = st->cur_func_name;
    h->mark             = st->block->n;
    st->cur_func_name   = h->name;
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_finalize_function_pst(ScParseState *st, struct FuncHead *h)
{
    tree_t *body  = sc_collect_body(st, h->mark);
    tree_t *def   = ast_node_new(TT_DEFINE);
    ast_push(def, sc_qlit(h->name));
    ast_push(def, h->params);
    ast_push(def, h->locals);
    ast_push(def, body);
    st->cur_func_name = h->prev_func;
    ct_drop(h->name); ct_drop(h);
    sc_append_stmt(st, def);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_append_label_node(ScParseState *st, const char *name) {
    for (uint32_t i = 0; i < st->labels.len; i++) if (!strcmp(CV_AT(st->labels, char *, i), name)) { sc_error(st, ct_fmt("duplicate label '%s'", name)); return; }
    tree_t *lab = ast_node_new(TT_LABEL);
    ast_push(lab, sc_qlit(name));
    CV_PUSH(st->labels, char *) = lab->c[0]->sval;
    sc_append_stmt(st, lab);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *sc_collect_body(ScParseState *st, int mark)
{
    tree_t *block = ast_node_new(TT_PROGRAM);
    for (int i = mark; i < st->block->n; i++) ast_push(block, st->block->c[i]);
    st->block->n = mark;
    return block;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sc_finalize_if_no_else_pst(ScParseState *st, struct IfHead *h)
{
    tree_t *then_block = sc_collect_body(st, h->mark);
    tree_t *if_node    = ast_node_new(TT_IF);
    ast_push(if_node, h->cond);
    ast_push(if_node, then_block);
    sc_append_stmt(st, if_node);
    ct_drop(h);
}
static void sc_finalize_if_else_pst(ScParseState *st, struct IfHead *h, int before_else)
{
    tree_t *else_block = sc_collect_body(st, before_else);
    tree_t *then_block = sc_collect_body(st, h->mark);
    tree_t *if_node    = ast_node_new(TT_IF);
    ast_push(if_node, h->cond);
    ast_push(if_node, then_block);
    ast_push(if_node, else_block);
    sc_append_stmt(st, if_node);
    ct_drop(h);
}
static void sc_finalize_while_pst(ScParseState *st, struct WhileHead *h, tree_t *cond)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *w      = ast_node_new(TT_WHILE);
    ast_push(w, cond);
    ast_push(w, body);
    sc_loop_pop(st);
    ct_drop(h);
    sc_append_stmt(st, w);
}
static void sc_finalize_do_while_pst(ScParseState *st, struct DoHead *h, tree_t *cond)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *dw     = ast_node_new(TT_DO_WHILE);
    ast_push(dw, body);
    ast_push(dw, cond);
    sc_loop_pop(st);
    ct_drop(h);
    sc_append_stmt(st, dw);
}
static void sc_finalize_for_pst(ScParseState *st, struct ForHead *h)
{
    tree_t    *body   = sc_collect_body(st, h->mark);
    tree_t    *f      = ast_node_new(TT_FOR);
    ast_push(f, h->init ? h->init : ast_node_new(TT_NUL));
    ast_push(f, h->cond);
    ast_push(f, h->step);
    ast_push(f, body);
    sc_loop_pop(st);
    sc_append_stmt(st, f);
    ct_drop(h);
}
static void sc_loop_push(ScParseState *st, char *cont_label, char *end_label, int is_loop) {
    LoopFrame *f = ct_zalloc(1, sizeof *f);
    f->cont_label = cont_label;
    f->end_label  = end_label;
    f->is_loop    = is_loop;
    f->outer      = st->loop_top;
    st->loop_top  = f;
}
static void sc_loop_pop(ScParseState *st) {
    LoopFrame *f = st->loop_top;
    if (!f) return;
    st->loop_top = f->outer;
    ct_drop(f->cont_label);
    ct_drop(f->end_label);
    ct_drop(f);
}
static void sc_append_break(ScParseState *st, char *user_label) {
    if (!st->loop_top) {
        sc_error(st, user_label ? "break: no enclosing loop or switch" : "break outside of loop or switch");
        return;
    }
    tree_t *brk = ast_node_new(TT_LOOP_BREAK);
    if (user_label) {
        tree_t *q = ast_node_new(TT_QLIT); q->sval = ct_strdup(user_label);
        ast_push(brk, q);
    }
    sc_append_stmt(st, brk);
}
static void sc_append_continue(ScParseState *st, char *user_label) {
    if (!st->loop_top) {
        sc_error(st, user_label ? "continue: no enclosing loop" : "continue outside of loop");
        return;
    }
    tree_t *nxt = ast_node_new(TT_LOOP_NEXT);
    if (user_label) {
        tree_t *q = ast_node_new(TT_QLIT); q->sval = ct_strdup(user_label);
        ast_push(nxt, q);
    }
    sc_append_stmt(st, nxt);
}
static void sc_switch_cases_grow(struct SwitchHead *h) {
    if (h->cases_count >= h->cases_cap) {
        int newcap = h->cases_cap ? h->cases_cap * 2 : 4;
        h->cases = ct_grow(h->cases, newcap * sizeof *h->cases);
        h->cases_cap = newcap;
    }
}
static struct SwitchHead *sc_switch_head_new(ScParseState *st, tree_t *disc) {
    struct SwitchHead *h = ct_zalloc(1, sizeof *h);
    h->disc          = disc;
    h->lineno        = st->ctx ? st->ctx->line : 0;
    h->prev_switch   = st->cur_switch;
    h->end_label     = NULL;
    h->default_label = NULL;
    h->has_default   = 0;
    h->tmp_name      = NULL;
    sc_loop_push(st, NULL, NULL, 0);
    st->cur_switch = h;
    return h;
}
static void sc_switch_case_label(ScParseState *st, tree_t *value) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) { sc_error(st, "case label outside of switch"); (void)value; return; }
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].value       = value;
    h->cases[h->cases_count].case_label  = NULL;
    h->cases[h->cases_count].mark        = st->block->n;
    h->cases_count++;
}
static void sc_switch_default_label(ScParseState *st) {
    struct SwitchHead *h = st->cur_switch;
    if (!h) { sc_error(st, "default label outside of switch"); return; }
    if (h->has_default) { sc_error(st, "duplicate default label in switch"); return; }
    h->has_default = 1;
    sc_switch_cases_grow(h);
    h->cases[h->cases_count].value       = NULL;
    h->cases[h->cases_count].case_label  = NULL;
    h->cases[h->cases_count].mark        = st->block->n;
    h->cases_count++;
}
static void sc_finalize_switch_pst(ScParseState *st, struct SwitchHead *h)
{
    int nc = h->cases_count;
    tree_t **bodies = ct_zalloc((size_t)(nc > 0 ? nc : 1), sizeof *bodies);
    for (int i = nc - 1; i >= 0; i--)
        bodies[i] = sc_collect_body(st, h->cases[i].mark);
    tree_t *node = ast_node_new(TT_CASE);
    ast_push(node, h->disc);
    for (int i = 0; i < nc; i++) {
        if (h->cases[i].value)
            ast_push(node, h->cases[i].value);
        else {
            tree_t *nul = ast_node_new(TT_NUL); ast_push(node, nul);
        }
        ast_push(node, bodies[i]);
    }
    ct_drop(bodies);
    sc_loop_pop(st);
    st->cur_switch = h->prev_switch;
    for (int i = 0; i < nc; i++) ct_drop(h->cases[i].case_label);
    ct_drop(h->cases);
    ct_drop(h->end_label);
    ct_drop(h->default_label);
    ct_drop(h->tmp_name);
    ct_drop(h);
    sc_append_stmt(st, node);
}
static void sc_emit_struct(ScParseState *st, char *name, tree_t *fields) {
    tree_t *node = ast_node_new(TT_STRUCT);
    ast_push(node, sc_qlit(name));
    ast_push(node, fields);
    sc_append_stmt(st, node);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *snocone_parse_tree(const char *src, const char *filename) {
    LexCtx          ctx = {0};
    ctx.p           = src ? src : "";
    ctx.line        = 1;
    ScParseState    state = {0};
    state.ctx       = (struct LexCtx *)&ctx;
    state.block     = ast_node_new(TT_PROGRAM);
    state.filename  = filename;
    state.nerrors   = 0;
    int rc = sc_parse(&state);
    while (state.loop_top) sc_loop_pop(&state);
    if (rc != 0 || state.nerrors > 0) return NULL;
    return state.block;
}
