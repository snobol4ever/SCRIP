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
static const char *pas_ptrvar_target(const char *v);
static const char *pas_ptrexpr_target(tree_t *e);
static int pas_rectype_nf(const char *rn);
static tree_t *mk_assign(tree_t *sel, tree_t *rhs);
static tree_t *mk_deref(tree_t *ptr) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_deref")); ast_push(e, ptr);
    return e;
}
static tree_t *mk_call(const char *name, PNodeList *args) {
    if (name && !strcmp(name, "ord") && args && args->count >= 1) return args->items[0];
    if (name && !strcmp(name, "new") && args && args->count >= 1) {
        tree_t *pv = args->items[0];
        const char *rt = pas_ptrexpr_target(pv);
        tree_t *alloc = ast_node_new(TT_FNC);
        if (rt) { ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc_rec")); ast_push(alloc, ilit(pas_rectype_nf(rt))); }
        else ast_push(alloc, leaf_s(TT_VAR, "__pas_alloc"));
        return mk_assign(pv, alloc);
    }
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
static tree_t *mk_in(tree_t *elem, tree_t *set) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_in")); ast_push(e, elem); ast_push(e, set);
    return e;
}
static tree_t *mk_set_ctor(PNodeList *elems) {
    tree_t *e = ast_node_new(TT_FNC);
    ast_push(e, leaf_s(TT_VAR, "__pas_set"));
    if (elems) for (int i = 0; i < elems->count; i++) ast_push(e, elems->items[i]);
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
static tree_t *mk_proc(const char *name, PNodeList *params, tree_t *body_stmt, int is_function, int decl_level, const char **lnames, int lcount) {
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
    tree_t *locals = ast_node_new(TT_VLIST);
    locals->v.ival = decl_level;
    for (int i = 0; i < lcount; i++) if (lnames[i]) ast_push(locals, leaf_s(TT_VAR, lnames[i]));
    ast_push(proc, locals);
    return proc;
}
static struct { const char *name; } g_pas_funcs[256]; static int g_pas_nfunc;
static void pas_func_add(const char *name) { if (g_pas_nfunc < 256 && name) { g_pas_funcs[g_pas_nfunc].name = strdup(name); g_pas_nfunc++; } }
static int pas_is_func(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nfunc; i++) if (g_pas_funcs[i].name && !strcmp(g_pas_funcs[i].name, name)) return 1; return 0; }
static struct { char *name; long long val; } g_pas_consts[256]; static int g_pas_nconst;
static void pas_const_add(const char *name, long long v) { if (g_pas_nconst < 256 && name) { g_pas_consts[g_pas_nconst].name = strdup(name); g_pas_consts[g_pas_nconst].val = v; g_pas_nconst++; } }
static int pas_const_get(const char *name, long long *out) { if (!name) return 0; for (int i = 0; i < g_pas_nconst; i++) if (g_pas_consts[i].name && !strcmp(g_pas_consts[i].name, name)) { *out = g_pas_consts[i].val; return 1; } return 0; }
static struct { char *name; long long high; } g_pas_arrays[256]; static int g_pas_narray;
static void pas_array_add(const char *name, long long high) { if (g_pas_narray < 256 && name) { g_pas_arrays[g_pas_narray].name = strdup(name); g_pas_arrays[g_pas_narray].high = high; g_pas_narray++; } }
#define PAS_REC_MAX 64
#define PAS_FIELD_MAX 32
static struct { char *tname; char *fields[PAS_FIELD_MAX]; char *fldptrto[PAS_FIELD_MAX]; int nf; } g_pas_rectypes[PAS_REC_MAX]; static int g_pas_nrectype;
static struct { char *vname; char *fields[PAS_FIELD_MAX]; int nf; } g_pas_recvars[PAS_REC_MAX]; static int g_pas_nrecvar;
static char *g_pas_pend_fields[PAS_FIELD_MAX]; static char *g_pas_pend_fldptrto[PAS_FIELD_MAX]; static int g_pas_pend_nf;
static char *g_pas_pend_ptrtarget; static char *g_pas_pend_typename;
static struct { char *pname; char *rname; } g_pas_ptrtypes[PAS_REC_MAX]; static int g_pas_nptrtype;
static void pas_ptrtype_add(const char *p, const char *r) { if (g_pas_nptrtype < PAS_REC_MAX && p && r) { int k = g_pas_nptrtype++; g_pas_ptrtypes[k].pname = strdup(p); g_pas_ptrtypes[k].rname = strdup(r); } }
static const char *pas_ptrtype_target(const char *p) { if (!p) return NULL; for (int i = 0; i < g_pas_nptrtype; i++) if (g_pas_ptrtypes[i].pname && !strcmp(g_pas_ptrtypes[i].pname, p)) return g_pas_ptrtypes[i].rname; return NULL; }
static struct { char *vname; char *rname; } g_pas_ptrvars[PAS_REC_MAX]; static int g_pas_nptrvar;
static void pas_ptrvar_add(const char *v, const char *r) { if (g_pas_nptrvar < PAS_REC_MAX && v && r) { int k = g_pas_nptrvar++; g_pas_ptrvars[k].vname = strdup(v); g_pas_ptrvars[k].rname = strdup(r); } }
static const char *pas_ptrvar_target(const char *v) { if (!v) return NULL; for (int i = 0; i < g_pas_nptrvar; i++) if (g_pas_ptrvars[i].vname && !strcmp(g_pas_ptrvars[i].vname, v)) return g_pas_ptrvars[i].rname; return NULL; }
static void pas_pend_reset(void) { g_pas_pend_nf = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL; }
static void pas_pend_add(const char *f) { if (g_pas_pend_nf < PAS_FIELD_MAX && f) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_pend_ptrtarget; g_pas_pend_fields[g_pas_pend_nf++] = strdup(f); } }
static void pas_rectype_add(const char *tn) { if (g_pas_nrectype >= PAS_REC_MAX || !tn) return; int k = g_pas_nrectype++; g_pas_rectypes[k].tname = strdup(tn); g_pas_rectypes[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) { g_pas_rectypes[k].fields[i] = g_pas_pend_fields[i]; g_pas_rectypes[k].fldptrto[i] = g_pas_pend_fldptrto[i]; } }
static int pas_rectype_to_pend(const char *tn) { if (!tn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, tn)) {
    pas_pend_reset(); for (int j = 0; j < g_pas_rectypes[i].nf; j++) { g_pas_pend_fldptrto[g_pas_pend_nf] = g_pas_rectypes[i].fldptrto[j]; g_pas_pend_fields[g_pas_pend_nf++] = g_pas_rectypes[i].fields[j]; } return 1; } return 0; }
static int pas_rectype_field_index(const char *rn, const char *fn) { if (!rn || !fn) return -1; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    for (int j = 0; j < g_pas_rectypes[i].nf; j++) if (g_pas_rectypes[i].fields[j] && !strcmp(g_pas_rectypes[i].fields[j], fn)) return j; return -1; } return -1; }
static int pas_rectype_nf(const char *rn) { if (!rn) return 0; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) return g_pas_rectypes[i].nf; return 0; }
static const char *pas_rectype_field_ptrto_by_index(const char *rn, long idx) { if (!rn || idx < 0) return NULL; for (int i = 0; i < g_pas_nrectype; i++) if (g_pas_rectypes[i].tname && !strcmp(g_pas_rectypes[i].tname, rn)) {
    if (idx < g_pas_rectypes[i].nf) return g_pas_rectypes[i].fldptrto[idx]; return NULL; } return NULL; }
static void pas_recvar_add(const char *vn) { if (g_pas_nrecvar >= PAS_REC_MAX || !vn || g_pas_pend_nf == 0) return; int k = g_pas_nrecvar++; g_pas_recvars[k].vname = strdup(vn); g_pas_recvars[k].nf = g_pas_pend_nf;
    for (int i = 0; i < g_pas_pend_nf; i++) g_pas_recvars[k].fields[i] = g_pas_pend_fields[i]; }
static int pas_recvar_field_index(const char *vn, const char *fn) { if (!vn || !fn) return -1; for (int i = 0; i < g_pas_nrecvar; i++) if (g_pas_recvars[i].vname && !strcmp(g_pas_recvars[i].vname, vn)) {
    for (int j = 0; j < g_pas_recvars[i].nf; j++) if (g_pas_recvars[i].fields[j] && !strcmp(g_pas_recvars[i].fields[j], fn)) return j; return -1; } return -1; }
static const char *pas_ptrexpr_target(tree_t *e);
static const char *pas_selector_rectype(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_FNC && e->n >= 2 && e->c[0] && e->c[0]->v.sval && !strcmp(e->c[0]->v.sval, "__pas_deref")) return pas_ptrexpr_target(e->c[1]);
    return NULL; }
static const char *pas_ptrexpr_target(tree_t *e) { if (!e) return NULL;
    if (e->t == TT_VAR && e->v.sval) return pas_ptrvar_target(e->v.sval);
    if (e->t == TT_IDX && e->n >= 2 && e->c[0] && e->c[1] && e->c[1]->t == TT_ILIT) { const char *rt = pas_selector_rectype(e->c[0]); if (rt) return pas_rectype_field_ptrto_by_index(rt, e->c[1]->v.ival); }
    return NULL; }
#define PAS_LOCAL_MAX 64
#define PAS_NEST_MAX  16
static int g_pas_level = 1;
static struct { const char *names[PAS_LOCAL_MAX]; int n; int decl_level; } g_pas_lstk[PAS_NEST_MAX]; static int g_pas_ldepth;
static void pas_proc_enter(void) { if (g_pas_ldepth < PAS_NEST_MAX) { g_pas_lstk[g_pas_ldepth].n = 0; g_pas_lstk[g_pas_ldepth].decl_level = g_pas_level; } g_pas_ldepth++; g_pas_level++; }
static void pas_proc_exit(void) { if (g_pas_ldepth > 0) g_pas_ldepth--; if (g_pas_level > 1) g_pas_level--; }
static void pas_local_add(const char *name) { if (g_pas_level < 2 || g_pas_ldepth == 0 || g_pas_ldepth > PAS_NEST_MAX || !name) return; int d = g_pas_ldepth - 1; if (g_pas_lstk[d].n < PAS_LOCAL_MAX) g_pas_lstk[d].names[g_pas_lstk[d].n++] = strdup(name); }
static struct { char *name; } g_pas_setvars[256]; static int g_pas_nsetvar;
static void pas_setvar_add(const char *name) { if (g_pas_nsetvar < 256 && name) { g_pas_setvars[g_pas_nsetvar++].name = strdup(name); } }
static int pas_is_setvar(const char *name) { if (!name) return 0; for (int i = 0; i < g_pas_nsetvar; i++) if (g_pas_setvars[i].name && !strcmp(g_pas_setvars[i].name, name)) return 1; return 0; }
static char g_pas_case_tmp[8][24]; static int g_pas_case_depth; static int g_pas_case_ctr;
static void pas_case_push(void) { if (g_pas_case_depth < 8) snprintf(g_pas_case_tmp[g_pas_case_depth], sizeof g_pas_case_tmp[0], "__pct%d", g_pas_case_ctr++); g_pas_case_depth++; }
static const char *pas_case_cur(void) { int d = g_pas_case_depth - 1; if (d < 0) d = 0; if (d > 7) d = 7; return strdup(g_pas_case_tmp[d]); }
static void pas_case_pop(void) { if (g_pas_case_depth > 0) g_pas_case_depth--; }
static int pas_is_setexpr(tree_t *e) { if (!e) return 0;
    if (e->t == TT_VAR && e->v.sval) return pas_is_setvar(e->v.sval);
    if (e->t == TT_FNC && e->n >= 1 && e->c[0] && e->c[0]->v.sval) { const char *f = e->c[0]->v.sval;
        return !strcmp(f, "__pas_set") || !strcmp(f, "__pas_setuni") || !strcmp(f, "__pas_setint") || !strcmp(f, "__pas_setdif"); }
    return 0; }
static tree_t *mk_set_bin(const char *name, tree_t *a, tree_t *b) { tree_t *e = ast_node_new(TT_FNC); ast_push(e, leaf_s(TT_VAR, name)); ast_push(e, a); ast_push(e, b); return e; }
static tree_t *pas_arith_or_set(tree_e ak, const char *setfn, tree_t *a, tree_t *b) { return (pas_is_setexpr(a) || pas_is_setexpr(b)) ? mk_set_bin(setfn, a, b) : bin(ak, a, b); }
static tree_t *mk_assign(tree_t *sel, tree_t *rhs) {
    if (sel && sel->t == TT_IDX && sel->n >= 2 && sel->c[0] && sel->c[0]->t == TT_FNC && sel->c[0]->n >= 2
        && sel->c[0]->c[0] && sel->c[0]->c[0]->v.sval && !strcmp(sel->c[0]->c[0]->v.sval, "__pas_deref")) {
        tree_t *e = ast_node_new(TT_FNC);
        ast_push(e, leaf_s(TT_VAR, "__pas_field_set"));
        ast_push(e, sel->c[0]->c[1]); ast_push(e, sel->c[1]); ast_push(e, rhs);
        return e;
    }
    return bin(TT_ASSIGN, sel, rhs);
}
static tree_t *mk_ident(const char *name) {
    if (name && !strcmp(name, "true"))  return ilit(1);
    if (name && !strcmp(name, "false")) return ilit(0);
    if (name && !strcmp(name, "nil"))   return ilit(0);
    long long cv; if (pas_const_get(name, &cv)) return ilit(cv);
    if (pas_is_func(name)) return mk_call(name, NULL);
    return leaf_s(TT_VAR, name);
}
static int pas_is_rel(tree_t *e) {
    if (!e) return 0;
    switch (e->t) { case TT_LT: case TT_LE: case TT_GT: case TT_GE: case TT_EQ: case TT_NE: return 1; default: return 0; }
}
static tree_t *pas_cond(tree_t *e) { return pas_is_rel(e) ? e : bin(TT_NE, e, ilit(0)); }
static tree_t *pas_bool(tree_t *e) { if (!pas_is_rel(e)) return e; tree_t *r = ast_node_new(TT_IF); ast_push(r, e); ast_push(r, ilit(1)); ast_push(r, ilit(0)); return r; }
static tree_t *pas_flip_rel(tree_t *e) {
    switch (e->t) { case TT_LT: e->t = TT_GE; break; case TT_GE: e->t = TT_LT; break; case TT_LE: e->t = TT_GT; break;
                    case TT_GT: e->t = TT_LE; break; case TT_EQ: e->t = TT_NE; break; case TT_NE: e->t = TT_EQ; break; default: break; }
    return e;
}
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
%type <node> case_elem constant_list
%type <list> case_list
%type <list> statement_list argument_list expression_list expression_list_opt id_list argument
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
          tree_t *mainp = mk_proc("main", NULL, body, 0, 0, NULL, 0); emit_proc(&g_pascal_procs, mainp);
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
type_decl: IDENT EQOP type SEMICOLON { if (g_pas_pend_ptrtarget) pas_ptrtype_add($1, g_pas_pend_ptrtarget); else if (g_pas_pend_nf > 0) pas_rectype_add($1); pas_pend_reset(); } ;
type:
    simple_type { $$ = (g_pas_pend_ptrtarget ? -3 : -1); }
    | ARROW IDENT { g_pas_pend_ptrtarget = strdup($2); $$ = -3; }
    | packed_opt ARRAYSY LBRACK simple_type RBRACK OFSY type { g_pas_pend_ptrtarget = NULL; $$ = $4; }
    | packed_opt RECORDSY record_body ENDSY { g_pas_pend_ptrtarget = NULL; $$ = -1; }
    | packed_opt SETSY OFSY simple_type { g_pas_pend_ptrtarget = NULL; $$ = -2; }
    | packed_opt FILESY { g_pas_pend_ptrtarget = NULL; $$ = -1; }
    ;
packed_opt: PACKEDSY | ;
simple_type:
    LPARENT id_list RPARENT
        { int _eo = 0;
          if ($2) for (int i = 0; i < $2->count; i++) {
              tree_t *_id = $2->items[i];
              if (_id && _id->v.sval) pas_const_add(_id->v.sval, (long long)(_eo++)); }
          $$ = -1; }
    | IDENT { g_pas_pend_typename = strdup($1); const char *_pt = pas_ptrtype_target($1); if (_pt) { g_pas_pend_ptrtarget = strdup(_pt); $$ = -3; } else { pas_rectype_to_pend($1); $$ = -1; } }
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
    id_list COLON type { if ($1) for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) pas_pend_add($1->items[i]->v.sval); }
    |
    ;
var_decl_list:
    var_decl_list var_decl
    | var_decl
    ;
var_decl: id_list COLON type SEMICOLON { if ($1) for (int i = 0; i < $1->count; i++) { tree_t *id = $1->items[i]; if (id && id->v.sval) { if ($3 == -3) { if (g_pas_pend_ptrtarget) pas_ptrvar_add(id->v.sval, g_pas_pend_ptrtarget); } else { if ($3 >= 0) pas_array_add(id->v.sval, $3); if ($3 == -2) pas_setvar_add(id->v.sval); if (g_pas_pend_nf > 0) { pas_recvar_add(id->v.sval); pas_array_add(id->v.sval, (long long)(g_pas_pend_nf - 1)); } } pas_local_add(id->v.sval); } } pas_pend_reset(); } ;
procedure_decl:
    PROCEDURESY IDENT parameter_list_opt SEMICOLON FORWARDSY SEMICOLON { }
    | FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON FORWARDSY SEMICOLON { pas_func_add($2); }
    | PROCEDURESY IDENT parameter_list_opt SEMICOLON { pas_proc_enter(); } block SEMICOLON
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc($2, $3, $6, 0, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
    | FUNCTIONSY IDENT parameter_list_opt COLON IDENT SEMICOLON { pas_func_add($2); pas_proc_enter(); } block SEMICOLON
        { int d = g_pas_ldepth - 1; int dl = (d >= 0) ? g_pas_lstk[d].decl_level : 1;
          const char **ln = (d >= 0) ? g_pas_lstk[d].names : NULL; int lc = (d >= 0) ? g_pas_lstk[d].n : 0;
          tree_t *p = mk_proc($2, $3, $8, 1, dl, ln, lc); pas_proc_exit(); emit_proc(&g_pascal_procs, p); }
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
    | VARSY id_list COLON IDENT { const char *_pt = pas_ptrtype_target($4); if (_pt) for (int i = 0; i < $2->count; i++) if ($2->items[i] && $2->items[i]->v.sval) pas_ptrvar_add($2->items[i]->v.sval, _pt); for (int i = 0; i < $2->count; i++) if ($2->items[i]) ast_push($2->items[i], ast_node_new(TT_SUCCEED)); $$ = $2; }
    | id_list COLON IDENT { const char *_pt = pas_ptrtype_target($3); if (_pt) for (int i = 0; i < $1->count; i++) if ($1->items[i] && $1->items[i]->v.sval) pas_ptrvar_add($1->items[i]->v.sval, _pt); $$ = $1; }
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
    | INTCONST COLON statement_no_label
        { char _lb[24]; snprintf(_lb, sizeof _lb, "%lld", (long long)$1);
          tree_t *L = ast_node_new(TT_LABEL_DEF); L->v.sval = strdup(_lb); ast_push(L, $3); $$ = L; }
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
    expression { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool($1)); pnl_push(_al, ilit(-1)); $$ = _al; }
    | expression COLON expression { PNodeList *_al = pnl_new(); pnl_push(_al, pas_bool($1)); pnl_push(_al, $3); $$ = _al; }
    ;
assignment:
    selector BECOMES expression
        { int _fnsel = ($1 && $1->t == TT_FNC && $1->n == 1 && $1->c[0] && $1->c[0]->t == TT_VAR && $1->c[0]->v.sval);
          if ($1 && ($1->t == TT_VAR || _fnsel) && pas_is_rel($3)) {
              tree_t *e = ast_node_new(TT_IF);
              ast_push(e, $3);
              ast_push(e, mk_assign($1, ilit(1)));
              tree_t *s2;
              if (_fnsel) { s2 = ast_node_new(TT_FNC); ast_push(s2, leaf_s(TT_VAR, $1->c[0]->v.sval)); }
              else s2 = leaf_s(TT_VAR, $1->v.sval);
              ast_push(e, mk_assign(s2, ilit(0)));
              $$ = e;
          } else { $$ = mk_assign($1, pas_bool($3)); } }
    ;
selector:
    selector LBRACK expression_list RBRACK { tree_t *e = ast_node_new(TT_IDX); ast_push(e, $1); if ($3) for (int i = 0; i < $3->count; i++) ast_push(e, $3->items[i]); $$ = e; }
    | selector PERIOD IDENT { int _fi = -1; const char *_rt = pas_selector_rectype($1); if (_rt) _fi = pas_rectype_field_index(_rt, $3); else if ($1 && $1->t == TT_VAR && $1->v.sval) _fi = pas_recvar_field_index($1->v.sval, $3);
        if (_fi >= 0) { tree_t *e = ast_node_new(TT_IDX); ast_push(e, $1); ast_push(e, ilit(_fi)); $$ = e; } else { $$ = bin(TT_FIELD, $1, leaf_s(TT_VAR, $3)); } }
    | selector ARROW { $$ = mk_deref($1); }
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
    GOTOSY INTCONST
        { char _gb[24]; snprintf(_gb, sizeof _gb, "%lld", (long long)$2);
          tree_t *G = ast_node_new(TT_GOTO_U); G->v.sval = strdup(_gb); $$ = G; }
    ;
if_statement:
    IFSY expression THENSY statement { $$ = bin(TT_IF, pas_cond($2), $4); }
    | IFSY expression THENSY statement ELSESY statement { tree_t *e = ast_node_new(TT_IF); ast_push(e, pas_cond($2)); ast_push(e, $4); ast_push(e, $6); $$ = e; }
    ;
case_statement:
    CASESY expression OFSY { pas_case_push(); } case_list ENDSY
        { tree_t *seq = ast_node_new(TT_SEQ_EXPR);
          ast_push(seq, bin(TT_ASSIGN, leaf_s(TT_VAR, pas_case_cur()), $2));
          tree_t *chain = NULL;
          if ($5) for (int i = $5->count - 1; i >= 0; i--) { tree_t *e = $5->items[i]; if (!e) continue; if (chain) ast_push(e, chain); chain = e; }
          ast_push(seq, chain ? chain : ast_node_new(TT_SUCCEED));
          pas_case_pop();
          $$ = seq; }
    ;
case_list:
    case_list SEMICOLON case_elem { if ($3) pnl_push($1, $3); $$ = $1; }
    | case_elem { PNodeList *l = pnl_new(); if ($1) pnl_push(l, $1); $$ = l; }
    ;
case_elem:
    constant_list COLON statement { $$ = bin(TT_IF, pas_cond($1), $3); }
    | { $$ = NULL; }
    ;
constant_list:
    constant_list COMMA constant { $$ = bin(TT_ADD, $1, bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit($3))); }
    | constant { $$ = bin(TT_EQ, leaf_s(TT_VAR, pas_case_cur()), ilit($1)); }
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
    | expression INOP simple_expression { $$ = mk_in($1, $3); }
    | expression LTOP simple_expression { $$ = bin(TT_LT, $1, $3); }
    | expression LEOP simple_expression { $$ = pas_arith_or_set(TT_LE, "__pas_subset", $1, $3); }
    | expression GTOP simple_expression { $$ = bin(TT_GT, $1, $3); }
    | expression GEOP simple_expression { $$ = pas_arith_or_set(TT_GE, "__pas_super", $1, $3); }
    | expression NEOP simple_expression { $$ = bin(TT_NE, $1, $3); }
    | expression EQOP simple_expression { $$ = bin(TT_EQ, $1, $3); }
    ;
simple_expression:
    term { $$ = $1; }
    | PLUS term { $$ = $2; }
    | MINUS term { $$ = un(TT_MNS, $2); }
    | simple_expression PLUS term { $$ = pas_arith_or_set(TT_ADD, "__pas_setuni", $1, $3); }
    | simple_expression MINUS term { $$ = pas_arith_or_set(TT_SUB, "__pas_setdif", $1, $3); }
    | simple_expression OROP term { $$ = bin(TT_ADD, $1, $3); }
    ;
term:
    factor { $$ = $1; }
    | term MUL factor { $$ = pas_arith_or_set(TT_MUL, "__pas_setint", $1, $3); }
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
    | NOTSY factor { $$ = pas_flip_rel(pas_cond($2)); }
    | LBRACK expression_list_opt RBRACK { $$ = mk_set_ctor($2); }
    ;
expression_list_opt:
    expression_list { $$ = $1; }
    | { $$ = NULL; }
    ;
%%
extern void *pascal_yy_scan_string(const char *);
extern void  pascal_yy_delete_buffer(void *);
tree_t *pascal_parse_string(const char *src) {
    pascal_prog_result = NULL;
    memset(&g_pascal_procs, 0, sizeof g_pascal_procs);
    g_pas_nconst = 0; g_pas_narray = 0; g_pas_nfunc = 0;
    g_pas_nrectype = 0; g_pas_nrecvar = 0; g_pas_pend_nf = 0; g_pas_nsetvar = 0;
    g_pas_nptrtype = 0; g_pas_nptrvar = 0; g_pas_pend_ptrtarget = NULL; g_pas_pend_typename = NULL;
    g_pas_level = 1; g_pas_ldepth = 0; g_pas_case_depth = 0; g_pas_case_ctr = 0;
    void *buf = pascal_yy_scan_string(src);
    pascal_yyparse();
    pascal_yy_delete_buffer(buf);
    return pascal_prog_result;
}
