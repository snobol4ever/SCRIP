#include "lower_clause.h"
#include "BB.h"
#include "ast.h"
#include "../frontend/prolog/prolog_atom.h"
#include <gc/gc.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
static IR_t *lower_pl_term(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_goal(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static int   resolve_goal_is_bounded(const tree_t *e);
static IR_t *lower_pl_new_Alt(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Ite(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Unify(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Compare(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Conj(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Call(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static IR_t *lower_pl_new_Builtin(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out);
static int flatten_comma(const tree_t *e, const tree_t **out, int cap);
static IR_t *resolve_leaf(IR_t *bb, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!bb) return NULL;
    bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb;
    if (β_out) *β_out = ω_in;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Alt(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *bα = NULL, *bβ = NULL;
    IR_t *b = lower_pl_goal(bbg, e->c[1], γ_in, ω_in, &bα, &bβ); if (!b) return NULL;
    IR_t *aα = NULL, *aβ = NULL;
    IR_t *a = lower_pl_goal(bbg, e->c[0], γ_in, bα,  &aα, &aβ); if (!a) return NULL;
    IR_t *bb = BB_node_alloc(bbg, IR_DISJ); if (!bb) return NULL;
    bb->α = aα; bb->β = bα;
    bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = aα; if (β_out) *β_out = bα;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Ite(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    const tree_t *cond = e->c[0];
    const tree_t *then_ = e->c[1];
    const tree_t *else_ = (e->n >= 3) ? e->c[2] : NULL;
    IR_t *bα = NULL, *bβ = NULL, *b = NULL;
    if (else_) { b = lower_pl_goal(bbg, else_, γ_in, ω_in, &bα, &bβ); if (!b) return NULL; }
    else       { b = BB_node_alloc(bbg, IR_FAIL); if (!b) return NULL; resolve_leaf(b, γ_in, ω_in, &bα, &bβ); }
    IR_t *tα = NULL, *tβ = NULL;
    IR_t *t = lower_pl_goal(bbg, then_, γ_in, ω_in, &tα, &tβ); if (!t) return NULL;
    IR_t *cα = NULL, *cβ = NULL;
    IR_t *c = lower_pl_goal(bbg, cond, tα, bα, &cα, &cβ); if (!c) return NULL;
    IR_t *ite = BB_node_alloc(bbg, IR_ITE); if (!ite) return NULL;
    ite->α = cα; ite->γ = γ_in; ite->ω = ω_in;
    {
        bb_ite_state_t *zi = (bb_ite_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zi);
        if (zi) { zi->cond = cα; zi->then_ = tα; zi->else_ = bα; ite->ival = (int64_t)(intptr_t)zi; }
    }
    if (α_out) *α_out = ite; if (β_out) *β_out = ω_in;
    return ite;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Unify(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    IR_t *lα = NULL, *lβ = NULL, *rα = NULL, *rβ = NULL;
    IR_t *lhs = lower_pl_term(bbg, e->c[0], γ_in, ω_in, &lα, &lβ); if (!lhs) return NULL;
    IR_t *rhs = lower_pl_term(bbg, e->c[1], γ_in, ω_in, &rα, &rβ); if (!rhs) return NULL;
    IR_t *bb = BB_node_alloc(bbg, IR_UNIFY); if (!bb) return NULL;
    bb->α = lα; bb->β = rα; bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb; if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Compare(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (e->n < 2) return NULL;
    IR_t *lα=NULL,*lβ=NULL,*rα=NULL,*rβ=NULL;
    IR_t *lhs = lower_pl_term(bbg,e->c[0],γ_in,ω_in,&lα,&lβ); if(!lhs) return NULL;
    IR_t *rhs = lower_pl_term(bbg,e->c[1],γ_in,ω_in,&rα,&rβ); if(!rhs) return NULL;
    IR_t *bb = BB_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = (e->t==TT_GT)?">":(e->t==TT_LT)?"<":(e->t==TT_GE)?">=":(e->t==TT_LE)?"<=":(e->t==TT_EQ)?"=:=":"=\=";
    bb->α = lα; bb->β = rα; bb->γ = γ_in; bb->ω = ω_in;
    if (α_out) *α_out = bb; if (β_out) *β_out = bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_node_is_resumable(const IR_t *nd) {
    if (!nd) return 0;
    return nd->t == IR_GOAL || nd->t == IR_CHOICE || nd->t == IR_DISJ;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Conj(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    const tree_t *flat[256];
    int n = flatten_comma(e, flat, 256);
    if (n <= 0) return NULL;
    if (n == 1) return lower_pl_goal(bbg, flat[0], γ_in, ω_in, α_out, β_out);
    IR_t **gα = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    IR_t **gβ = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    IR_t **gnodes = (IR_t **)GC_MALLOC((size_t)n * sizeof(IR_t *));
    {
        IR_t *bb = lower_pl_goal(bbg, flat[n-1], γ_in, NULL, &gα[n-1], &gβ[n-1]);
        if (!bb) return NULL; gnodes[n-1] = bb;
    }
    for (int i = n-2; i >= 0; i--) {
        IR_t *my_γ = gα[i+1];
        IR_t *my_ω = (i == 0) ? ω_in : NULL;
        IR_t *bb = lower_pl_goal(bbg, flat[i], my_γ, my_ω, &gα[i], &gβ[i]);
        if (!bb) return NULL; gnodes[i] = bb;
    }
    gβ[0] = resolve_node_is_resumable(gnodes[0]) ? gnodes[0] : ω_in;
    for (int i = 1; i < n; i++) {
        gβ[i] = resolve_node_is_resumable(gnodes[i]) ? gnodes[i] : gβ[i-1];
    }
    for (int i = 1; i < n; i++) if (!gnodes[i]->ω) gnodes[i]->ω = gβ[i-1];
    IR_t *seq = BB_node_alloc(bbg, IR_GCONJ); if (!seq) return NULL;
    seq->α = gα[0]; seq->γ = γ_in; seq->ω = ω_in;
    {
        bb_conj_state_t *zs = (bb_conj_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zs);
        if (zs) {
            zs->goals  = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n * sizeof(IR_t *));
            zs->ngoals = n;
            for (int i = 0; i < n; i++) zs->goals[i] = gnodes[i];
            seq->ival = (int64_t)(intptr_t)zs;
        }
    }
    if (α_out) *α_out = seq;
    if (β_out) *β_out = seq;
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Call(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    int n = e ? e->n : 0;
    IR_t *bb = BB_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
    bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
    zc->callee=fn; zc->arity=n; zc->cs=NULL;
    zc->args = (n>0) ? (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n*sizeof(IR_t *)) : NULL;
    zc->nargs=0;
    for (int i=0; i<n; i++) {
        IR_t *aα=NULL,*aβ=NULL;
        IR_t *a=lower_pl_term(bbg,e->c[i],γ_in,ω_in,&aα,&aβ); if(!a) return NULL;
        zc->args[zc->nargs++]=aα;
    }
    bb->sval=fn; bb->ival=(int64_t)(intptr_t)zc;
    if (n>=1) bb->α=zc->args[0];
    if (n>=2) bb->β=zc->args[1];
    bb->γ=γ_in; bb->ω=ω_in;
    if (α_out) *α_out=bb; if (β_out) *β_out=bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
enum { RESOLVE_BI_NONE=0, RESOLVE_BI_AB, RESOLVE_BI_CHAIN, RESOLVE_BI_CHAIN_ABOLISH, RESOLVE_BI_TYPETEST };
static int resolve_builtin_style(const char *fn, int n) {
    if (strcmp(fn,"write")==0||strcmp(fn,"writeln")==0||strcmp(fn,"is")==0
        ||strcmp(fn,">")==0||strcmp(fn,"<")==0||strcmp(fn,">=")==0||strcmp(fn,"<=")==0
        ||strcmp(fn,"=:=")==0||strcmp(fn,"=\\=")==0||(strcmp(fn,"succ")==0&&n==2)
        ||strcmp(fn,"==")==0||strcmp(fn,"\\==")==0
        ||strcmp(fn,"@<")==0||strcmp(fn,"@>")==0||strcmp(fn,"@=<")==0||strcmp(fn,"@>=")==0) return RESOLVE_BI_AB;
    if (n==1 && (strcmp(fn,"var")==0||strcmp(fn,"nonvar")==0||strcmp(fn,"atom")==0||strcmp(fn,"atomic")==0
        ||strcmp(fn,"number")==0||strcmp(fn,"integer")==0||strcmp(fn,"float")==0||strcmp(fn,"compound")==0
        ||strcmp(fn,"callable")==0||strcmp(fn,"is_list")==0||strcmp(fn,"ground")==0)) return RESOLVE_BI_TYPETEST;
    if ((strcmp(fn,"functor")==0 && n==3) || (strcmp(fn,"arg")==0 && n==3)
        || (strcmp(fn,"=..")==0 && n==2)
        || (strcmp(fn,"atom_length")==0 && n==2) || (strcmp(fn,"atom_concat")==0 && n==3)
        || (strcmp(fn,"atom_chars")==0 && n==2)  || (strcmp(fn,"atom_codes")==0 && n==2)
        || (strcmp(fn,"upcase_atom")==0 && n==2) || (strcmp(fn,"downcase_atom")==0 && n==2)
        || (strcmp(fn,"char_type")==0 && n==2)
        || (strcmp(fn,"atom_string")==0 && n==2) || (strcmp(fn,"number_string")==0 && n==2)
        || (strcmp(fn,"string_upper")==0 && n==2) || (strcmp(fn,"string_lower")==0 && n==2)
        || (strcmp(fn,"string_concat")==0 && n==3) || (strcmp(fn,"string_length")==0 && n==2)
        || (strcmp(fn,"string_chars")==0 && n==2)  || (strcmp(fn,"string_codes")==0 && n==2)
        || (strcmp(fn,"term_to_atom")==0 && n==2)  || (strcmp(fn,"term_string")==0 && n==2)
        || (strcmp(fn,"atom_number")==0 && n==2)
        || (strcmp(fn,"copy_term")==0 && n==2)
        || (strcmp(fn,"atomic_list_concat")==0 && (n==2||n==3)) || (strcmp(fn,"concat_atom")==0 && n==2)
        || (strcmp(fn,"string_to_atom")==0 && n==2)) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"sort")==0||strcmp(fn,"msort")==0) && n==2) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"format")==0 && (n==1||n==2)) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"numbervars")==0 && n==3) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"writeq")==0||strcmp(fn,"write_canonical")==0||strcmp(fn,"print")==0) && n==1) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"retract")==0||strcmp(fn,"retractall")==0) && n==1) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"abolish")==0 && n==1) return RESOLVE_BI_CHAIN_ABOLISH;
    if (strcmp(fn,"plus")==0 && n==3) return RESOLVE_BI_CHAIN;
    if ((strcmp(fn,"nb_setval")==0||strcmp(fn,"nb_getval")==0) && n==2) return RESOLVE_BI_CHAIN;
    if (strcmp(fn,"aggregate_all")==0 && n==3) return RESOLVE_BI_CHAIN;
    return RESOLVE_BI_NONE;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_new_Builtin(IR_graph_t *bbg, const char *fn, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    int style = resolve_builtin_style(fn, e->n);
    if (style == RESOLVE_BI_NONE) return NULL;
    IR_t *bb = BB_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL;
    bb->sval = fn; bb->ival = e->n;
    if (style == RESOLVE_BI_AB) {
        if (e->n>=1) { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[0],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; bb->α=aα; }
        if (e->n>=2) { IR_t *bα=NULL,*bβ=NULL; IR_t *b=lower_pl_term(bbg,e->c[1],γ_in,ω_in,&bα,&bβ); if(!b) return NULL; bb->β=bα; }
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    if (style == RESOLVE_BI_TYPETEST) {
        IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[0],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; bb->α=aα;
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    IR_t *prev = NULL;
    for (int i=0; i<e->n; i++) {
        IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[i],NULL,NULL,&aα,&aβ); if(!a) return NULL;
        if (i==0) bb->α=aα; else prev->γ=aα;
        prev = aα;
    }
    bb->γ=γ_in; bb->ω=ω_in;
    if (α_out) *α_out=bb;
    if (β_out) *β_out = (style==RESOLVE_BI_CHAIN_ABOLISH) ? ω_in : bb;
    return bb;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_is_arith_functor(const char *fn, int arity) {
    if (!fn) return 0;
    if (arity == 0)
        return strcmp(fn,"pi")==0 || strcmp(fn,"e")==0;
    if (arity == 1)
        return strcmp(fn,"-")==0 || strcmp(fn,"+")==0 || strcmp(fn,"abs")==0 || strcmp(fn,"sign")==0
            || strcmp(fn,"sqrt")==0 || strcmp(fn,"sin")==0 || strcmp(fn,"cos")==0 || strcmp(fn,"tan")==0
            || strcmp(fn,"asin")==0 || strcmp(fn,"acos")==0 || strcmp(fn,"atan")==0
            || strcmp(fn,"exp")==0 || strcmp(fn,"log")==0
            || strcmp(fn,"float")==0 || strcmp(fn,"integer")==0
            || strcmp(fn,"float_integer_part")==0 || strcmp(fn,"float_fractional_part")==0
            || strcmp(fn,"truncate")==0 || strcmp(fn,"round")==0
            || strcmp(fn,"ceiling")==0 || strcmp(fn,"floor")==0
            || strcmp(fn,"\\")==0 || strcmp(fn,"msb")==0;
    if (arity == 2)
        return strcmp(fn,"+")==0 || strcmp(fn,"-")==0 || strcmp(fn,"*")==0 || strcmp(fn,"/")==0
            || strcmp(fn,"//")==0 || strcmp(fn,"mod")==0 || strcmp(fn,"rem")==0 || strcmp(fn,"div")==0
            || strcmp(fn,"**")==0 || strcmp(fn,"^")==0
            || strcmp(fn,"min")==0 || strcmp(fn,"max")==0 || strcmp(fn,"gcd")==0
            || strcmp(fn,"/\\")==0 || strcmp(fn,"\\/")==0 || strcmp(fn,"xor")==0
            || strcmp(fn,"<<")==0 || strcmp(fn,">>")==0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_term(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!e) return NULL;
    switch (e->t) {
    case TT_ILIT: { IR_t *bb = BB_node_alloc(bbg, IR_LIT_I); if (!bb) return NULL; bb->ival = e->v.ival; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_FLIT: { IR_t *bb = BB_node_alloc(bbg, IR_LIT_F); if (!bb) return NULL; bb->dval = e->v.dval; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_QLIT: case TT_NAME: { IR_t *bb = BB_node_alloc(bbg, IR_ATOM); if (!bb) return NULL; bb->sval = e->v.sval; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_VAR: { IR_t *bb = BB_node_alloc(bbg, IR_LOGICVAR); if (!bb) return NULL; bb->ival = e->v.ival; bb->sval = NULL; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
    case TT_MAKELIST: {
        int has_tail = (e->v.ival == 1);
        int n_elem   = has_tail ? e->n - 1 : e->n;
        IR_t *rest;
        if (has_tail && e->n >= 1) {
            IR_t *tα=NULL,*tβ=NULL; rest = lower_pl_term(bbg, e->c[e->n-1], NULL, NULL, &tα, &tβ); if (!rest) return NULL; rest = tα;
        } else {
            IR_t *nil = BB_node_alloc(bbg, IR_ATOM); if (!nil) return NULL; nil->sval = "[]"; rest = nil;
        }
        for (int i = n_elem - 1; i >= 0; i--) {
            IR_t *hα=NULL,*hβ=NULL; IR_t *head = lower_pl_term(bbg, e->c[i], NULL, NULL, &hα, &hβ); if (!head) return NULL;
            IR_t *cons = BB_node_alloc(bbg, IR_STRUCT); if (!cons) return NULL;
            cons->sval = "."; cons->ival = 2;
            cons->α = hα; hα->γ = rest;
            rest = cons;
        }
        rest->γ = γ_in; rest->ω = ω_in;
        if (α_out) *α_out = rest; if (β_out) *β_out = rest;
        return rest;
    }
    case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: {
        if (e->n < 2) return NULL;
        IR_t *lα = NULL, *lβ = NULL, *rα = NULL, *rβ = NULL;
        IR_t *lhs = lower_pl_term(bbg, e->c[0], NULL, NULL, &lα, &lβ); if (!lhs) return NULL;
        IR_t *rhs = lower_pl_term(bbg, e->c[1], NULL, NULL, &rα, &rβ); if (!rhs) return NULL;
        IR_t *bb = BB_node_alloc(bbg, IR_ARITH); if (!bb) return NULL;
        bb->sval = (e->t==TT_ADD)?"+":(e->t==TT_SUB)?"-":(e->t==TT_MUL)?"*":(e->t==TT_DIV)?"/":"mod";
        bb->ival = 2;
        bb->α = lα; bb->β = rα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = bb;
        return bb;
    }
    case TT_FNC: {
        const char *fn = e->v.sval;
        if (!fn) return NULL;
        if (resolve_is_arith_functor(fn, e->n)) {
            IR_t *bb = BB_node_alloc(bbg, IR_ARITH); if (!bb) return NULL;
            bb->sval = fn; bb->ival = e->n;
            if (e->n >= 1) {
                IR_t *lα=NULL,*lβ=NULL; IR_t *l = lower_pl_term(bbg, e->c[0], NULL, NULL, &lα, &lβ); if (!l) return NULL; bb->α = lα;
            }
            if (e->n >= 2) {
                IR_t *rα=NULL,*rβ=NULL; IR_t *r = lower_pl_term(bbg, e->c[1], NULL, NULL, &rα, &rβ); if (!r) return NULL; bb->β = rα;
            }
            bb->γ = γ_in; bb->ω = ω_in;
            if (α_out) *α_out = bb; if (β_out) *β_out = bb;
            return bb;
        }
        if (e->n == 0) { IR_t *bb = BB_node_alloc(bbg, IR_ATOM); if (!bb) return NULL; bb->sval = fn; return resolve_leaf(bb, γ_in, ω_in, α_out, β_out); }
        {
            IR_t *st = BB_node_alloc(bbg, IR_STRUCT); if (!st) return NULL;
            st->sval = fn; st->ival = e->n; st->γ = γ_in; st->ω = ω_in;
            IR_t *prev = NULL;
            for (int i = 0; i < e->n; i++) {
                IR_t *aα = NULL, *aβ = NULL;
                IR_t *arg = lower_pl_term(bbg, e->c[i], NULL, NULL, &aα, &aβ); if (!arg) return NULL;
                if (i == 0) st->α = aα; else if (prev) prev->γ = aα;
                prev = aα;
            }
            if (α_out) *α_out = st; if (β_out) *β_out = st;
            return st;
        }
    }
    default: return NULL;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int flatten_comma(const tree_t *e, const tree_t **out, int cap) {
    if (!e || cap <= 0) return 0;
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2) {
        int L = flatten_comma(e->c[0], out, cap);
        return L + flatten_comma(e->c[1], out+L, cap-L);
    }
    out[0] = e; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int resolve_goal_is_bounded(const tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_CUT) return 1;
    if (e->t == TT_UNIFY) return 1;
    if (e->t==TT_GT||e->t==TT_LT||e->t==TT_GE||e->t==TT_LE||e->t==TT_EQ||e->t==TT_NE) return 1;
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2)
        return resolve_goal_is_bounded(e->c[0]) && resolve_goal_is_bounded(e->c[1]);
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,";")==0 && e->n==2) return 0;
    if (e->t == TT_IF && e->n >= 2) {
        const tree_t *then_ = e->c[1];
        const tree_t *else_ = (e->n >= 3) ? e->c[2] : NULL;
        if (then_ && !resolve_goal_is_bounded(then_)) return 0;
        if (else_ && !resolve_goal_is_bounded(else_)) return 0;
        return 1;
    }
    if (e->t == TT_UNIFY || (e->t==TT_FNC && e->v.sval && strcmp(e->v.sval,"=")==0 && e->n==2)) return 1;
    if (e->t == TT_QLIT && e->v.sval) {
        const char *fn = e->v.sval;
        if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0||strcmp(fn,"fail")==0||strcmp(fn,"false")==0) return 1;
        if (strcmp(fn,"nl")==0||strcmp(fn,"!")==0) return 1;
        return 0;
    }
    if (e->t == TT_VAR) return 0;
    if (e->t != TT_FNC || !e->v.sval) return 0;
    const char *fn = e->v.sval;
    if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0||strcmp(fn,"fail")==0||strcmp(fn,"false")==0||strcmp(fn,"nl")==0) return 1;
    if (resolve_builtin_style(fn, e->n) != RESOLVE_BI_NONE) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_t *lower_pl_goal(IR_graph_t *bbg, const tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    if (!e) return NULL;
    if (getenv("SCRIP_PL_BOUNDED_TRACE")) {
        const char *gn = (e->t==TT_FNC && e->v.sval) ? e->v.sval : (e->t==TT_QLIT && e->v.sval) ? e->v.sval : (e->t==TT_CUT) ? "!" : (e->t==TT_VAR) ? "<var>" : "<op>";
        fprintf(stderr, "[PL-BOUNDED] goal=%-12s bounded=%d\n", gn, resolve_goal_is_bounded(e));
    }
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,",")==0 && e->n==2) {
        return lower_pl_new_Conj(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_FNC && e->v.sval && strcmp(e->v.sval,";")==0 && e->n==2) {
        return lower_pl_new_Alt(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_IF && e->n >= 2) {
        return lower_pl_new_Ite(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_UNIFY || (e->t==TT_FNC && e->v.sval && strcmp(e->v.sval,"=")==0 && e->n==2)) {
        return lower_pl_new_Unify(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t==TT_GT||e->t==TT_LT||e->t==TT_GE||e->t==TT_LE||e->t==TT_EQ||e->t==TT_NE) {
        return lower_pl_new_Compare(bbg, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_CUT) return resolve_leaf(BB_node_alloc(bbg, IR_CUT), γ_in, ω_in, α_out, β_out);
    if (e->t == TT_QLIT && e->v.sval) {
        const char *fn = e->v.sval;
        if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0) return resolve_leaf(BB_node_alloc(bbg,IR_SUCCEED),γ_in,ω_in,α_out,β_out);
        if (strcmp(fn,"fail")==0||strcmp(fn,"false")==0)     return resolve_leaf(BB_node_alloc(bbg,IR_FAIL),γ_in,ω_in,α_out,β_out);
        if (strcmp(fn,"nl")==0)   { IR_t *bb=BB_node_alloc(bbg,IR_BUILTIN); if(!bb) return NULL; bb->sval=fn; return resolve_leaf(bb,γ_in,ω_in,α_out,β_out); }
        if (strcmp(fn,"!")==0)    return resolve_leaf(BB_node_alloc(bbg,IR_CUT),γ_in,ω_in,α_out,β_out);
        return lower_pl_new_Call(bbg, fn, e, γ_in, ω_in, α_out, β_out);
    }
    if (e->t == TT_VAR) {
        IR_t *bb = BB_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
        bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
        zc->callee = "call"; zc->arity = 1; zc->cs = NULL;
        zc->args = (IR_t **)GC_MALLOC_UNCOLLECTABLE(sizeof(IR_t *)); zc->nargs = 0;
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *a = lower_pl_term(bbg, e, γ_in, ω_in, &aα, &aβ); if (!a) return NULL;
        zc->args[zc->nargs++] = aα;
        bb->sval = "call"; bb->ival = (int64_t)(intptr_t)zc;
        bb->α = zc->args[0];
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb;
        if (β_out) *β_out = bb;
        return bb;
    }
    if (e->t != TT_FNC || !e->v.sval) return NULL;
    const char *fn = e->v.sval;
    if (strcmp(fn,"true")==0||strcmp(fn,"otherwise")==0) return resolve_leaf(BB_node_alloc(bbg,IR_SUCCEED),γ_in,ω_in,α_out,β_out);
    if (strcmp(fn,"fail")==0||strcmp(fn,"false")==0)     return resolve_leaf(BB_node_alloc(bbg,IR_FAIL),γ_in,ω_in,α_out,β_out);
    if (strcmp(fn,"nl")==0) { IR_t *bb=BB_node_alloc(bbg,IR_BUILTIN); if(!bb) return NULL; bb->sval=fn; return resolve_leaf(bb,γ_in,ω_in,α_out,β_out); }
    if (resolve_builtin_style(fn, e->n) != RESOLVE_BI_NONE) return lower_pl_new_Builtin(bbg, fn, e, γ_in, ω_in, α_out, β_out);
    if (strcmp(fn,"phrase")==0 && (e->n==2 || e->n==3)) {
        const tree_t *nt = e->c[0];
        const char *callee = NULL; int nt_arity = 0; const tree_t * const *nt_args = NULL;
        if (nt && (nt->t==TT_QLIT || nt->t==TT_NAME) && nt->v.sval) { callee = nt->v.sval; nt_arity = 0; }
        else if (nt && nt->t==TT_FNC && nt->v.sval) { callee = nt->v.sval; nt_arity = nt->n; nt_args = (const tree_t * const *)nt->c; }
        else return NULL;
        int call_arity = nt_arity + 2;
        IR_t *bb = BB_node_alloc(bbg, IR_GOAL); if (!bb) return NULL;
        bb_goal_state_t *zc = (bb_goal_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
        zc->callee=callee; zc->arity=call_arity; zc->cs=NULL;
        zc->args = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)call_arity*sizeof(IR_t *));
        zc->nargs=0;
        for (int i=0; i<nt_arity; i++) {
            IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,nt_args[i],γ_in,ω_in,&aα,&aβ); if(!a) return NULL;
            zc->args[zc->nargs++]=aα;
        }
        { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[1],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; zc->args[zc->nargs++]=aα; }
        if (e->n==3) { IR_t *aα=NULL,*aβ=NULL; IR_t *a=lower_pl_term(bbg,e->c[2],γ_in,ω_in,&aα,&aβ); if(!a) return NULL; zc->args[zc->nargs++]=aα; }
        else { IR_t *nil = BB_node_alloc(bbg, IR_ATOM); if (!nil) return NULL; nil->sval="[]"; nil->γ=γ_in; nil->ω=ω_in; zc->args[zc->nargs++]=nil; }
        bb->sval=callee; bb->ival=(int64_t)(intptr_t)zc;
        if (call_arity>=1) bb->α=zc->args[0];
        if (call_arity>=2) bb->β=zc->args[1];
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    if (strcmp(fn,"throw")==0 && e->n==1) {
        IR_t *bb = BB_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL; bb->sval="throw";
        IR_t *tα=NULL,*tβ=NULL; IR_t *t = lower_pl_term(bbg, e->c[0], γ_in, ω_in, &tα, &tβ); if (!t) return NULL;
        bb->α = tα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = ω_in;
        return bb;
    }
    if (strcmp(fn,"catch")==0 && e->n==3) {
        IR_t *bb = BB_node_alloc(bbg, IR_CATCH); if (!bb) return NULL;
        bb_catch_state_t *zc = (bb_catch_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc); if (!zc) return NULL;
        IR_t *cα=NULL,*cβ=NULL; IR_t *c = lower_pl_term(bbg, e->c[1], NULL, NULL, &cα, &cβ); if (!c) return NULL;
        zc->catcher = cα;
        IR_graph_t *gcfg = BB_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
        IR_t *gα=NULL,*gβ=NULL; IR_t *g = lower_pl_goal(gcfg, e->c[0], NULL, NULL, &gα, &gβ); if (!g) return NULL;
        gcfg->entry = gα ? gα : g;
        zc->goal_g = gcfg;
        IR_graph_t *rcfg = BB_alloc(128, IR_LANG_PL); if (!rcfg) return NULL;
        IR_t *rα=NULL,*rβ=NULL; IR_t *r = lower_pl_goal(rcfg, e->c[2], NULL, NULL, &rα, &rβ); if (!r) return NULL;
        rcfg->entry = rα ? rα : r;
        zc->rec_g = rcfg;
        bb->ival = (int64_t)(intptr_t)zc;
        bb->α = cα;
        bb->γ = γ_in; bb->ω = ω_in;
        if (α_out) *α_out = bb; if (β_out) *β_out = ω_in;
        return bb;
    }
    if (strcmp(fn,"findall")==0 && e->n==3) {
        IR_t *bb = BB_node_alloc(bbg, IR_BUILTIN); if (!bb) return NULL; bb->sval="findall"; bb->ival=0;
        bb_findall_state_t *fs = (bb_findall_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *fs);
        IR_t *tα=NULL,*tβ=NULL; IR_t *t=lower_pl_term(bbg,e->c[0],NULL,NULL,&tα,&tβ); if(!t) return NULL; fs->tmpl=tα;
        IR_t *rα=NULL,*rβ=NULL; IR_t *r=lower_pl_term(bbg,e->c[2],NULL,NULL,&rα,&rβ); if(!r) return NULL; fs->result=rα;
        IR_graph_t *gcfg = BB_alloc(128, IR_LANG_PL); if (!gcfg) return NULL;
        IR_t *gα=NULL,*gβ=NULL; IR_t *g=lower_pl_goal(gcfg,e->c[1],NULL,NULL,&gα,&gβ); if(!g) return NULL;
        gcfg->entry = gα ? gα : g;
        fs->gcfg = gcfg;
        bb->sval="findall"; bb->ival=(int64_t)(intptr_t)fs;
        bb->γ=γ_in; bb->ω=ω_in;
        if (α_out) *α_out=bb; if (β_out) *β_out=bb;
        return bb;
    }
    return lower_pl_new_Call(bbg, fn, e, γ_in, ω_in, α_out, β_out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static IR_graph_t *lower_pl_clause_body(const tree_t *clause, int n_args) {
    if (!clause || clause->t != TT_CLAUSE) return NULL;
    int n_body = clause->n - n_args;
    int n_total = n_args + (n_body > 0 ? n_body : 1);
    IR_graph_t *bbg = BB_alloc(128, IR_LANG_PL); if (!bbg) return NULL;
    const tree_t **stmts = (const tree_t **)calloc((size_t)n_total, sizeof(tree_t *));
    if (!stmts) { BB_free(bbg); return NULL; }
    int n_stmts = 0;
    for (int i=0; i<n_args; i++) if (clause->c[i]) stmts[n_stmts++] = clause->c[i];
    for (int i=0; i<n_body; i++) if (clause->c[n_args+i]) stmts[n_stmts++] = clause->c[n_args+i];
    if (n_stmts == 0) {
        IR_t *bb = BB_node_alloc(bbg, IR_SUCCEED); if (!bb) { free(stmts); BB_free(bbg); return NULL; }
        bb->γ = NULL; bb->ω = NULL;
        bbg->entry = bb; free(stmts); return bbg;
    }
    IR_t **nα = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    IR_t **nβ = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    IR_t **gnodes = (IR_t **)calloc((size_t)n_stmts, sizeof(IR_t *));
    if (!nα || !nβ || !gnodes) { free(stmts); free(nα); free(nβ); free(gnodes); BB_free(bbg); return NULL; }
    IR_t *succ = NULL;
    for (int i = n_stmts-1; i >= 0; i--) {
        const tree_t *st = stmts[i];
        IR_t *aα = NULL, *aβ = NULL;
        IR_t *bb;
        if (i < n_args) {
            IR_t *lα=NULL,*lβ=NULL,*rα=NULL,*rβ=NULL;
            IR_t *slot_var = BB_node_alloc(bbg, IR_LOGICVAR); if (!slot_var) goto fail;
            slot_var->ival = i; slot_var->sval = NULL;
            resolve_leaf(slot_var, succ, NULL, &lα, &lβ);
            IR_t *head_ir = lower_pl_term(bbg, st, succ, NULL, &rα, &rβ); if (!head_ir) goto fail;
            IR_t *uni = BB_node_alloc(bbg, IR_UNIFY); if (!uni) goto fail;
            uni->α = lα; uni->β = rα; uni->γ = succ; uni->ω = NULL;
            aα = uni; aβ = uni; bb = uni;
        } else {
            bb = lower_pl_goal(bbg, st, succ, NULL, &aα, &aβ); if (!bb) goto fail;
        }
        nα[i] = aα ? aα : bb;
        nβ[i] = aβ ? aβ : bb;
        gnodes[i] = bb;
        succ = nα[i];
    }
    {
        IR_t *eff_β_prev = NULL;
        for (int i = 0; i < n_stmts; i++) {
            int resumable = resolve_node_is_resumable(gnodes[i]);
            if (i > 0 && !gnodes[i]->ω) gnodes[i]->ω = eff_β_prev;
            eff_β_prev = resumable ? gnodes[i] : eff_β_prev;
        }
    }
    {
        IR_t *seq = BB_node_alloc(bbg, IR_GCONJ);
        if (!seq) goto fail;
        seq->α = nα[0]; seq->γ = NULL; seq->ω = NULL;
        bb_conj_state_t *zs = (bb_conj_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zs);
        if (zs) {
            zs->goals  = (IR_t **)GC_MALLOC_UNCOLLECTABLE((size_t)n_stmts * sizeof(IR_t *));
            zs->ngoals = n_stmts;
            for (int i = 0; i < n_stmts; i++) zs->goals[i] = gnodes[i];
            seq->ival = (int64_t)(intptr_t)zs;
        }
        bbg->entry = seq;
    }
    free(stmts); free(nα); free(nβ); free(gnodes);
    return bbg;
fail:
    free(stmts); free(nα); free(nβ); free(gnodes); BB_free(bbg); return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long resolve_clause_first_arg_key(const tree_t *clause, int n_args) {
    if (n_args < 1 || !clause || clause->n < 1) return RESOLVE_IDX_VAR;
    const tree_t *a0 = clause->c[0];
    if (!a0) return RESOLVE_IDX_VAR;
    switch (a0->t) {
    case TT_VAR: case TT_DEFER: case TT_INDIRECT: return RESOLVE_IDX_VAR;
    case TT_ILIT: return RESOLVE_IDX_INT(a0->v.ival);
    case TT_FLIT: return RESOLVE_IDX_FLT;
    case TT_QLIT: case TT_NAME: return RESOLVE_IDX_ATOM(prolog_atom_intern(a0->v.sval ? a0->v.sval : "[]"));
    case TT_MAKELIST: {
        int has_tail = (a0->v.ival == 1);
        int n_elem = has_tail ? a0->n - 1 : a0->n;
        if (n_elem <= 0 && !has_tail) return RESOLVE_IDX_ATOM(prolog_atom_intern("[]"));
        return RESOLVE_IDX_CMP(prolog_atom_intern("."), 2);
    }
    case TT_FNC: return RESOLVE_IDX_CMP(prolog_atom_intern(a0->v.sval ? a0->v.sval : "[]"), a0->n);
    default: return RESOLVE_IDX_VAR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_graph_t *lower_pl_predicate(tree_t *choice) {
    if (!choice || choice->t != TT_CHOICE || choice->n < 1) return NULL;
    const char *_csl = choice->v.sval ? strrchr(choice->v.sval, '/') : NULL;
    int arity = _csl ? atoi(_csl+1) : 0;
    if (choice->n == 1) return lower_pl_clause_body(choice->c[0], arity);
    IR_graph_t *bbg = BB_alloc(64, IR_LANG_PL); if (!bbg) return NULL;
    IR_t *bb = BB_node_alloc(bbg, IR_CHOICE); if (!bb) { BB_free(bbg); return NULL; }
    bb_choice_state_t *zc = (bb_choice_state_t *)GC_MALLOC_UNCOLLECTABLE(sizeof *zc);
    zc->bodies  = (IR_graph_t **)GC_MALLOC_UNCOLLECTABLE((size_t)choice->n * sizeof(IR_graph_t *));
    zc->nbodies = 0; zc->cur = 0; zc->mark = 0; zc->saved_env = NULL;
    zc->idx_key = (long *)GC_MALLOC_UNCOLLECTABLE((size_t)choice->n * sizeof(long));
    zc->idx_ok = 0;
    for (int i=0; i<choice->n; i++) {
        IR_graph_t *body = lower_pl_clause_body(choice->c[i], arity);
        if (!body) { BB_free(bbg); return NULL; }
        zc->idx_key[zc->nbodies] = resolve_clause_first_arg_key(choice->c[i], arity);
        zc->bodies[zc->nbodies++] = body;
    }
    zc->idx_ok = (arity >= 1) ? 1 : 0;
    bb->ival = (int64_t)(intptr_t)zc;
    bb->γ = NULL; bb->ω = NULL;
    bbg->entry = bb;
    return bbg;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
IR_t *lower_pl_threaded(IR_graph_t *bbg, const struct tree_t *e, IR_t *γ_in, IR_t *ω_in, IR_t **α_out, IR_t **β_out) {
    return lower_pl_goal(bbg, e, γ_in, ω_in, α_out, β_out);
}
